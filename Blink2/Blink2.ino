
/*
  Blink
*/

// Pulse TIM-6 trigger pin at steady rate

const int TriggerPin = 6; 

// the setup function runs once when you press reset or power the board
void setup() 
{
  pinMode (TriggerPin, OUTPUT);
  digitalWrite (TriggerPin, HIGH);
  Serial.begin (9600);
}

// the loop function runs over and over again forever
void loop() 
{
  digitalWrite (TriggerPin, LOW); 
  delay(2);
  digitalWrite (TriggerPin, HIGH);
//  delay(5);
//  digitalWrite (TriggerPin, LOW); 
//  delay(2);
//  digitalWrite (TriggerPin, HIGH);


  delay(200);                 

}
