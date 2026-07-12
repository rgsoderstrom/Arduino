
// Ignition2 driver

#include "JobQueue.h"
#include "IgnitionDisplay.h"

JobQueue         OneTimeJobs;
PeriodicJobQueue PeriodicJobs;

// digital pin numbers
const int PointsPin  = 3; // goes low when points close  
const int TriggerPin = 6; // driven high to energize coil 
const int ScopePin = 5;   // test point, pulses when points close

IgnitionDisplay display;

//*******************************************************************

bool          IgnoreInt = true;
volatile bool IntFlag = false;

void setup() 
{
    pinMode (PointsPin,  INPUT); 
    pinMode (TriggerPin, OUTPUT);
    pinMode (ScopePin,   OUTPUT);
  
    digitalWrite (TriggerPin, HIGH);
    digitalWrite (ScopePin,   LOW);
    attachInterrupt (digitalPinToInterrupt (PointsPin), myISR, FALLING);

    display.Init ();
    PeriodicJobs.Add (CheckButtons, (void *) 0, "Buttons", 50);
    EnableInt ("");
}

void loop() 
{
    unsigned long now = millis ();
    PeriodicJobs.RunJobs (now);
    OneTimeJobs.RunJobs (now);

    if (IntFlag == true)
    {
        IntFlag = false;

        if (IgnoreInt == false)
        {
            digitalWrite (ScopePin, HIGH);

            IgnoreInt = true;
            OneTimeJobs.Add (EnergizeCoil, "Energize", display.Delay1 ());  
            OneTimeJobs.Add (EnableInt,    "Enable",   display.Delay2 ());  
            digitalWrite (ScopePin, LOW);
        }
    }
}

void EnergizeCoil (char *msg)
{
    for (int i=0; i<display.Sparks (); i++)
    {
        digitalWrite (TriggerPin, LOW); 
        delay        (display.Dwell ());
        digitalWrite (TriggerPin, HIGH);
        delay        (2);
    }
}

void EnableInt (char *msg)
{
    IgnoreInt = false;
}

void myISR ()
{
    IntFlag = true;
}

//*******************************************************************

int wasButtons = 0;

void CheckButtons ()
{    
    uint8_t buttons = display.ButtonState ();

    if (buttons == wasButtons)
        return;

    wasButtons = buttons;

    if (buttons & BUTTON_UP) 
        display.IncrDataItem ();
    
    if (buttons & BUTTON_DOWN)
        display.DecrDataItem ();

    if (buttons & BUTTON_LEFT) 
        display.PrevMenuItem ();

    if (buttons & BUTTON_RIGHT) 
        display.NextMenuItem ();
}
