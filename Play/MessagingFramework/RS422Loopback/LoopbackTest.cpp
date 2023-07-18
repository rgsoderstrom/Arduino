
//
// LoopbackTest.cpp
//

#include "LoopbackTest.h"

LoopbackTest::LoopbackTest (LoopbackDataMsg::Data receivedData)
{
    testInput.source = receivedData.source;
    testInput.dataByteCount = receivedData.dataByteCount;

    for (int i=0; i<receivedData.dataByteCount; i++)
        testInput.dataBytes [i] = receivedData.dataBytes [i];
}

void LoopbackTest::RunTest ()
{
    testOutput.source = testInput.source + 10;
    testOutput.dataByteCount = testInput.dataByteCount;
  
    for (int i=0; i<testInput.MaxCount; i++)
        testOutput.dataBytes [i] = testInput.dataBytes [testInput.MaxCount - 1 - i];
}
  
LoopbackDataMsg::Data& LoopbackTest::GetResults ()
{
    return testOutput;
}
