
// TIM6_Ignition driver

int PointsPin  = 3;  // digital pin number
int Logic0     = 12; // ditto
int TriggerPin = 6;  // ditto 

void setup() 
{
  Serial.begin (9600);
  Serial.println ("TIM6");
 
  pinMode (PointsPin,  INPUT_PULLUP); // --- external pull-ups added
  pinMode (Logic0,     OUTPUT);
  pinMode (TriggerPin, OUTPUT);
  
  attachInterrupt (digitalPinToInterrupt (PointsPin), myISR, FALLING);

  digitalWrite (TriggerPin, HIGH);
  digitalWrite (Logic0,     LOW);
}

volatile unsigned int counter = 0;
unsigned int wasCounter = 0xff;

void loop() 
{
  delay (50);

  if (wasCounter != counter)
  {
    Serial.println (counter);
    wasCounter = counter;
  }

//  digitalWrite (TriggerPin, LOW);
//  delay (10);


  // delay (10);
  // Serial.print (++counter); Serial.print (" "); Serial.println (state);

}

void myISR ()
{
   counter++;
}

