
// StepperMotor.h

#ifndef STEPPERMOTOR_H
#define STEPPERMOTOR_H

enum ShieldConnector {Stepper_M1 = 1, Stepper_M2};

class StepperMotor
{
  public:    
    ShieldConnector Connector;
    unsigned short  StepsPerRevolution;
};

#endif
