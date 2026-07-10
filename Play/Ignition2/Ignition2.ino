
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

// #define DelayIndex 0
// #define DwellIndex 1

// #define initialStartDelay 1 // milliseconds
// #define initialDwellTime  2

// unsigned int times [2] = {initialStartDelay, initialDwellTime};
// unsigned int select = DelayIndex;

// void WriteLCD ()
// {
//     char obuf [10];

//     lcd.setCursor (0, 0);
//     lcd.print (select == DelayIndex ? "*" : " ");
//     lcd.setCursor (3, 0);
//     sprintf (obuf, "%2d", times [DelayIndex]);
//     obuf [2] = 0;
//     lcd.print (obuf);

//     lcd.setCursor (0, 1);
//     lcd.print (select == DwellIndex ? "*" : " ");
//     lcd.setCursor (3, 1);
//     sprintf (obuf, "%2d ", times [DwellIndex]);
//     obuf [2] = 0;
//     lcd.print (obuf);
// }

// unsigned long LastButtonMillis = 0;

// void CheckButtons ()
// {
//     unsigned long now = millis ();

//     if (LastButtonMillis > now - 500)
//         return;

//     LastButtonMillis = now;

//     uint8_t buttons = lcd.readButtons();

//     if (buttons == 0)
//         return;

    // if ((buttons & BUTTON_UP) || (buttons & BUTTON_DOWN))
    //     select ^= 1;

//     if (buttons & BUTTON_LEFT) 
//     {
//         if (times [select] > 0)
//             --times [select];
//     }

//     if (buttons & BUTTON_RIGHT) 
//     {
//         ++times [select];
//     }

//     WriteLCD ();
// }

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
