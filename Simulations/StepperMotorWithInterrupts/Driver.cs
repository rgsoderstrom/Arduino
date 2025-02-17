
//
// Driver - models the Arduino environment
//          - calls setup () once at startup
//          - calls loop () repeatedly
//
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.IO;

namespace MotorShieldSim
{
    class Driver
    {
        public static StreamWriter outputStream = new StreamWriter (@"C:\Users\rgsod\Documents\Arduino\Simulations\StepperMotorWithInterrupts\output.txt");
        private static uint currentTime = 0;

        static StepperMotorTest test = null;

        //***************************************************************************

        public static void delay (uint millis)
        {
            Driver.outputStream.Write ("{0} ", Driver.currentTime);

            for (int i=0; i<Adafruit_PWMServoDriver.NumberPWMs; i++)
            {
                Driver.outputStream.Write ("{0} ", StepperMotorTask.AFMS._pwm.dutyCycle [i]);
            }

            Driver.outputStream.WriteLine ("");

            currentTime += millis;
        }

        //***************************************************************************

        public static float TimeNowInSeconds ()
        {
            return (float) (currentTime / 1000.0);
        }

        //***************************************************************************

        static void Main (string [] args)
        {
            try
            {
                test = new StepperMotorTest ();

                test.setup ();

                while (test.TaskRunning ())
                {
                    test.loop ();
                    Driver.delay (1); // milliseconds
                }

                outputStream.Close ();
            }

            catch (Exception ex)
            {
                Console.WriteLine ("Exception: {0}", ex.Message);
            } 
        }
    }
}
