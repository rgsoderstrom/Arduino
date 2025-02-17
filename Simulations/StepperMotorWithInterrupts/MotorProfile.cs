using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace MotorShieldSim
{
    public abstract class MotorProfile
    {
        public const ushort MaxPWM = 4096;


        public enum MotorDirection     {Forward, Reverse, Off};
        public enum StepperMotorSelect {M1 = 1, M2};
        public enum DCMotorSelect      {M1 = 1, M2, M3, M4};



        static private ushort interruptsPerSecond = 1000; 

        public float InterruptsPerSecond
        {
            get {return ((float) (interruptsPerSecond));}
            set {interruptsPerSecond = (ushort) (value);}            
        }

        public float SecondsPerInterrupt
        {
            get {return 1 / InterruptsPerSecond;}
            set {InterruptsPerSecond = 1 / value;}
        }

        public float InterruptsPerMicroseconds
        {
            get {return (float) (InterruptsPerSecond / 1e6);}
            set {InterruptsPerSecond = (float) (value * 1e6);}            
        }

        public float MicrosecondsPerInterrupt
        {
            get {return 1 / InterruptsPerMicroseconds;}
            set {SecondsPerInterrupt = (float) (value / 1e6);}            
        }

        public MotorProfile ()
        {
        }
    }
}
