
// InterruptDemo

const int PhaseA = 2;
const int PhaseB = 4;
const int PressurePin = 0;
const int interruptPin = PhaseA;

volatile int interruptCounter = 0;
//int loopCounter = 0;

void setup() 
{
  Serial.begin (9600);
  Serial.println ("Starting");
  pinMode (PhaseA, INPUT);
  pinMode (PhaseB, INPUT);
  attachInterrupt (digitalPinToInterrupt (interruptPin), IntHandler, RISING);
  interruptCounter = 0;
}

int wasIntCounter = 0;

void loop() 
{
  if (wasIntCounter != interruptCounter)
  {
      wasIntCounter = interruptCounter;

      int  degrees = interruptCounter >> 1;
      bool half    = interruptCounter & 1;

      if (half) 
      {
          Serial.print   (degrees);
          Serial.println (".5");
      }
      else
          Serial.println (degrees);
  }
}

void IntHandler ()
{
  int b = digitalRead (PhaseB) & 1;

  if (b == 0) interruptCounter++;
  else        interruptCounter--;
}
