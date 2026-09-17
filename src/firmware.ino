// Pinos do Driver de Motor
const int pinENA = 9;   // Motor 1 PWM (Velocidade - Esquerda)
const int pinIN1 = 8;   // Motor 1 Sentido 1
const int pinIN2 = 7;   // Motor 1 Sentido 2

const int pinENB = 11;  // Motor 2 PWM (Velocidade - Direita)
const int pinIN3 = 12;  // Motor 2 Sentido 1
const int pinIN4 = 13;  // Motor 2 Sentido 2

unsigned long ultimoComandoRecebido = 0;
const unsigned long timeoutMs = 150;

bool motorGirando = false;


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

  digitalWrite(pinIN3, LOW);
  digitalWrite(pinIN4, HIGH);
  analogWrite(pinENB, 255);

  motorGirando = true;
}

void girarEsquerda() {
  digitalWrite(pinIN1, LOW);
  digitalWrite(pinIN2, HIGH);  
  analogWrite(pinENA, 180);

  digitalWrite(pinIN3, HIGH);
  digitalWrite(pinIN4, LOW);
  analogWrite(pinENB, 255);

  motorGirando = true;
}

void virarEsquerdaAndando() {
  // ligamos os dois motores no mesmo sentido, mas deixamos o do lado que queremos virar mais lento.
  digitalWrite(pinIN1, HIGH);
  digitalWrite(pinIN2, LOW);  
  analogWrite(pinENA, 180); 

  digitalWrite(pinIN3, HIGH);
  digitalWrite(pinIN4, LOW);
  analogWrite(pinENB, 255);

  motorGirando = true;
}

void virarDireitaAndando() {
  // ligamos os dois motores no mesmo sentido, mas deixamos o do lado que queremos virar mais lento.
  digitalWrite(pinIN1, HIGH);
  digitalWrite(pinIN2, LOW);  
  analogWrite(pinENA, 255); 

  digitalWrite(pinIN3, HIGH);
  digitalWrite(pinIN4, LOW);
  analogWrite(pinENB, 180);

  motorGirando = true;
}

void pararMotor() {
  digitalWrite(pinIN1, LOW);
  digitalWrite(pinIN2, LOW);
  digitalWrite(pinIN3, LOW);
  digitalWrite(pinIN4, LOW);

  analogWrite(pinENA, 0);
  analogWrite(pinENB, 0);

  motorGirando = false;
}


void setup() {
  Serial.begin(115200);
  Serial.setTimeout(10); 
  pinMode(pinENA, OUTPUT);
  pinMode(pinIN1, OUTPUT);
  pinMode(pinIN2, OUTPUT);

  pinMode(pinENB, OUTPUT);
  pinMode(pinIN3, OUTPUT);
  pinMode(pinIN4, OUTPUT);

  pararMotor();
}

void loop() {
  while (Serial.available() > 0) {
    String c = Serial.readStringUntil('\n');
    c.trim(); 

    if (c.length() > 0) {
      ultimoComandoRecebido = millis();

      if (c == "w") {
        ligarMotor();
      } 
      else if (c == "s") {
        andarReverse();
      } 
      else if (c == "a") {
        girarEsquerda();
      } 
      else if (c == "aw" || c == "wa") {
        virarEsquerdaAndando();
      }
      else if (c == "dw" || c == "wd") {
        virarDireitaAndando();
      }
      else if (c == "x") {
        pararMotor();
      }
    }
  }

  if (motorGirando && (millis() - ultimoComandoRecebido > timeoutMs)) {
    pararMotor();
  }
}

