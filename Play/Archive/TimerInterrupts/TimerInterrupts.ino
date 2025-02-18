
// TimerInterrupts - on each timer interuupt, change the time to the next one. Want
//                   to do this in stepper motor driver

#include <TimerOne.h>

const int MaxTimerValue = 10000; // 10000 microseconds = 10 milliseconds
int currentTimerValue = MaxTimerValue;
int timerDelta = 1000;

bool tic = false;

void setup() 
{
   //int q = FOC0B;
   
    pinMode (2, OUTPUT);
    pinMode (3, OUTPUT);

    Timer1.initialize (currentTimerValue); 
    Timer1.attachInterrupt (timerInterrupt);
}

void loop() 
{
    if (tic)
    {
        tic = false;

        if (currentTimerValue > timerDelta)
        {
            currentTimerValue -= timerDelta;
        }
        else
        {
            currentTimerValue = MaxTimerValue;
            digitalWrite (3, HIGH);
            digitalWrite (3, LOW);
        }

        digitalWrite (2, HIGH);
        digitalWrite (2, LOW);
    
        Timer1.initialize (currentTimerValue); 
    }

    delay (1); // millisecond
}

void timerInterrupt ()
{
    tic = true;
}
