
//
// LoopbackTestFPGA.cpp
//

#include "LoopbackTestFPGA.h"

LoopbackTestFPGA::LoopbackTestFPGA (LoopbackDataMsg::Data receivedData)
{  
    testInput.source = receivedData.source;
    testInput.dataByteCount = receivedData.dataByteCount;

    for (int i=0; i<receivedData.dataByteCount; i++)
        testInput.dataBytes [i] = receivedData.dataBytes [i];

    pinMode (A2, INPUT);
    pinMode (A3, OUTPUT);
    pinMode (A4, INPUT);
    pinMode (A5, OUTPUT);

    pinMode (2, INPUT);
    pinMode (3, INPUT);
    pinMode (5, OUTPUT);
    pinMode (6, OUTPUT);
        
}

void LoopbackTestFPGA::RunTest ()
{
    testOutput.dataByteCount = 0;
    
    for (int i = 0; i < testInput.dataByteCount; i++)
    {
        int nybble = testInput.dataBytes [i] & 0xf;
        
        digitalWrite (A3, (nybble & 1) != 0);  
        digitalWrite (A5, (nybble & 2) != 0);  
        digitalWrite (5,  (nybble & 4) != 0);  
        digitalWrite (6,  (nybble & 8) != 0);  

        int bit0 = digitalRead (A2);
        int bit1 = digitalRead (2);
        int bit2 = digitalRead (A4);
        int bit3 = digitalRead (3);

        byte readByte = (bit3 << 7) | (bit2 << 6) | (bit1 << 5) | (bit0 << 4); 
        
        testOutput.dataBytes [testOutput.dataByteCount++] = readByte;
    }

    testOutput.source = testInput.source + 20;
}
  
LoopbackDataMsg::Data& LoopbackTestFPGA::GetResults ()
{
    return testOutput;
}
