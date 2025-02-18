
//
// StepperMotorProfile1 - speed profile generated at run time, as needed
//

//
// explains how to include files/libraries:
//       https://www.arduino.cc/en/Hacking/BuildProcess

#include "Arduino.h"

#include <Wire.h>
#include <Adafruit_MotorShield.h>

#include "StepperMotor.h"
#include "StepperMotorSpeedProfile.h"
#include "StepperMotorTask.h"

// describes motor hardware and its connection
StepperMotor motor;

// what we want it to do
StepperMotorSpeedProfile speedProfile;

// interface to Adafruit drivers
StepperMotorTask *task = NULL;

int digitalPin = 4;

float interruptPeriod = 0.005f; // seconds
static float currentTime = 0;
static bool  tic = false;

//***********************************************************
//
// put your setup code here, to run once
//
void setup() 
{
    Serial.begin(9600);           // set up Serial library at 9600 bps

    pinMode (digitalPin, OUTPUT);
    
    StepperMotorTask::AFMS = new Adafruit_MotorShield ();  // static
    StepperMotorTask::AFMS->begin ();

  // motor characteristics
    motor.StepsPerRevolution = 48; // 200;
    motor.Connector = Stepper_M2;
    
  // desired speed profile
    speedProfile.Initialize (motor.StepsPerRevolution, MotorDir_Forward);

    speedProfile.Ramp (10, 100);
    speedProfile.Hold (10); 
    speedProfile.Ramp (10, 0);

    speedProfile.ResetReadback ();


  // create stepper motor task
    task = new StepperMotorTask (motor, &speedProfile);
    task->StartAt (currentTime + 5);

  // timer interrupt
    TCB0.CTRLB = TCB_CNTMODE_INT_gc; // Use "Periodic Interrupt" mode

    // input clock rate (from TCA0) is 250kHz
    TCB0.CCMP = (int) (250000 * interruptPeriod);
    TCB0.INTCTRL = TCB_CAPT_bm; // Enable the interrupt
    TCB0.CTRLA = TCB_CLKSEL_CLKTCA_gc | TCB_ENABLE_bm; // Use Timer A as clock, enable timer

//    delay (100);
}

//***********************************************************
//
// 
//
// Timer Interrupt Handler
ISR (TCB0_INT_vect)
{
    currentTime += interruptPeriod; // seconds  
    tic = true;
    
   // Clear interrupt flag
   TCB0.INTFLAGS = TCB_CAPT_bm;
}

//***********************************************************
//
// put your main code here, to run repeatedly
//

int lastTimePrinted = -999;
float timePrintPeriod = 1;//5; // print time every timePrintPerio seconds
 
void loop() 
{
    if (tic)
    {
        noInterrupts ();
        tic = false;
        float timeCopy = currentTime;
        interrupts ();

        digitalWrite (digitalPin, HIGH);
        digitalWrite (digitalPin, LOW);

        if (lastTimePrinted + timePrintPeriod < timeCopy)
        {
            lastTimePrinted = timeCopy;
            Serial.println (timeCopy);
        }
        
        if (task != NULL)
        {
            bool done = task->PeriodicTask (timeCopy);
            
            if (done)
            {
                char obuf [100];
                
                task = NULL;
                sprintf (obuf, "done after %d seconds", (int) (currentTime + 0.5));  Serial.println (obuf);
            }            
        }
    }
}
