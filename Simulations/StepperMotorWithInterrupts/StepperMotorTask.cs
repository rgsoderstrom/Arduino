using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace MotorShieldSim
{
    public class StepperMotorTask
    {
      //
      // statics
      //
        public static Adafruit_MotorShield AFMS = null;
        private Adafruit_StepperMotor motorInterface = null;

        private StepperMotorSpeedProfile speedProfile;

        float timeOfNextStep = 0; // seconds

        

        //********************************************************************************
        //
        // Constructor
        //
        public StepperMotorTask (StepperMotor             motor,
                                 StepperMotorSpeedProfile profile)
        {
            motorInterface = AFMS.getStepper (profile.StepsPerRevolution, (byte) motor.Connector);
            speedProfile = profile;
        }

        //********************************************************************************
        //
        // 
        //
        public void StartAt (float t) 
        {
            speedProfile.StartAt (t);
            speedProfile.GetNext (ref timeOfNextStep);
        }

        //********************************************************************************
        //
        // 
        //
        public bool PeriodicTask (float timeNowSeconds) 
        {
            bool done = false;

            if (timeNowSeconds >= timeOfNextStep)
            {
               motorInterface.onestep ((byte) speedProfile.Direction, Constants.DOUBLE);
         //    motorInterface.onestep_double_forward ();

                done = speedProfile.GetNext (ref timeOfNextStep);


         //       Console.WriteLine ("{0}", timeOfNextStep - timeNowSeconds);
            }

            return done;
        }

    }
}
