
//***************************************************************
//
// MotorProfile.h - virtual base class for DC & Stepper motor profiles
//    

#ifndef MOTORPROFILE_H
#define MOTORPROFILE_H

class MotorProfile
{
    public:
      MotorProfile () {interruptsPerSecond = 100;}
      MotorProfile (unsigned short ips) {interruptsPerSecond = ips;}

      float GetInterruptsPerSecond () {return (float) interruptsPerSecond;}
      void  SetInterruptsPerSecond (unsigned short value) {interruptsPerSecond = value;}

      float GetSecondsPerInterrupt () {return 1 / interruptsPerSecond;}
      void  SetSecondsPerInterrupt (float value) {interruptsPerSecond = 1 / value;}

     // largest value to write to PWM register. (NumberWritten / MaxPWM) is duty cycle
      static const unsigned int MaxPWM = 4096;

    private:
      unsigned short interruptsPerSecond;
};

#endif

