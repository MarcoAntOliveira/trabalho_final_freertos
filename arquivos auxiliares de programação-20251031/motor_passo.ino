const int stepPin = 2;
const int dirPin =5;

void setup() {
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
}

void loop() {
  digitalWrite(dirPin, HIGH);
  delayMicroseconds(10); // Tempo para o driver reconhecer a direção
  for (int i = 0; i < 50; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(1000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(1000);
  }

  delay(1000);

  digitalWrite(dirPin, LOW);
  delayMicroseconds(10); // Tempo para o driver reconhecer a direção
  for (int i = 0; i < 50; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(1000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(1000);
  }

  delay(1000);
}
