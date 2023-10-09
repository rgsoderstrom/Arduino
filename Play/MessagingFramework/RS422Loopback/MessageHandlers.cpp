
//
// MessageHandlers.cpp
//

#include <Arduino.h>
#include <TcpClientRev2.h>

#include "MessageHandlers.h"
#include "src/LoopbackDataMessage.h"
#include "src/StatusMessage.h"
#include "src/TextMessage.h"

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
//**************************************************************************
//**************************************************************************
//
// LoopbackDataMsgHandler 
//    - called when Loopback Data Message received
//    - passed message bytes
//

LoopbackTestFPGA *loopbackTest = null;

void MessageHandlers::LoopbackDataMsgHandler (byte msgBytes []) 
{ 
    Serial.println ("Loopback data");
    ((LoopbackDataMsg *) msgBytes)->ToConsole ();
    
    
    loopbackTest = new LoopbackTestFPGA (msgBytes);    
    statusMsgPtr->SetDataReceived (1);
    socketPtr->write ((char *) statusMsgPtr, statusMsgPtr->ByteCount ());       
}

//**************************************************************************

void MessageHandlers::RunLoopbackTestMsgHandler (byte msgBytes [])
{
    if (loopbackTest != null)
    {
        loopbackTest->RunTest (msgBytes);
		
        statusMsgPtr->SetDataReady (1);
        socketPtr->write ((char *) statusMsgPtr, statusMsgPtr->ByteCount ());       
    }
}
      
void MessageHandlers::SendLoopbackTestResultsMsgHandler (byte msgBytes [])
{
    if (loopbackTest != null)
    {    
        loopbackTest->SendResults (msgBytes); // forward message on to FPGA
        
        statusMsgPtr->SetDataReceived (0);
        statusMsgPtr->SetDataReady (0);
        socketPtr->write ((char *) statusMsgPtr, statusMsgPtr->ByteCount ());       
    }
}
