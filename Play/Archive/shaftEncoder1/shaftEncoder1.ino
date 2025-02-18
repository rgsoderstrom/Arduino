
// shaftEncoder1 - characterize motor driver & ShaftEncoder
//               - max = approx. 880 transitions in 1 second, less than 1/2 revolution

#include <math.h>
#include "ShaftEncoder.h"
#include "DualMC33926MotorShield.h"

ShaftEncoder shaft1;
ShaftEncoder shaft2;

DualMC33926MotorShield motorController;

#define NumbTests       10  // check set of all data points this many times
#define NumbDataPoints 400

int transitions1 [NumbDataPoints];
int transitions2 [NumbDataPoints];

int testCounter = 0;
int dataPointCounter = 0;

unsigned long lastMillis;
int speed = 0;

//***************************************************************************

void setup() 
{
    Serial.begin (115200);

    for (int i=0; i<NumbDataPoints; i++)
    {
        transitions1 [i] = 0;
        transitions2 [i] = 0;
    }

    testCounter = 0;
    dataPointCounter = 0;

    shaft1.Open (1);
    shaft2.Open (2);

    motorController.init ();
    motorController.setM1Speed (0);
    motorController.setM2Speed (0);

    lastMillis = millis ();
}

//***************************************************************************

bool done = false;

void loop() 
{
    if (done)
        return;
        
    unsigned long now = millis ();
    
    shaft1.TestForTransition ();
    shaft2.TestForTransition ();

    if (now > lastMillis + 100) //1000)
    {
        lastMillis = now;
        
        transitions1 [dataPointCounter]   += fabs (shaft1.GetNumberOfTransitions (true));
        transitions2 [dataPointCounter++] += fabs (shaft2.GetNumberOfTransitions (true));

        if (dataPointCounter == NumbDataPoints)
        {
            dataPointCounter = 0;
            speed = 0;
            
            if (++testCounter == NumbTests)
                done = true;
        }
        
        if (!done)
        {
            speed++;
            motorController.setM1Speed (speed);
            motorController.setM2Speed (speed);
        }
        else
        {
            motorController.setM1Speed (0);
            motorController.setM2Speed (0);
            
            for (int i=0; i<NumbDataPoints; i++)
            {
                Serial.print (i+1); 
                Serial.print (" "); Serial.print (transitions1 [i] / NumbTests);  
                Serial.print (" "); Serial.print (transitions2 [i] / NumbTests); 
                
                if (i < NumbDataPoints - 1) Serial.println ("; ...");                   
            }
        }
    }
}




