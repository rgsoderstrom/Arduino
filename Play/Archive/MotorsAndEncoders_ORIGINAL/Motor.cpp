
//
// Motor.cpp
//

#include "Arduino.h"

#include <Wire.h>
#include <Adafruit_MotorShield.h>

#include "Motor.h"

Motor::Motor ()
{
  
}


bool Motor::Setup ()
{
    StepperMotorTask::AFMS = new Adafruit_MotorShield ();  // static
    StepperMotorTask::AFMS->begin ();

  // motor characteristics
    motor.StepsPerRevolution = 96; // 48; // 200;
    motor.Connector = Stepper_M2;
}

float startTime = 0;

void Motor::Start (float currTime)
{
    startTime = currTime;
    
  // desired speed profile
    speedProfile.Initialize (motor.StepsPerRevolution, MotorDir_Forward);

    speedProfile.Clear ();
    
    speedProfile.Ramp (5, 100);
    speedProfile.Hold (20);
    speedProfile.Ramp (5, 0);

    speedProfile.Print ();


    speedProfile.ResetReadback ();

  // create stepper motor task
    task = new StepperMotorTask (motor, &speedProfile);
    task->StartAt (0);
}

void Motor::Stop ()
{
    delete task; // task dtor will turn off power to motor
    task = NULL;  
}

void Motor::Loop (float currentTime)
{
    if (task != NULL)
    {
        bool done = task->PeriodicTask (currentTime); // timeCopy);
        
        if (done)
        {
            char obuf [100];
            
            task = NULL;
    
            sprintf (obuf, "Done. Task ran for %d seconds", (int) (currentTime - startTime));
            Serial.println (obuf);
        }            
    }
}
