//Motor 1
#define PIN_IN1  27 // ESP32 pin GIOP27 connected to the IN1 pin L298N
#define PIN_IN2  26 // ESP32 pin GIOP26 connected to the IN2 pin L298N
//enable pin
#define PIN_ENA  14 // ESP32 pin GIOP14 connected to the EN1 pin L298N
//Motor 2
#define PIN_IN3 12
#define PIN_IN4 13

void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);

  // initialize digital pins as outputs.
  pinMode(PIN_IN1, OUTPUT); //AIN1
  pinMode(PIN_IN2, OUTPUT); //AIN2
  pinMode(PIN_ENA, OUTPUT); //enable pin 
  pinMode(PIN_IN3, OUTPUT); //BIN1
  pinMode(PIN_IN4, OUTPUT); //BIN2
}

void loop() {
  // put your main code here, to run repeatedly:

Motoren_Voorwaarts();
Serial.println("vooruit");
delay(3000);

Motoren_Stop();
Serial.println("stop");
delay(3000);

Motoren_Achterwaarts();
Serial.println("achteruit");
delay(3000);

Motoren_Stop();
Serial.println("stop");
delay(3000);

Motoren_Links();
Serial.println("links");
delay(3000);

Motoren_Stop();
Serial.println("stop");
delay(3000);

Motoren_Rechts();
Serial.println("rechts");
delay(3000);

Motoren_Stop();
Serial.println("stop");
delay(3000);
}


void Motoren_Voorwaarts() {
  digitalWrite(PIN_IN1, HIGH); // control the motor's direction in clockwise
  digitalWrite(PIN_IN2, LOW);  // control the motor's direction in clockwise
  digitalWrite(PIN_IN3, HIGH);
  digitalWrite(PIN_IN4, LOW);
  
  for (int speed = 0; speed <= 255; speed++) {
    analogWrite(PIN_ENA, speed); // speed up
    delay(10);
  }
}

void Motoren_Stop() {
  digitalWrite(PIN_IN1, LOW);
  digitalWrite(PIN_IN2, LOW);
  digitalWrite(PIN_IN3, LOW);
  digitalWrite(PIN_IN4, LOW);

}

void Motoren_Achterwaarts() {
  digitalWrite(PIN_IN1, LOW);   // control the motor's direction in anti-clockwise
  digitalWrite(PIN_IN2, HIGH);  // control the motor's direction in anti-clockwise
  digitalWrite(PIN_IN3, LOW);
  digitalWrite(PIN_IN4, HIGH);

  for (int speed = 0; speed <= 255; speed++) {
    analogWrite(PIN_ENA, speed); // speed up
    delay(10);
  }
}

void Motoren_Rechts() {
  digitalWrite(PIN_IN1, LOW);
  digitalWrite(PIN_IN2, HIGH);
  digitalWrite(PIN_IN3, LOW);
  digitalWrite(PIN_IN4, HIGH);

for (int speed = 0; speed <= 255; speed++) {
    analogWrite(PIN_ENA, speed); // speed up
    delay(10);
  }
}

void Motoren_Links() {
  digitalWrite(PIN_IN1, HIGH);
  digitalWrite(PIN_IN2, LOW);
  digitalWrite(PIN_IN3, HIGH);
  digitalWrite(PIN_IN4, LOW);

for (int speed = 0; speed <= 255; speed++) {
    analogWrite(PIN_ENA, speed); // speed up
    delay(10);
  }
}