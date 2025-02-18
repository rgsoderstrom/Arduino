//*******************************************************************************
//
// DCMotorTask.h
//

#ifndef DCMOTORTASK_H
#define DCMOTORTASK_H

#include "Arduino.h"

//
// explains how to include files/libraries:
//       https://www.arduino.cc/en/Hacking/BuildProcess


#include <Wire.h>
#include <Adafruit_MotorShield.h>

#include "DCMotorSpeedProfile.h"

enum MotorSelect {MotorSelect_M1, MotorSelect_M2, MotorSelect_M3, MotorSelect_M4};
enum Direction {MotorDir_Forward, MotorDir_Reverse, MotorDir_Off};

class DCMotorTask
{
    public:
        DCMotorTask (MotorSelect          select,
                     Direction            dir,
                     DCMotorSpeedProfile *profile);

        void Start ();
        bool PeriodicTask ();
        
        static Adafruit_MotorShield *AFMS;

    private:
        char in1Pin;
        char in2Pin;
        char pwmPin;

        Direction direction;
        
        DCMotorSpeedProfile *speedProfile;


};

#endif

