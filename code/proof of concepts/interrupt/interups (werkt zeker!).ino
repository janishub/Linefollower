#define LED_PIN 16

bool status = false;

void setup() 
{
  pinMode(LED_PIN, OUTPUT);
  attachInterrupt(4,STARTSTOP,RISING);
}

void loop() 
{
    digitalWrite (LED_PIN,status);
}

void STARTSTOP()
{
  status =! status;
}