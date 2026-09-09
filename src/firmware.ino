// Pinos do Driver de Motor
const int pinENA = 9; // motor 1 PWM (Velocidade)
const int pinIN1 = 8; // motor 1 Sentido 1 - frente
const int pinIN2 = 7; // motor 1 Sentido 2 - trás



const int pinENB = 11; // motor 2 PWM (Velocidade)
const int pinIN3 = 12; // motor 2 sentido 1 - frente
const int pinIN4 = 13;  // motor 2 sentido 2 - trás

    unsigned long ultimoRecebidoW = 0;
const unsigned long timeoutMs = 300;

bool motorGirando = false;

void setup() {
  Serial.begin(115200);

  pinMode(pinENA, OUTPUT);
  pinMode(pinIN1, OUTPUT);
  pinMode(pinIN2, OUTPUT);

  pararMotor();
}

void loop() {

  while (Serial.available() > 0) {
    char c = Serial.read();

    if (c == 'w') {
      ultimoRecebidoW = millis();

      if (!motorGirando) {
        ligarMotor();
      }
    }
    if (c == 's') {
        ultimoRecebidoW = millis();
              if (!motorGirando) {
                 andarReverse();
              }
    }
    if (c == 'a') {
      virarEsquerda();
    }
  }

  if (motorGirando && (millis() - ultimoRecebidoW > timeoutMs)) {
    pararMotor();
  }
}

void ligarMotor() {
  digitalWrite(pinIN1, HIGH);
  digitalWrite(pinIN2, LOW);
  analogWrite(pinENA, 255);
  digitalWrite(pinIN3, HIGH);
  digitalWrite(pinIN4, LOW);
  analogWrite(pinENB, 255);
  motorGirando = true;
}

void andarReverse() {
  digitalWrite(pinIN1, LOW);
  digitalWrite(pinIN2, HIGH);
  analogWrite(pinENA, 255);
  motorGirando = true;
}

void virarEsquerda() {
  ligarMotor();
  digitalWrite(pinIN3, LOW);
  digitalWrite(pinIN4, HIGH);
  analogWrite(pinENB, 127);
}

void pararMotor() {
  digitalWrite(pinIN1, LOW);
  digitalWrite(pinIN2, LOW);
  analogWrite(pinENA, 0);
  motorGirando = false;
}
