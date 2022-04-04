
//
// MotorBoard.cpp
//

#include "MotorBoard.h"

MotorBoard::MotorBoard ()
{
    directions [0] = directions [1] = 0;
    ConfigureReadOnly ();
}

void MotorBoard::ConfigurePins (ControlOptions option)
{
    switch (option)
    {
        case ReadOnly:
            ConfigureReadOnly ();
            break;
            
        case EnableOnly:
            ConfigureEnableOnly ();
            break;

        case EnableAndDirection:  // not yet supported
            break;
                  
        case FullControl:  // not yet supported
            break;
    };
}

void MotorBoard::ConfigureReadOnly ()
{
    pinMode (EnableMotors_Pin, INPUT);
    pinMode (M1Dir_Pin,        INPUT);
    pinMode (M2Dir_Pin,        INPUT);
    pinMode (M1PWM_Pin,        INPUT);
    pinMode (M2PWM_Pin,        INPUT);
    pinMode (SF_Pin,           INPUT);
    currentConfiguration = ReadOnly;
}
    
void MotorBoard::ConfigureEnableOnly ()
{
    pinMode (EnableMotors_Pin, OUTPUT);
    digitalWrite (EnableMotors_Pin, 0);
    
    pinMode (M1Dir_Pin,        INPUT);
    pinMode (M2Dir_Pin,        INPUT);
    pinMode (M1PWM_Pin,        INPUT);
    pinMode (M2PWM_Pin,        INPUT);
    pinMode (SF_Pin,           INPUT);
    currentConfiguration = EnableOnly;
}
    
void MotorBoard::EnableMotors ()
{
    switch (currentConfiguration)
    {
        case ReadOnly:
            break;
            
        case EnableOnly:
        case EnableAndDirection:
        case FullControl:
            digitalWrite (EnableMotors_Pin, 1);  
            break;
    }
}
  
void MotorBoard::DisableMotors ()
{
    switch (currentConfiguration)
    {
        case ReadOnly:
            break;
            
        case EnableOnly:
        case EnableAndDirection:
        case FullControl:
            digitalWrite (EnableMotors_Pin, 0);  
            break;
    }
}
    
void MotorBoard::SetDirection (int select, int dir)
{
    switch (currentConfiguration)
    {
        case ReadOnly:
        case EnableOnly:
            break;
            
        case EnableAndDirection:
        case FullControl:
            directions [select] = dir;
            if (select == 0) digitalWrite (M1Dir_Pin, dir);
            if (select == 1) digitalWrite (M2Dir_Pin, dir);  
            break;
    }
}

int MotorBoard::GetDirection (int select)
{
    return directions [select];
}
