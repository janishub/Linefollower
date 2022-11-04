//ingebouwde led aan en uit doen (blauw);
//doet de led aan en uit, herhaaldelijk;

int LED_BUILTIN = 18;

void setup() 
{
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() 
{
  digitalWrite(LED_BUILTIN, HIGH);   
  delay(1000);                       
  digitalWrite(LED_BUILTIN, LOW);    
  delay(1000);                       
}