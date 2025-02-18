
//
// ShaftEncoder.cpp - this version counts all transistions (i.e. both inputs) and checks 
//                    for direction change every time
//

#include "Arduino.h"

#include "ShaftEncoder.h"

const char ShaftEncoder::states [] = {0, 1, 3, 2};
const int  ShaftEncoder::N = sizeof (ShaftEncoder::states) / sizeof (ShaftEncoder::states [0]);

//***********************************************************************************

ShaftEncoder::ShaftEncoder ()
{
}

//***********************************************************************************

bool ShaftEncoder::Open (int select)
{
    if (select != 1 && select != 2)
    {
        Serial.println ("Invalid ShaftEncoder select");    
        return false;
    }
    
    if (select == 1)
    {
        inputA = encoder1InputA;
        inputB = encoder1InputB;
    }
    else if (select == 2)
    {
        inputA = encoder2InputA;
        inputB = encoder2InputB;
    }
    
    pinMode (inputA, INPUT);
    pinMode (inputB, INPUT);
    numberTransitions = 0;
    numberMissed = 0;
    
    int a = digitalRead (inputA);
    int b = digitalRead (inputB);

    wasA = a;
    wasB = b;
    int initialState = ((a << 1) | b) & (N-1);

    for (wasIndex=0; wasIndex<N; wasIndex++)
        if (states [wasIndex] == initialState)
            break; 
}

//***********************************************************************************

void ShaftEncoder::GetNumberOfTransitions (int& numbTrans, int& numbMissed, bool clearAfterRead)
{
    numbTrans  = numberTransitions;
    numbMissed = numberMissed;

    if (clearAfterRead)
    {
        numberTransitions = 0;
        numberMissed = 0;
    }
}

int ShaftEncoder::GetNumberOfTransitions (bool clearAfterRead)
{
    int count = numberTransitions;

    if (clearAfterRead)
    {
        numberTransitions = 0;
        numberMissed = 0;
    }
        
    return count;
}

//***********************************************************************************

void ShaftEncoder::TestForTransition ()
{
    int a = digitalRead (inputA);
    int b = digitalRead (inputB);

    if (a != wasA || b != wasB)
    {
        int isState = ((a << 1) | b) & (N-1);
        wasA = a;
        wasB = b;

        int isIndex;
        
        for (isIndex=0; isIndex<N; isIndex++)
            if (states [isIndex] == isState)
                break; 
        
      // expect isIndex to be one of these. if not, we missed a transistion
        int i1 = (wasIndex + 1) & (N - 1);
        int i2 = (wasIndex - 1) & (N - 1);

      // for next iteration
        wasIndex = isIndex;
        
        if (isIndex == i1) 
            numberTransitions++;
        else if (isIndex == i2) 
            numberTransitions--;
        else
        {
            numberMissed++;
        }
    }   
}




