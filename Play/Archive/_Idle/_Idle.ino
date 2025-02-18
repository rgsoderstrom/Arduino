
// Pin9 is PWM for motor 1

void setup() 
{
  Serial.begin (9600);

  for (int i=0; i<=19; i++)
    pinMode (i, INPUT);

  int EnablePin = 4; // motor shield enable
  
  pinMode (EnablePin, OUTPUT); 
  digitalWrite (EnablePin, 1);
}

void loop() 
{
//  delay (1000);
//  Serial.println ("A");
}
