#include <stdlib.h>
#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#include <wiring.h>
#endif
#include <AccelStepper.h>
#include <MultiStepper.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

//using namespace AccelStepper;
unsigned const int xnmotores=2;
SemaphoreHandle_t xmutexmotores = xSemaphoreCreateMutex(), semaforomotores[xnmotores];
/*for(int i=0; i<xnmotores; i++){
  semaforomotores[i]=xSemaphoreCreateBinary();
}*/
unsigned int xprioridademotor = 5, _HomeSpeed=100, _HomeAccel=100;

class Motor{ //guarda o objeto stepper e o semaforo associados a um motor
  public:
  AccelStepper* stepper;
  SemaphoreHandle_t semaforo;
  Motor(AccelStepper* st, SemaphoreHandle_t se){
    this->stepper=st;
    this->semaforo=se;
  }
};
struct pontos{
  int x, y;
};
typedef struct {
    Motor* motorx;
    Motor* motory;
    pontos destino;
} AndaCirculoParams;

int home(uint8_t pinoswitch0, uint8_t pinoswitch1, AccelStepper stepper){
    float speedvelha=stepper.speed();
    float acelvelha = stepper.acceleration();
    stepper.setSpeed(_HomeSpeed);
    stepper.setAcceleration(_HomeAccel);
    while(!digitalRead(pinoswitch0)){
      stepper.move(1);
      stepper.run();
    }
    int maxi=0;
    while(!digitalRead(pinoswitch1)){
      stepper.move(-1);
      stepper.run();
      maxi++;
    }
    stepper.setCurrentPosition(0);
    stepper.setSpeed(speedvelha);
    stepper.setAcceleration(acelvelha);
  return maxi;
}
void vNossaRunToPosition(void* param){
    Motor* motor = (Motor*)param;
    bool chegou=false;
    while (true) {
        xSemaphoreTake(xmutexmotores, portMAX_DELAY);  //garante que soh um motor dah passo por vez
        chegou=(!motor->stepper->run());  // dah um passo
        xSemaphoreGive(xmutexmotores);  //garante que soh um motor dah passo por vez
        if(chegou){
          xSemaphoreGive(motor->semaforo); //indica que o motor n~ao estah mais em uso
          vTaskDelete(NULL); //se morre
        }
        vTaskDelay((1000/motor->stepper->speed())/portTICK_PERIOD_MS);   // espera o intervalo entre passos antes de desbloquear a task
    }
}
void NossaRunToPosition(Motor* motor, long position){
  xSemaphoreTake(motor->semaforo, portMAX_DELAY);
  motor->stepper->moveTo(position);
    xTaskCreate(
        vNossaRunToPosition,
        "motor andando",
        2048,   
        (void*) motor,   // passa o objeto motor
        xprioridademotor,
        NULL  // handle opcional para ser utilizado em trocas de contexto
    );
}




void vAndaCirculo(void* AndaCirculoParameters) {

    AndaCirculoParams* p = (AndaCirculoParams*) AndaCirculoParameters;

    Motor* motorx = p->motorx;
    Motor* motory = p->motory;
    pontos destino = p->destino;
  bool chegou=false;
  int countx, county;
  while(!chegou){
    countx = uxSemaphoreGetCount(motorx->semaforo);
    county = uxSemaphoreGetCount(motory->semaforo);
    if(countx==1 && county==1){
      pontos ponto = calculacirculo(destino);
      NossaRunToPosition(motorx, ponto.x);
      NossaRunToPosition(motory, ponto.y);
  }else{
    vTaskDelay(100);
  }
  if(destino.x==motorx->stepper->currentPosition() && destino.y==motory->stepper->currentPosition()){
    vTaskDelete(NULL);
  }
  }
}

void AndaCirculo(Motor* motorx, Motor* motory, pontos destino){
  AndaCirculoParams param;
  param.motorx=motorx;
  param.destino=destino;
  param.motory=motory;
  xTaskCreate(
        vAndaCirculo,
        "motor andando em circulo",
        2048,   
         param,   // passa o objeto motor
        xprioridademotor,
        NULL  // handle opcional para ser utilizado em trocas de contexto
    );
}