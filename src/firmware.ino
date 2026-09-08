// Pinos do Driver de Motor
const int pinENA = 9;  // PWM (Velocidade)
const int pinIN1 = 8;  // Sentido 1
const int pinIN2 = 7;  // Sentido 2

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
    
  }

  if (motorGirando && (millis() - ultimoRecebidoW > timeoutMs)) {
    pararMotor();
  }
  
}

void ligarMotor() {
  digitalWrite(pinIN1, HIGH);
  digitalWrite(pinIN2, LOW);
  analogWrite(pinENA, 255); 
  motorGirando = true;
}

void andarReverse() {
  digitalWrite(pinIN1, LOW);
  digitalWrite(pinIN2, HIGH);
  analogWrite(pinENA, 255); 
}

void pararMotor() {
  digitalWrite(pinIN1, LOW);
  digitalWrite(pinIN2, LOW);
  analogWrite(pinENA, 0);
  motorGirando = false;
}