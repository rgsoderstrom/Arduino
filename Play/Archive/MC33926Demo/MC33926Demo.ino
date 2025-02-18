#include "DualMC33926MotorShield.h"

DualMC33926MotorShield md;

void stopIfFault()
{
  if (md.getFault())
  {
    Serial.println("fault");
    while(1);
  }
}

int startDelay     = 3000; // milliseconds
int rampUpTime     = 3000;
int constSpeedTime = 2000;
int rampDownTime   = 2000;

int maxSpeed = 200;

bool done = false;
int startMillis = 0;

void setup()
{
    done = false;
    startMillis = millis ();
    
    for (int i=0; i<19; i++)
      pinMode (i, INPUT);

  pinMode (1, OUTPUT);
  digitalWrite (1, 1);
  
  Serial.begin(9600);
  Serial.println("Dual MC33926 Motor Shield");
  md.init();

  md.setM1Speed (0);
  md.setM2Speed (0);

#if defined(__AVR_ATmega168__)|| defined(__AVR_ATmega328P__)
  Serial.println("AVR ATmega");
  // Timer 1 configuration
  // prescaler: clockI/O / 1
  // outputs enabled
  // phase-correct PWM
  // top of 400
  //
  // PWM frequency calculation
  // 16MHz / 1 (prescaler) / 2 (phase-correct) / 400 (top) = 20kHz
  TCCR1A = 0b10100000;
  TCCR1B = 0b00010001;
  ICR1 = 400;
#endif

  //TCA0.CTRLA = 4;
  //TCB3.CTRLB |= 0x10;            TCB3 controls "delay ()", not PWM

  //TCB3.CNT = 0x32;
 // TCB3.CCMPL = 0xf;
 // TCB3.CCMPH = 0xf;

  //TCA0.SINGLE.CTRLA = 9;

/************
  unsigned int A = TCA0.SINGLE.CTRLA; 
  unsigned int B = TCA0.SINGLE.CTRLB;
  unsigned int C = TCA0.SINGLE.CTRLC;
  unsigned int D = TCA0.SINGLE.CTRLD;
  
  unsigned int P = TCA0.SINGLE.PER; 
  
  unsigned int C0 = TCA0.SINGLE.CMP0; // READ TCA0 REGS AFTER WRITING TO PWM PINS, WHILE MOTOR RUNNING
  unsigned int C1 = TCA0.SINGLE.CMP1;
  unsigned int C2 = TCA0.SINGLE.CMP2;


  
  Serial.println (A, HEX); // B
  Serial.println (B, HEX); // 3
  Serial.println (C, HEX); // 7
  Serial.println (D, HEX); // 0

  Serial.println (P, HEX);  // ff
  Serial.println (C0, HEX); // 80
  Serial.println (C1, HEX); // 80
  Serial.println (C2, HEX); // 80
***********/

#ifdef _AVR_ATMEGA4808_H_INCLUDED
    Serial.println ("_AVR_ATMEGA4808_H_INCLUDED");
#endif
    
#ifdef _AVR_ATMEGA4809_H_INCLUDED
    Serial.println ("_AVR_ATMEGA4809_H_INCLUDED");
#endif
    
}

int lastSpeedSent = -1;

void loop()
{
    if (done == true)
      return;

    int timeNow = millis () - startMillis;

    if (timeNow < startDelay)
    {
        return;
    }

    else if (timeNow < startDelay + rampUpTime)
    {
        int dt       = timeNow - startDelay;
        int speedNow = maxSpeed * (float) dt / rampUpTime;
        
        if (lastSpeedSent != speedNow)
        {
          lastSpeedSent = speedNow;
          md.setM1Speed (lastSpeedSent);
          md.setM2Speed (lastSpeedSent);
        }
    }
  
    else if (timeNow < startDelay + rampUpTime + constSpeedTime)
    {
        //int dt = timeNow - (startDelay + rampUpTime);
        
      
    }
  
    else if (timeNow < startDelay + rampUpTime + constSpeedTime + rampDownTime)
    {
        int dt       = timeNow - (startDelay + rampUpTime + constSpeedTime);        
        int speedNow = maxSpeed * (1.0 - (float) dt / rampUpTime);

        if (lastSpeedSent != speedNow)
        {
          lastSpeedSent = speedNow;
          md.setM1Speed (lastSpeedSent);
          md.setM2Speed (lastSpeedSent);
        }
    }

    else
    {
        done = true;
        md.setM1Speed (0);
        md.setM2Speed (0);
    }
  
//    int maxCount = 1000;
//    double maxSpeed = 400;
//    
//    for (int i=0; i<maxCount; i++)
//    {
//        double angle = 2 * PI * (float) i / maxCount;
//
//        md.setM1Speed ((int) (maxSpeed * sin (angle)));
//        md.setM2Speed ((int) (maxSpeed * cos (angle)));
//        stopIfFault();
//        delay (20);
//    }

}
