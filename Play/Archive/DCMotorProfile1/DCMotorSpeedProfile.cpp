
//
// DCMotorSpeedProfile.h
//

#include "DCMotorSPeedProfile.h"

DCMotorSpeedProfile::DCMotorSpeedProfile () 
{
    putIndex = 0;    // used to create profile, index of next entry to write speedProfile[]
    getIndex = 0;    // used to read profile, index of current segment in speedProfile[]
    
    unsigned int stepCounter = 0; // count interrupts within a segment

    float lastReadbackValue = 0; // last value sent to PWM. Add delta to get next value
}


//********************************************************************************
//
// Ramp - add a speed change to profile
//
void DCMotorSpeedProfile::Ramp (float seconds,              // time in seconds to make the change
                                float normalizedFinalLevel) // 0..1
{
    if (normalizedFinalLevel < 0 || normalizedFinalLevel > 1)
        return;

    if (putIndex >= MaxSpeedProfileSegment)
        return;
        
    unsigned int numberSteps = (unsigned long) (seconds * GetInterruptsPerSecond () + 0.5);

    float final   = (int) (normalizedFinalLevel * MaxPWM + 0.5);
    float current = putIndex == 0 ? 0 : speedProfile [putIndex - 1].final;

    float delta = (final - current) / numberSteps;

    speedProfile [putIndex].delta = delta;
    speedProfile [putIndex].final = final;
    speedProfile [putIndex].steps = numberSteps;
    putIndex++;
}

//********************************************************************************
//
// Hold - add a constant speed section to profile
//
void DCMotorSpeedProfile::Hold (float seconds)
{
    if (putIndex >= MaxSpeedProfileSegment)
        return;
        
    unsigned int numberSteps = (int) (seconds * GetInterruptsPerSecond () + 0.5);
    float current = putIndex == 0 ? 0 : speedProfile [putIndex - 1].final;

    speedProfile [putIndex].delta = 0;
    speedProfile [putIndex].final = current;
    speedProfile [putIndex].steps = numberSteps;
    putIndex++;
}

//********************************************************************************
//
// ResetReadback () - called prior to beginning a read-back
//
void DCMotorSpeedProfile::ResetReadback ()
{
    getIndex = 0;
    stepCounter = 0;
    lastReadbackValue = 0;
}

//********************************************************************************
//
// GetNext - get next value to be sent to PWM. returns true if this is last value
//
bool DCMotorSpeedProfile::GetNext (unsigned int& val)
{
    if (getIndex >= putIndex)
        return true;

    lastReadbackValue += speedProfile [getIndex].delta;
    val = (unsigned int) (lastReadbackValue + 0.5);

    if (++stepCounter == speedProfile [getIndex].steps)
    {
        getIndex++;
        stepCounter = 0;
    }

    return getIndex == putIndex;
}

