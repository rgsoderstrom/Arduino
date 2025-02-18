
// StepperMotorSpeedProfile.cpp

#include "Arduino.h"
#include "Utils.h"
#include "StepperMotorSpeedProfile.h"

static float MinutesPerSecond = (float) (1 / 60.0);

//********************************************************************************
//
// Constructor
//
StepperMotorSpeedProfile::StepperMotorSpeedProfile ()
{

}

//********************************************************************************
//
// Initialize
//
bool StepperMotorSpeedProfile::Initialize (unsigned short stepsPerRev, MotorDirection dir)
{
    ResetReadback ();
    stepsPerRevolution = stepsPerRev;
    direction = dir;
    StartAt (0);
    
    return true;
}
    
//********************************************************************************
//
// StartAt ()
//
void StepperMotorSpeedProfile::StartAt (float time)
{
    profileStartTime = time;   
}

//********************************************************************************

void StepperMotorSpeedProfile::Print ()
{
    char obuf [100];
    char nbuf [100];
    
    sprintf (obuf, "SpeedProfile has %d segments", putIndex); Serial.println (obuf);

    for (int i=0; i<putIndex; i++)
    {
        StepperSpeedProfileSegment& segment = speedProfile [i];

        Serial.print ("Segment: "); Serial.println (i);
        
        sprintf (obuf, "StartTime %s", Utils::FloatToString (segment.startTime, nbuf)); Serial.println (obuf);        
        sprintf (obuf, "Duration %s",  Utils::FloatToString (segment.duration,  nbuf)); Serial.println (obuf);

        sprintf (obuf, "initial RPM %s", Utils::FloatToString (segment.initialRPM, nbuf)); Serial.println (obuf);
        sprintf (obuf, "final RPM %s",   Utils::FloatToString (segment.finalRPM,   nbuf)); Serial.println (obuf);

        /**
        
        sprintf (obuf, "initial PPS %d", segment.initialPPS); Serial.println (obuf);
        sprintf (obuf, "final PPS %d",   segment.finalPPS);   Serial.println (obuf);
        
        sprintf (obuf, "C1 %s", Utils::FloatToString (segment.C1); Serial.println (obuf);
        sprintf (obuf, "C2 %s", Utils::FloatToString (segment.C2); Serial.println (obuf);
        
        sprintf (obuf, "number steps %d", segment.numberSteps); Serial.println (obuf);      
        **/
        Serial.println (" ");
    }
}

//********************************************************************************
//
// Ramp - add a speed change to profile
//
void StepperMotorSpeedProfile::Ramp (float seconds, float finalRPM)
{    
    if (putIndex >= MaxSpeedProfileSegment)
        return;
        
    StepperSpeedProfileSegment& segment = speedProfile [putIndex]; // note this is a reference

    if (putIndex == 0)    
    {
        segment.initialRPM = 0;
        segment.initialPPS = 0;
        segment.startTime  = 0;
    }
    else
    {    
        StepperSpeedProfileSegment& previous = speedProfile [putIndex - 1];

        segment.initialRPM = previous.finalRPM;
        segment.initialPPS = previous.finalPPS;
        segment.startTime  = previous.startTime + previous.duration;
    }

    segment.finalRPM = finalRPM;
    segment.finalPPS = segment.finalRPM  * stepsPerRevolution * MinutesPerSecond;
    segment.duration = seconds;

    segment.numberSteps = min (segment.initialPPS, segment.finalPPS) * segment.duration + 0.5 * segment.duration * fabs (segment.finalPPS - segment.initialPPS);

    segment.C1 = segment.initialPPS;
    segment.C2 = 0.5f * (segment.finalPPS - segment.initialPPS) / segment.duration;

    putIndex++;
}

//********************************************************************************
//
// Hold - add a constant speed section to profile
//
void StepperMotorSpeedProfile::Hold (float seconds)
{
    if  (putIndex == 0)
        Ramp (seconds, 0);
    else
        Ramp (seconds, speedProfile [putIndex - 1].finalRPM);
}

//********************************************************************************
//
// ResetReadback () - called prior to beginning a read-back
//
void StepperMotorSpeedProfile::ResetReadback ()
{
    getIndex = 0;
    stepsSent = 0;
}

//********************************************************************************
//
// GetNext - get time of next pulse. returns true if this is last value
//
bool StepperMotorSpeedProfile::GetNext (float& nextTime)
{
    StepperSpeedProfileSegment& segment = speedProfile [getIndex];

    stepsSent++; // calculate the time of the next step

  // if in a speed change section
    if (fabs (segment.C2) > 1e-4) // arbitrary small threshold
    {
      nextTime = (float) (-segment.C1 + sqrt (segment.C1 * segment.C1 + 4 * segment.C2 * stepsSent)) / (2 * segment.C2);
    }
    else // constant speed
    {
        nextTime = stepsSent / segment.C1;
    }

    nextTime += segment.startTime;
    nextTime += profileStartTime;

  // see if we're at the end of a segment. if so, go to next if any remaining
    if (stepsSent == speedProfile [getIndex].numberSteps)
    {
        getIndex++;
        stepsSent = 0;

        Serial.println ("Segment done");
    }

    return getIndex == putIndex;
}
