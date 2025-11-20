#include <AccelStepper.h>
#include <MultiStepper.h>

AccelStepper motorX(AccelStepper::DRIVER, 2, 5);
AccelStepper motorY(AccelStepper::DRIVER, 3, 6);
MultiStepper steppers;

const int enablePin = 8;
const float passosPorMM = 80.0;
const float raioMM = .5;
const int segmentos = 5;

long circunferenciaX[segmentos + 1];
long circunferenciaY[segmentos + 1];
int pontoAtual = 0;

void setup() {
  Serial.begin(9600);

  pinMode(enablePin, OUTPUT);
  digitalWrite(enablePin, LOW); // Habilita driver

  motorX.setMaxSpeed(600);
  motorY.setMaxSpeed(600);
  motorX.setAcceleration(100);
  motorY.setAcceleration(100);

  steppers.addStepper(motorX);
  steppers.addStepper(motorY);

  Serial.println("🙌 Posicione o robô manualmente na origem e ligue.");

  delay(2000); // tempo para estabilizar após ligar

  // Define a posição atual como (0, 0)
  motorX.setCurrentPosition(0);
  motorY.setCurrentPosition(0);

  Serial.println("📍 Origem definida manualmente.");
  Serial.println("🌀 Calculando circunferência...");

  for (int i = 0; i <= segmentos; i++) {
    float angulo = 2 * PI * i / segmentos;
    circunferenciaX[i] = lround(raioMM * cos(angulo) * passosPorMM);
    circunferenciaY[i] = lround(raioMM * sin(angulo) * passosPorMM);
  }

  Serial.println("✅ Iniciando desenho contínuo.");
}

void loop() {
  long pos[2] = { circunferenciaX[pontoAtual], circunferenciaY[pontoAtual] };

  steppers.moveTo(pos);
  steppers.runSpeedToPosition();

  pontoAtual++;
  if (pontoAtual > segmentos) {
    pontoAtual = 0;
  }
}
