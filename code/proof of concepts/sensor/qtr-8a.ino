
int IRSensor1 = 12;
int IRSensor2 = 14;
int IRSensor3 = 27;
int IRSensor4 = 34;
int IRSensor5 = 35;
int IRSensor6 = 32;
int IRSensor7 = 33;
//int IRSensor8 = 25;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);

}

void loop() {

Serial.println(" waarde: ");
Serial.print(analogRead(IRSensor1));
Serial.print("  ");
Serial.print(analogRead(IRSensor2));
Serial.print("  ");
Serial.print(analogRead(IRSensor3));
Serial.print("  ");
Serial.print(analogRead(IRSensor4));
Serial.print("  ");
Serial.print(analogRead(IRSensor5));
Serial.print("  ");
Serial.print(analogRead(IRSensor6));
Serial.print("  ");
Serial.print(analogRead(IRSensor7));
Serial.print("  ");
//Serial.print(analogRead(IRSensor8));
Serial.println();
delay(100);
}
