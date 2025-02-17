using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace MotorShieldSim
{
    public class StepperMotor
    {
        public enum ShieldConnector {M1 = 1, M2};

        public ShieldConnector Connector {get; set;}

        public ushort StepsPerRevolution {get; set;}
    }
}
