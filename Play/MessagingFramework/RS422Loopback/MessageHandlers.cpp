
//
// MessageHandlers.cpp
//

#include <Arduino.h>
#include <TcpClientRev2.h>

#include "MessageHandlers.h"
#include "src/LoopbackDataMessage.h"
#include "src/StatusMessage.h"
#include "src/TextMessage.h"

#include "LoopbackTest.h"
#include "LoopbackTestFPGA.h"

#define null ((void *) 0)

//**************************************************************************

MessageHandlers::MessageHandlers ()
{
    socketPtr = null;
    statusMsgPtr = null;
}

//**************************************************************************

void MessageHandlers::Initialize (StatusMessage *smp, TcpClientRev2 *sp)
{
    socketPtr = sp;
    statusMsgPtr = smp;
}

//**************************************************************************

void MessageHandlers::StatusRequestMsgHandler (byte msgBytes [])
{
    Serial.println ("StatusRequestMsgId");
    TextMessage msg ("Status Request");
    socketPtr->write ((char *) &msg, msg.ByteCount ());       

    socketPtr->write ((char *) statusMsgPtr, statusMsgPtr->ByteCount ());       
}

//**************************************************************************

//LoopbackTestFPGA *loopbackTest = null;
LoopbackTest *loopbackTest = null;

void MessageHandlers::LoopbackDataMsgHandler (byte msgBytes []) 
{ 
    Serial.println ("Loopback data");
    LoopbackDataMsg *msg = new LoopbackDataMsg (msgBytes);
    
    loopbackTest = new LoopbackTest (msg->GetData ());    
    //loopbackTest = new LoopbackTestFPGA (msg->GetData ());    
    delete msg;

    statusMsgPtr->SetDataReceived (1);
    socketPtr->write ((char *) statusMsgPtr, statusMsgPtr->ByteCount ());       
}

//**************************************************************************

void MessageHandlers::RunLoopbackTestMsgHandler (byte msgBytes [])
{
    if (loopbackTest != null)
    {
        loopbackTest->RunTest ();  
        statusMsgPtr->SetDataReady (1);
        socketPtr->write ((char *) statusMsgPtr, statusMsgPtr->ByteCount ());       
    }
}
      
void MessageHandlers::SendLoopbackTestResultsMsgHandler  (byte msgBytes [])
{
    if (loopbackTest != null)
    {    
        LoopbackDataMsg *msg = new LoopbackDataMsg (loopbackTest->GetResults ());
        socketPtr->write ((char *) msg, msg->ByteCount ());       
        delete msg;
        delete loopbackTest;
        loopbackTest = null;
    }
}
