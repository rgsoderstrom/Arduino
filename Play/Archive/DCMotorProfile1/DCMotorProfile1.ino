
//
// explains how to include files/libraries:
//       https://www.arduino.cc/en/Hacking/BuildProcess

#include "Arduino.h"

#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <TimerOne.h>

#include "DCMotorTask.h"
#include "DCMotorSpeedProfile.h"

Adafruit_MotorShield *AFMS;
DCMotorSpeedProfile  *speedProfile;
DCMotorTask          *task;

unsigned int counter = 0;

void setup() 
{
    Serial.begin(9600);           // set up Serial library at 9600 bps
    
    AFMS = new Adafruit_MotorShield ();
    AFMS->begin ();

    DCMotorTask::AFMS = AFMS;  // static

    speedProfile = new DCMotorSpeedProfile ();

    speedProfile->Ramp (10.0f, 0.95f);
    speedProfile->Hold (3.0f);
    speedProfile->Ramp (5.0f, 0);
    speedProfile->ResetReadback ();

    task = new DCMotorTask (MotorSelect_M1, MotorDir_Reverse, speedProfile);
    task->Start ();

    Timer1.initialize (10000); // 10000 microseconds = 10 milliseconds = 100 per second
    Timer1.attachInterrupt (timerInterrupt);
}

bool cycle = false;

void loop() 
{   
    unsigned int counterCopy = 0;     

    noInterrupts ();
    counterCopy = counter;
    interrupts ();

    if (cycle)
    {
        cycle = false;
        
        if (counterCopy % 100 == 0)
            Serial.println (counterCopy);

        bool done = task->PeriodicTask ();
    
        if (done)
        {
            Serial.println ("done");
            Timer1.detachInterrupt ();    
        }
    }
    
    delay (1);  // milliseconds
}

void timerInterrupt ()
{
    cycle = true;
    counter++;  
}

