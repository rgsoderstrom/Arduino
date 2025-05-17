
// InterruptDemo

const int PhaseA = 2;
const int PhaseB = 4;
const int PressurePin = 0;
const int interruptPin = PhaseA;

volatile int interruptCounter = 0;
//volatile int WasAB = 0;

void setup() 
{
  Serial.begin (9600);
  Serial.println ("Starting");
  pinMode (PhaseA, INPUT);
  pinMode (PhaseB, INPUT);
  attachInterrupt (digitalPinToInterrupt (interruptPin), IntHandler, CHANGE); // RISING);
  interruptCounter = 0;

  //int a = digitalRead (PhaseA) & 1;
  //int b = digitalRead (PhaseB) & 1;

  //WasAB = (a << 1) | b;
}

int wasIntCounter = 0;

void loop() 
{
  if (wasIntCounter != interruptCounter)
  {
    wasIntCounter = interruptCounter;

    int unsignedCount;

    char sign = ' ';

    if (interruptCounter >= 0)
    {
        unsignedCount = interruptCounter;
    }
    else
    {
        sign = '-';
        unsignedCount = interruptCounter * -1;
    }

    Serial.print (sign);
    Serial.print (unsignedCount >> 2);

    switch (unsignedCount & 3)
    {
        case 0: Serial.println (".00"); break;
        case 1: Serial.println (".25"); break;
        case 2: Serial.println (".50"); break;
        case 3: Serial.println (".75"); break;
    }
  }
}

void IntHandler ()
{
  int a = digitalRead (PhaseA) & 1;
  int b = digitalRead (PhaseB) & 1;

  if (a == b) interruptCounter++;
  else        interruptCounter--;
}


