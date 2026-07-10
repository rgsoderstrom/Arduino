
/*
  Ignition1 
      - SparksPerCycle output pulses for one falling input transition
*/

#include "JobQueue.h"

JobQueue OneTimeJobs;

const int PointsPin  = 3;  // input digital pin number

const int TIM6Pin = 6;   
const int ScopePin = 5;  

const int DwellMillis = 2;
const int Delay1 = 10;
const int Delay2 = 1000;
const int SparksPerCycle = 3;

volatile bool IgnoreInt = false;
volatile bool IntFlag = false;

void setup() 
{
    pinMode (PointsPin, INPUT); 
    pinMode (TIM6Pin,   OUTPUT);
  
    digitalWrite (TIM6Pin, HIGH);
    digitalWrite (ScopePin, LOW);
    attachInterrupt (digitalPinToInterrupt (PointsPin), myISR, FALLING);
    EnableInt ("");
}

void loop() 
{
    OneTimeJobs.RunJobs (millis ());

    if (IntFlag == true)
    {
        IntFlag = false;

        if (IgnoreInt == false)
        {
            digitalWrite (ScopePin, HIGH);

            IgnoreInt = true;
            OneTimeJobs.Add (EnergizeCoil, "Energize", Delay1);  
            OneTimeJobs.Add (EnableInt,    "Enable",   Delay2);  
            digitalWrite (ScopePin, LOW);
        }
    }
}

void EnergizeCoil (char *msg)
{
    for (int i=0; i<SparksPerCycle; i++)
    {
        digitalWrite (TIM6Pin, LOW); 
        delay        (DwellMillis);
        digitalWrite (TIM6Pin, HIGH);
        delay        (2);
    }
}

void EnableInt (char *msg)
{
    IgnoreInt = false;
}

void myISR ()
{
    IntFlag = true;
}




