
//
// DCMotorSpeedProfile.h
//

#ifndef DCMOTORSPEEDPROFILE_H
#define DCMOTORSPEEDPROFILE_H

#include "MotorProfile.h"

#define MaxSpeedProfileSegment 5

//*****************************************************************************************
//
// SpeedProfileSegment
//
struct SpeedProfileSegment
{
  public:
    float        delta;  // change each step, 0..4096. used when reading profile back
    float        final;  // final value after all steps complete, 0..4096. used when building profile
    unsigned int steps;  // number of interrupts this profile is active
};

class DCMotorSpeedProfile : public MotorProfile
{
  public:
    DCMotorSpeedProfile ();

    void Ramp (float seconds, float normalizedFinalLevel);
    void Hold (float seconds);
    void ResetReadback ();
    bool GetNext (unsigned int& val);

  private:
    SpeedProfileSegment speedProfile [MaxSpeedProfileSegment];
    int                 putIndex; // used to create profile, index of next entry to write speedProfile[]
    int                 getIndex; // used to read-back profile, index of current segment in speedProfile[]
    
    unsigned long stepCounter;       // count interrupts within a segment
    float         lastReadbackValue; // last value sent to PWM. Add delta to get next value

};

#endif

