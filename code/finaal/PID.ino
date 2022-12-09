#include "SerialCommand.h"
#include "EEPROMAnything.h"
#include "EEPROM.h"
#include "BluetoothSerial.h"

#define SerialPort Serial
#define Baudrate 115200

#define MotorLeftForward 16//vooruit
#define MotorLeftBackward 17 //achteruit
#define MotorRightForward 19//vooruit
#define MotorRightBackward 18 //achteruit


#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif

BluetoothSerial SerialBT;


//SerialCommand sCmd(SerialPort);
SerialCommand sCmd(SerialBT);
bool debug;
bool run;
bool stop;
unsigned long previous, calculationTime;

const int sensor[] = {14, 27, 33, 35, 32, 34}; //34, 32, 35, 33, 27, 14 // 14, 27, 33, 35, 32, 34

struct param_t
{
  unsigned long cycleTime;
  /* andere parameters die in het eeprom geheugen moeten opgeslagen worden voeg je hier toe ... */
  int black[6];
  int white[6];
  int power;
  float diff;
  float kp;
  float ki;
  float kd;
} params;

int normalised[6];
float debugPosition;
float iTerm;
float lastErr;

//hier de voids zetten

void setup()
{
  SerialPort.begin(Baudrate);
  SerialBT.begin("ESP32janis");

  sCmd.addCommand("set", onSet);
  sCmd.addCommand("debug", onDebug);
  sCmd.addCommand("calibrate", onCalibrate);
  sCmd.addCommand("run", onRun);
  sCmd.addCommand("stop", onStop);
  sCmd.setDefaultHandler(onUnknownCommand);

  EEPROM.begin(4000);
  EEPROM_readAnything(0, params);

  pinMode(13, OUTPUT);
  SerialPort.println("ready");

}

void loop()
{
  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }
  if (SerialBT.available()) {
    Serial.write(SerialBT.read());
  }
  sCmd.readSerial();
 
  unsigned long current = micros();
  if (current - previous >= params.cycleTime)
  {
    previous = current;

    //int normalised[6];
    //normaliseren
    for (int i = 0; i < 6; i++ )
    {
      normalised[i] = map(analogRead(sensor[i]), params.black[i], params.white[i], 0, 4096);
    }

    //interpolatie

    int index = 0;
    for (int i = 1; i <6; i++) if (normalised[i] < normalised[index]) index = i;

    //if (normalised[index] > 3000) run = false; // gaat uit als hij geen lijn meer kan zien

    float position =0;
    if (index == 0) position = -30;
    else if (index == 5) position = 30;
    else 
    {
    int sNul = normalised[index];
    int sMinEen = normalised[index -1];
    int sPlusEen = normalised[index +1];

    float b = sPlusEen - sMinEen;
    b = b / 2;

    float a = sPlusEen - b - sNul;

    position = -b / (2 * a);
    position += index;
    position -= 2.5;

    //afstand tussen sensoren
    position *= 15;
    }
    debugPosition = position;

    //p regelaar
    float error = -position;
    float output = error * params.kp;

    //i regelaar
    iTerm += params.ki * error;
    iTerm = constrain(iTerm, -510, 510);
    output += iTerm;

    //d regelaar
    output += params.kd * (error - lastErr);
    lastErr = error;

    //constrain beperkingen opstellen
    output = constrain(output, -510, 510);

    int powerLeft = 0;
    int powerRight = 0;

    if (run) if (output >= 0)
    {
      powerLeft = constrain(params.power + params.diff * output, -255, 255);
      powerRight = constrain(powerLeft - output, -255, 255);
      powerLeft = powerRight + output;
    }
    else
    {
      powerRight = constrain(params.power - params.diff * output, -255, 255);
      powerLeft = constrain(powerRight + output, -255, 255);
      powerRight = powerLeft - output;
    }

    analogWrite(MotorLeftForward, powerLeft > 0 ? powerLeft : 0);
    analogWrite(MotorLeftBackward, powerLeft < 0 ? -powerLeft : 0);
    analogWrite(MotorRightForward, powerRight > 0 ? powerRight : 0);
    analogWrite(MotorRightBackward, powerRight < 0 ? - powerRight : 0);
    /* code die cyclisch moet uitgevoerd worden programmeer je hier ... */

    /* normaliseren en interpoleren sensor */

    /* pid regeling */

    /* aansturen motoren */
  }

  unsigned long difference = micros() - current;
  if (difference > calculationTime) calculationTime = difference;
}

void onUnknownCommand(char *command)
{
  SerialPort.print("unknown command: \"");
  SerialPort.print(command);
  SerialPort.println("\"");
}

void onSet()
{
  char* param = sCmd.next();
  char* value = sCmd.next();  
  
  if (strcmp(param, "cycle") == 0)
  {
    long newCycleTime = atol(value);
    float ratio = ((float) newCycleTime) / ((float) params.cycleTime);

    params.ki *= ratio;
    params.kd /= ratio;

    params.cycleTime = newCycleTime;
  }
  else if (strcmp(param, "ki") == 0)
  {
    float cycleTimeInSec = ((float) params.cycleTime) / 1000000;
    params.ki = atof(value) * cycleTimeInSec;
  }
  else if (strcmp(param, "kd") == 0)
  {
    float cycleTimeInSec = ((float) params.cycleTime) / 1000000;
    params.kd = atof(value) / cycleTimeInSec;
  }
  
  else if (strcmp(param, "power")== 0) params.power = atol(value);
  else if (strcmp(param, "diff")== 0) params.diff = atof(value);
  else if (strcmp(param, "kp")== 0) params.kp = atof(value);

  /* parameters een nieuwe waarde geven via het set commando doe je hier ... */
  
  EEPROM_writeAnything(0, params);
  EEPROM.commit();
}

void onDebug()
{
  SerialPort.print("cycle time: ");
  SerialPort.println(params.cycleTime);
  SerialBT.print("cycle time: ");
  SerialBT.println(params.cycleTime);
  
  /* parameters weergeven met behulp van het debug commando doe je hier ... */
  SerialPort.print("black: ");
  SerialBT.print("black: ");
  for (int i = 0; i < 6; i++)
  {
    SerialPort.print(params.black[i]);
    SerialPort.print(" ");
    SerialBT.print(params.black[i]);
    SerialBT.print(" ");
  }
  SerialPort.println(" ");
  SerialBT.println(" ");

  SerialPort.print("white: ");
  SerialBT.print("white: ");
  for (int i = 0; i < 6; i++)
  {
    SerialPort.print(params.white[i]);
    SerialPort.print(" ");
    SerialBT.print(params.white[i]);
    SerialBT.print(" ");
  }
  SerialPort.println(" ");
  SerialBT.println(" ");

  SerialPort.print("normalised: ");
  SerialBT.print("normalised: ");
  for (int i = 0; i < 6; i++)
  {
    SerialPort.print(normalised[i]);
    SerialPort.print(" ");
    SerialBT.print(normalised[i]);
    SerialBT.print(" ");
  }
  SerialPort.println(" ");
  SerialBT.println(" ");

  SerialPort.print("position: ");
  SerialPort.print(debugPosition);
  SerialBT.print("position: ");
  SerialBT.print(debugPosition);

  SerialPort.print("power: ");
  SerialPort.println(params.power);
  SerialBT.print("power: ");
  SerialBT.println(params.power);

  SerialPort.print("diff: ");
  SerialPort.println(params.diff);
  SerialBT.print("diff: ");
  SerialBT.println(params.diff);

  SerialPort.print("kp: ");
  SerialPort.println(params.kp);
  SerialBT.print("kp: ");
  SerialBT.println(params.kp);

  float cycleTimeInSec = ((float) params.cycleTime) / 1000000;
  float ki = params.ki / cycleTimeInSec;
  SerialPort.print("Ki: ");
  SerialPort.println(ki);
  SerialBT.print("ki: ");
  SerialBT.println(ki);

  float kd = params.kd * cycleTimeInSec;
  SerialPort.print("Kd: ");
  SerialPort.println(kd);
  SerialBT.print("kd: ");
  SerialBT.println(kd);

  SerialPort.print("calculation time: ");
  SerialPort.println(calculationTime);
  SerialBT.print("calculation time: ");
  SerialBT.println(calculationTime);

  calculationTime = 0;
}

//kalibratie sensor array
void onCalibrate()
{
  char* param = sCmd.next();

  if (strcmp(param, "black") == 0)
  {
    SerialPort.print("start calibrating black... ");
    SerialBT.print("start calibrating black... ");
    for (int i = 0; i < 6; i++) params.black[i]=analogRead(sensor[i]);
    SerialPort.println("done");
    SerialBT.println("done");
  }
  else if (strcmp(param, "white") == 0)
  {
    SerialPort.print("start calibrating white... ");
    SerialBT.print("start calibrating white... ");    
    for (int i = 0; i < 6; i++) params.white[i]=analogRead(sensor[i]);  
    SerialPort.println("done");
    SerialBT.println("done");      
  }

  EEPROM_writeAnything(0, params);
  EEPROM.commit();
}

void onRun()
{
    run = true;

}

void onStop()
{
  run = false;
}