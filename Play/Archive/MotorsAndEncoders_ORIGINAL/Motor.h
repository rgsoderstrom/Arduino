
//
// Motor.h
//

#ifndef MOTOR_H
#define MOTOR_H

#undef DOUBLE

#include "StepperMotorSpeedProfile.h"
#include "StepperMotor.h"
#include "StepperMotorTask.h"

class Motor
{
  public:
    Motor ();
    bool Setup ();
    void Loop (float currTime);

    void Start (float currTime);
    void Stop ();

    bool Busy () {return task != NULL;}
    
  private:
    // interface to Adafruit drivers
    StepperMotorTask *task = NULL;

    // describes motor hardware and its connection
    StepperMotor motor;
    
    // what we want it to do
    StepperMotorSpeedProfile speedProfile;
    
    
  
};

#endif
