#include <AccelStepper.h>

AccelStepper stepper1(AccelStepper::DRIVER, 2, 5);
AccelStepper stepper2(AccelStepper::DRIVER, 3, 6);
AccelStepper stepper3(AccelStepper::DRIVER, 4, 7);

// Pinos das chaves fim de curso
const int fimDeCursoX = 9;
const int fimDeCursoY = 10;
const int fimDeCursoZ = 11;

void setup() {
  Serial.begin(9600);

  // Define velocidades e aceleração
  stepper1.setMaxSpeed(400);
  stepper2.setMaxSpeed(400);
  stepper3.setMaxSpeed(400);

  stepper1.setAcceleration(50);
  stepper2.setAcceleration(50);
  stepper3.setAcceleration(50);

  // Define os pinos das chaves
  pinMode(fimDeCursoX, INPUT_PULLUP);
  pinMode(fimDeCursoY, INPUT_PULLUP);
  pinMode(fimDeCursoZ, INPUT_PULLUP);
  pinMode(8, OUTPUT);
  digitalWrite(8, LOW); // habilita os drivers

  // Executa homing
  goHome();

  // Zera a posição atual dos motores após encontrar o fim de curso
  stepper1.setCurrentPosition(0);
  stepper2.setCurrentPosition(0);
  stepper3.setCurrentPosition(0);

  Serial.println("🏁 Motores prontos na origem!");
}

void loop() {
  // Nada é feito no loop
}

void goHome() {
  bool muoviX = true;
  bool muoviY = true;
  bool muoviZ = true;

  // Move os motores até as chaves fim de curso serem ativadas
  while (muoviX || muoviY || muoviZ) {
    if (digitalRead(fimDeCursoX) == LOW) {
      stepper1.setSpeed(-200);
      stepper1.runSpeed();
    } else {
      muoviX = false;
      stepper1.stop();
    }

    if (digitalRead(fimDeCursoY) == LOW) {
      stepper2.setSpeed(-200);
      stepper2.runSpeed();
    } else {
      muoviY = false;
      stepper2.stop();
    }

    if (digitalRead(fimDeCursoZ) == LOW) {
      stepper3.setSpeed(-200);
      stepper3.runSpeed();
    } else {
      muoviZ = false;
      stepper3.stop();
    }
  }
}
