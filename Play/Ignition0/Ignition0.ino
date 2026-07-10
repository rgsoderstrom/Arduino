
/*
  Ignition0 - Pulse trigger pin at steady rate
*/

const int TriggerPin = 6; 
const int DwellMillis = 2;
const int PeriodMillis = 200;
const int SparksPerCycle = 3;

// the setup function runs once when you press reset or power the board
void setup() 
{
  pinMode      (TriggerPin, OUTPUT);
  digitalWrite (TriggerPin, HIGH);
  Serial.begin (9600);
  Serial.println ("Ignition0 - free running"); 
}

// the loop function runs over and over again forever
void loop() 
{
  for (int i=0; i<SparksPerCycle; i++)
  {
    digitalWrite (TriggerPin, LOW); 
    delay        (DwellMillis);
    digitalWrite (TriggerPin, HIGH);
    delay        (2);
  }

  delay (PeriodMillis - SparksPerCycle * DwellMillis);                 
}
