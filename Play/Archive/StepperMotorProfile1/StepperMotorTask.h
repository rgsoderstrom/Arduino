
// StepperMotorTask.h

#ifndef STEPERMOTORTASK_H
#define STEPERMOTORTASK_H

#include "Adafruit_MotorShield.h"
#include "StepperMotor.h"
#include "StepperMotorSpeedProfile.h"

class StepperMotorTask
{
  public:
    static Adafruit_MotorShield *AFMS;

    StepperMotorTask (StepperMotor&             motor,
                      StepperMotorSpeedProfile *profile);

    void StartAt (float t);

    bool PeriodicTask (float timeNowSeconds);

  private:
    Adafruit_StepperMotor *motorInterface;

    StepperMotorSpeedProfile *speedProfile;

    float timeOfNextStep; // seconds
};
  
#endif
