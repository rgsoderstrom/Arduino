
//
// MessageHandlers.cpp
//

#include <Arduino.h>
#include <TcpClientRev2.h>

#include "MessageHandlers.h"
// #include "src/LoopbackDataMessage.h"
// #include "src/StatusMessage.h"
// #include "src/TextMessage.h"

#define null ((void *) 0)

//**************************************************************************

MessageHandlers::MessageHandlers ()
{
    socketPtr = null;
}

//**************************************************************************

void MessageHandlers::Initialize (TcpClientRev2 *sp)
{
    socketPtr = sp;
    nextSample = 512;
}

//**************************************************************************

void MessageHandlers::ClearMsgHandler (byte msgBytes [])
{
    Serial.println ("Clear");
    
    for (int i=0; i<BufferSize; i++)
      SampleBuffer [i] = 1234;

    get = 0;
}

void MessageHandlers::CollectMsgHandler (byte msgBytes [])
{
    Serial.println ("Collect");
    
    for (int i=0; i<BufferSize; i++)
      SampleBuffer [i] = nextSample++;

    get = 0;
}

void MessageHandlers::SendMsgHandler (byte msgBytes [])
{
    Serial.print   ("Send ");
    Serial.println (get);
    
    for (int i=0; i<SampleDataMsg_Auto::Data::MaxCount; i++)
    {
        sampleMsg.data.Sample [i] = SampleBuffer [get++];
    }

    socketPtr->write ((char *) &sampleMsg, sampleMsg.header.ByteCount);

    if (get >= BufferSize)
    {
      socketPtr->write ((char *) &allSentMsg, allSentMsg.header.ByteCount);
      get = 0;
    }
}

//**************************************************************************
//**************************************************************************
//**************************************************************************
