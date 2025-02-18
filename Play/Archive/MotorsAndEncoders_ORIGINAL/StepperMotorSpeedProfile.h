
// StepperMotorSpeedProfile.h

#ifndef STEPPERMOTORSPEEDPROFILE_H
#define STEPPERMOTORSPEEDPROFILE_H

#include "StepperMotor.h"

#define MaxSpeedProfileSegment 5

enum MotorSelect    {MotorSelect_M1, MotorSelect_M2};
enum MotorDirection {MotorDir_Forward, MotorDir_Reverse, MotorDir_Off};

//*****************************************************************************************
//
// SpeedProfileSegment
//
struct StepperSpeedProfileSegment
{
  public:
      // specified by caller
        float  startTime; // seconds
        float  duration;  // seconds

        float  finalRPM;
        float  initialRPM; // final RPM of previous segment

      // calculated
        short initialPPS; // final PPS of previous segment
        short finalPPS;   // calculated from caller specified final RPM

        float C1;  // see Visio drawing or Matlab stepper.m
        float C2;

        unsigned short numberSteps; // calculated number of steps this segment
};

class StepperMotorSpeedProfile
{
  public:
    StepperMotorSpeedProfile ();
    bool Initialize (unsigned short stepsPerRev, MotorDirection dir);
    
    void StartAt (float startTime);
    void Ramp (float seconds, float finalRPM);
    void Hold (float seconds);
    void ResetReadback ();
    bool GetNext (float& nextTime);

    unsigned short GetStepsPerRevolution () {return stepsPerRevolution;}
    MotorDirection GetDirection () {return direction;}
    
    void Print ();
    void Clear () {putIndex = 0;}
    
  private:
    StepperSpeedProfileSegment speedProfile [MaxSpeedProfileSegment];
    int                        putIndex; // used to create profile, index of next entry to write speedProfile[]
    int                        getIndex; // used to read-back profile, index of current segment in speedProfile[]

    unsigned short stepsSent;   // number of steps that have been sent for current profile segment
    float          profileStartTime;

    unsigned short stepsPerRevolution;
    MotorDirection direction;
};

#endif
