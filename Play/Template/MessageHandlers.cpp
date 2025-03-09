
//
// MessageHandlers.cpp
//

#include <Arduino.h>
#include <TcpClientRev2.h>
#include "src\TextMessage.h"

#include "MessageHandlers.h"

#define null ((void *) 0)

MessageHandlers* MessageHandlers::thisPtr = null;

//********************************************************************************

MessageHandlers::MessageHandlers ()
{
    socketPtr       = null;
    periodicJobsPtr = null;
    jobsPtr         = null;

}

//**************************************************************************

void MessageHandlers::Initialize (TcpClientRev2 *sp, PeriodicJobQueue *pjq, JobQueue *jq)
{
    socketPtr       = sp;
    periodicJobsPtr = pjq;
    jobsPtr         = jq;
}

//**************************************************************************

void MessageHandlers::Button1MsgHandler (byte msgBytes [])
{
    TextMessage msg ("Button 1 reply");
    socketPtr->write ((char *) &msg, msg.ByteCount ());   
}

void MessageHandlers::Button2MsgHandler (byte msgBytes [])
{
    delay (250);

    TextMessage msg ("Button 2 reply");
    socketPtr->write ((char *) &msg, msg.ByteCount ());   
}

void MessageHandlers::Button3MsgHandler (byte msgBytes [])
{
    delay (500);

    TextMessage msg ("Button 3 reply");
    socketPtr->write ((char *) &msg, msg.ByteCount ());   
}


