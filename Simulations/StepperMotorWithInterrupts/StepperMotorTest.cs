﻿
//
// StepperMotorTest - models the Arduino top-level functions
//                      - setup ()
//                      - loop ()
//
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace MotorShieldSim
{
    public class StepperMotorTest
    {
      // describes motor hardware and its connection
        public StepperMotor motor = null;


        public StepperMotorTask task = null;

        //***************************************************************

        public StepperMotorTest ()
        {
            StepperMotorTask.AFMS = new Adafruit_MotorShield ();
        }

        //***************************************************************

        public bool TaskRunning ()
        {
            return (task != null);
        }

        //***************************************************************

        public void timerInterrupt ()
        {
            if (task != null)
            {
                bool done = task.PeriodicTask (Driver.TimeNowInSeconds ());

                if (done)
                    task = null;
            }
        }

        //***************************************************************

        public void setup ()
        {
          // motor characteristics
            motor = new StepperMotor ();
            motor.StepsPerRevolution = 200;
            motor.Connector = StepperMotor.ShieldConnector.M2;

          // desired speed profile
            StepperMotorSpeedProfile speedProfile = new StepperMotorSpeedProfile (motor.StepsPerRevolution, MotorProfile.MotorDirection.Forward);

            speedProfile.Ramp (2.5f, 60);
            speedProfile.Hold (5);
            speedProfile.Ramp (2.5f, 0);

          //**************************************************************************************
          //
          // write profile to an include file for use by Arduino
          //
            StreamWriter writetext = new StreamWriter(@"C:\Users\Randy\Documents\Arduino\Simulations\StepperMotorWithInterrupts\profile.h");

            writetext.WriteLine("//");
            writetext.WriteLine("// profile.h - stepper motor profile, generated by Visual Studio StepperMotorWithInterrupts project");
            writetext.WriteLine("//");

            writetext.WriteLine("");
            writetext.WriteLine("static int stepsPerRevolution = {0};", motor.StepsPerRevolution);
            writetext.WriteLine("");
            writetext.WriteLine("static int numberProfileSteps = {0};", speedProfile.NumberSteps);
            writetext.WriteLine("");

            writetext.WriteLine ("PROGMEM const float stepTimes [] = {");

            bool done = false;

            while (done == false)
            {
                float t = 0;
                done = speedProfile.GetNext (ref t);
                writetext.WriteLine ("{0},", t);
            }

            writetext.WriteLine ("};");
            writetext.Close();

          //**************************************************************************************

          // create stepper motor task
            speedProfile.ResetReadback ();
            task = new StepperMotorTask (motor, speedProfile);
            task.StartAt (0);//(0.5f);
        }

        //***************************************************************

        // called repeatedly

        public void loop ()
        {
            timerInterrupt ();
        }



    }
}
