
//************************************************************************
//
// DCMotorTask.cpp
//

#include "Arduino.h"
#include "Constants.h"
#include "DCMotorTask.h"

Adafruit_MotorShield *DCMotorTask::AFMS;

//********************************************************************************
//
// Constructor
//
DCMotorTask::DCMotorTask (MotorSelect          select,   // which motor, M0..M3
                          Direction            dir,      // forward or reverse
                          DCMotorSpeedProfile *profile)  // speed as functiom of time
{
    direction = dir;

    switch (select)
    {
        case MotorSelect_M1:
            pwmPin = 8; in2Pin = 9; in1Pin = 10;
            break;

        case MotorSelect_M2:
            pwmPin = 13; in2Pin = 12; in1Pin = 11;
            break;

        case MotorSelect_M3:
            pwmPin = 2; in2Pin = 3; in1Pin = 4;
            break;

        case MotorSelect_M4:
            pwmPin = 7; in2Pin = 6; in1Pin = 5;
            break;

        default:
            pwmPin = in2Pin = in1Pin = 0;
            speedProfile = NULL;
            return;
    }

    speedProfile = profile;
}

//********************************************************************************
//
// Start () - set direction switches
//
void DCMotorTask::Start ()
{
    if (direction == MotorDir_Forward)
    {
        AFMS->setPin (in2Pin, LOW); 
        AFMS->setPin (in1Pin, HIGH);
    }

    if (direction == MotorDir_Reverse)
    {
        AFMS->setPin (in1Pin, LOW);
        AFMS->setPin (in2Pin, HIGH);
    }
}

//********************************************************************************
//
// Periodic Task - returns true when done
//
bool DCMotorTask::PeriodicTask ()
{
    unsigned int pwmValue = 0;

    bool done = speedProfile->GetNext (pwmValue);

    AFMS->setPWM (pwmPin, pwmValue);

    return done;
}

