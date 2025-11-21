#include <AccelStepper.h>

AccelStepper motorZ(AccelStepper::DRIVER, 4, 7);  // STEP = 4, DIR = 7

bool etapa1 = false;
bool etapa2 = false;

void setup() {
  Serial.begin(9600);

  pinMode(8, OUTPUT);          // Pino EN do driver (se aplicável)
  digitalWrite(8, LOW);        // LOW = habilita driver

  // Configuração para máximo torque
  motorZ.setMaxSpeed(50);     // Baixa velocidade = mais torque
  motorZ.setAcceleration(10);  // Aceleração suave

  motorZ.moveTo(30);         // Desce 300 passos
  Serial.println("⏬ Descendo eixo Z...");
}

void loop() {
  motorZ.run();

  if (!etapa1 && motorZ.distanceToGo() == 0) {
    etapa1 = true;
    delay(1000);               // Espera 1 segundo
    motorZ.moveTo(-30);          // Sobe de volta
    Serial.println("⏫ Subindo eixo Z...");
  }

  if (etapa1 && !etapa2 && motorZ.distanceToGo() == 0) {
    etapa2 = true;
    Serial.println("✅ Teste concluído.");
  }
}
