
// InterruptDemo

const int PhaseA = 2;
const int PhaseB = 4;
const int interruptPin = PhaseA;

volatile int interruptCounter = 0;
int loopCounter = 0;

void setup() 
{
  Serial.begin (9600);
  pinMode (interruptPin, INPUT_PULLUP);
  pinMode (PhaseB, INPUT_PULLUP);
  attachInterrupt (digitalPinToInterrupt (interruptPin), IntHandler, RISING);
//  attachInterrupt (0, IntHandler, RISING);
}

void loop() 
{
  Serial.print (loopCounter++);
  Serial.print (" ");
  noInterrupts ();
  Serial.println (interruptCounter);
  interrupts ();
  delay (250);
}

void IntHandler ()
{
  int b = digitalRead (PhaseB) & 1;

  if (b == 0) interruptCounter++;
  else        interruptCounter--;
}
