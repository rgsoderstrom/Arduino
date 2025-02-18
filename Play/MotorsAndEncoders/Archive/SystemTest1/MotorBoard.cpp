
//
// MotorBoard.cpp
//

#include "MotorBoard.h"

MotorBoard::MotorBoard ()
{
    directions [0] = directions [1] = 0;
}

void MotorBoard::ConfigurePins ()
{
  // motor driver board pins
    pinMode (EnableMotors_Pin, OUTPUT);
    pinMode (M1Dir_Pin,        OUTPUT);
    pinMode (M2Dir_Pin,        OUTPUT);
    pinMode (M1PWM_Pin,        INPUT);
    pinMode (M2PWM_Pin,        INPUT);
    pinMode (SF_Pin,           INPUT);
    
    digitalWrite (EnableMotors_Pin, 0);
    digitalWrite (M1Dir_Pin,        1);
    digitalWrite (M2Dir_Pin,        1);  
}
    
void MotorBoard::EnableMotors ()
{
    digitalWrite (EnableMotors_Pin, 1);  
}
  
void MotorBoard::DisableMotors ()
{
    digitalWrite (EnableMotors_Pin, 0);  
}
    
void MotorBoard::SetDirection (int select, int dir)
{
  directions [select] = dir;
  
  if (select == 0) digitalWrite (M1Dir_Pin, dir);
  if (select == 1) digitalWrite (M2Dir_Pin, dir);  
}

int MotorBoard::GetDirection (int select)
{
    return directions [select];
}
