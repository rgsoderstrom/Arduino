
// StepperMotorTask.cpp

#include "Adafruit_MotorShield.h"
#include "StepperMotorTask.h"

Adafruit_MotorShield *StepperMotorTask::AFMS = NULL;

//*************************************************************************
//
//
//
StepperMotorTask::StepperMotorTask (StepperMotor&             motor,
                                    StepperMotorSpeedProfile *profile)
 : speedProfile (profile)
{
    motorInterface = AFMS->getStepper (profile->GetStepsPerRevolution (), (byte) motor.Connector);
    speedProfile = profile;
}

//********************************************************************************
//
// 
//
void StepperMotorTask::StartAt (float t) 
{
    speedProfile->StartAt (t);
    speedProfile->GetNext (timeOfNextStep);
}

//********************************************************************************
//
// 
//

float lastPrintAt = 0;

bool StepperMotorTask::PeriodicTask (float timeNowSeconds) 
{
    bool done = false;

    if (timeNowSeconds >= timeOfNextStep)
    {
        motorInterface->onestep ((byte) speedProfile->GetDirection (), DOUBLE);
    //  motorInterface->onestep ((byte) speedProfile->GetDirection (), SINGLE);

        done = speedProfile->GetNext (timeOfNextStep);

        if (done)
            motorInterface->release ();
    }

    return done;
}
