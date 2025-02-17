using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace MotorShieldSim
{
    //*****************************************************************************************
    //
    // SpeedProfileSegment
    //
    class SpeedProfileSegment
    {
      // specified by caller
        public float  startTime; // seconds
        public float  duration;  // seconds

        public float  finalRPM;
        public float  initialRPM; // final RPM of previous segment

      // calculated
        public UInt16 initialPPS; // final PPS of previous segment
        public UInt16 finalPPS;   // calculated from caller specified final RPM

        public float C1;  // see Visio drawing or Matlab stepper.m
        public float C2;

        public UInt16 numberSteps; // calculated number of steps this segment
    }

    public class StepperMotorSpeedProfile : MotorProfile
    {
        private List<SpeedProfileSegment> speedProfile = new List<SpeedProfileSegment> ();

        private int getIndex = 0;    // used to read-back profile, index of current segment in speedProfile[]

        private UInt16 stepsSent = 0;   // number of steps that have been sent for current profile segment

        private float profileStartTime = 0;


        public UInt16 NumberSteps
        {
            get
            {
                UInt16 count = 0;

                foreach (SpeedProfileSegment seg in speedProfile)
                    count += seg.numberSteps;

                return count;
            }
        }



        public ushort StepsPerRevolution {get; set;} // motor characteristic
        public MotorDirection Direction {get; set;}



        //********************************************************************************
        //
        // Constructor
        //
        public StepperMotorSpeedProfile (ushort stepsPerRev, MotorDirection dir)
        {
            StepsPerRevolution = stepsPerRev;
            Direction = dir;
        }

        //********************************************************************************
        //
        // StartAt ()
        //
        public void StartAt (float time)
        {
            profileStartTime = time;
        }

        //********************************************************************************
        //
        // Ramp - add a speed change to profile
        //
        public void Ramp (float seconds, 
                          float finalRPM)
        {
            SpeedProfileSegment segment = new SpeedProfileSegment ();
             
            if (speedProfile.Count == 0)
            {
                segment.initialRPM = 0;
                segment.initialPPS = 0;
                segment.startTime  = 0;
            }
            else
            {
                SpeedProfileSegment previous = speedProfile [speedProfile.Count - 1];

                segment.initialRPM = previous.finalRPM;
                segment.initialPPS = previous.finalPPS;
                segment.startTime  = previous.startTime + previous.duration;
            }

            segment.finalRPM = finalRPM;
            segment.finalPPS = (ushort) (segment.finalRPM  * StepsPerRevolution * Constants.MinutesPerSecond);
            segment.duration = seconds;

            segment.numberSteps = (ushort) (Math.Min (segment.initialPPS, segment.finalPPS) * segment.duration + 0.5 * segment.duration * Math.Abs (segment.finalPPS - segment.initialPPS));

            segment.C1 = segment.initialPPS;
            segment.C2 = 0.5f * (segment.finalPPS - segment.initialPPS) / segment.duration;

            speedProfile.Add (segment);
        }

        //********************************************************************************
        //
        // Hold - add a constant speed section to profile
        //
        public void Hold (float seconds)
        {
            if  (speedProfile.Count == 0)
                Ramp (seconds, 0);
            else
                Ramp (seconds, speedProfile [speedProfile.Count - 1].finalRPM);
        }

        //********************************************************************************
        //
        // ResetReadback () - called prior to beginning a read-back
        //
        public void ResetReadback ()
        {
            getIndex = 0;
            stepsSent = 0;
        }

        //********************************************************************************
        //
        // GetNext - get time of next pulse. returns true if this is last value
        //
        public bool GetNext (ref float nextTime)
        {
            SpeedProfileSegment segment = speedProfile [getIndex];

            stepsSent++; // calculate the time of the next step

          // if in a speed change section
            if (Math.Abs (segment.C2) > 1e-4) // arbitrary small threshold
            {
                nextTime = (float) (-segment.C1 + Math.Sqrt (segment.C1 * segment.C1 - 4 * segment.C2 * -stepsSent)) / (2 * segment.C2);
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
            }

            return getIndex == speedProfile.Count;
        }
    }
}
