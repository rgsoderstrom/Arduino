
//
// MotorBoard.h
//
//    - messages to communicate with 
//

#ifndef MOTORBOARD_H
#define MOTORBOARD_H

#include <arduino.h>

class MotorBoard
{
  public:
    enum ControlOptions {ReadOnly = 1,       // default, all pins set to input
                         EnableOnly,         // Arduino only enables motors, FPGA controls speed and directon
                         EnableAndDirection, // Arduino can enable and set direction but not drive PWMs
                         FullControl};       // Arduino has full control

    MotorBoard ();
    void ConfigurePins (ControlOptions opt); 
    
    void EnableMotors ();
    void DisableMotors ();
    
    void SetDirection (int select,  // 0 or 1
                       int dir);    // 0 or 1

    int  GetDirection (int select);  // 0 or 1

  private:
    ControlOptions currentConfiguration;
    
    void ConfigureReadOnly ();
    void ConfigureEnableOnly ();
    
    int directions [2];

  //
  // Arduino pins connected to motor board
  //
    static const int EnableMotors_Pin = 4;  // enable both motors when high
    static const int M1Dir_Pin        = 7;
    static const int M2Dir_Pin        = 8;
    static const int M1PWM_Pin        = 9;  // driven by FPGA
    static const int M2PWM_Pin        = 10; //   "    "   "
    static const int SF_Pin           = 12; // status flag
    static const int M1Current_Pin    = A0;
    static const int M2Current_Pin    = A1;
  
};

#endif
