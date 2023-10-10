
//
// MessageHandlers.cpp
//

#include <Arduino.h>
#include <TcpClientRev2.h>

#include "MessageHandlers.h"
#include "src/LoopbackDataMessage.h"
#include "src/StatusMessage.h"
#include "src/TextMessage.h"

#define null ((void *) 0)

//**************************************************************************

MessageHandlers::MessageHandlers ()
{
    socketPtr        = null;
    fpgaInterfacePtr = null;
    statusMsgPtr     = null;
}

//**************************************************************************

void MessageHandlers::Initialize (StatusMessage *smp, FPGA_Interface *fpga, TcpClientRev2 *sp)
{
    fpgaInterfacePtr = fpga;
    socketPtr        = sp;
    statusMsgPtr     = smp;
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

void MessageHandlers::LoopbackDataMsgHandler (byte msgBytes []) 
{ 
    Serial.println ("Loopback data");
    ((LoopbackDataMsg *) msgBytes)->ToConsole ();
        
    int byteCount = ((MessageHeader *) msgBytes)->ByteCount;
    
//    Serial.println ("Passing PC RunTest msg to FPGA");
//    Serial.print (byteCount);
//    Serial.println (" bytes");
    
    fpgaInterfacePtr->WriteBytes (msgBytes, byteCount);
      
    statusMsgPtr->SetDataReceived (1);
    socketPtr->write ((char *) statusMsgPtr, statusMsgPtr->ByteCount ());       
}

//**************************************************************************

void MessageHandlers::RunLoopbackTestMsgHandler (byte msgBytes [])
{
    int byteCount = ((MessageHeader *) msgBytes)->ByteCount;
    
//    Serial.println ("Passing PC RunTest msg to FPGA");
//    Serial.print (byteCount);
//    Serial.println (" bytes");
//    
    fpgaInterfacePtr->WriteBytes (msgBytes, byteCount);
//        statusMsgPtr->SetDataReady (1);
//        socketPtr->write ((char *) statusMsgPtr, statusMsgPtr->ByteCount ());       
}
      
void MessageHandlers::SendLoopbackTestResultsMsgHandler (byte msgBytes [])
{
    int byteCount = ((MessageHeader *) msgBytes)->ByteCount;
    
//    Serial.println ("Passing PC SendResults msg to FPGA");
//    Serial.print (byteCount);
//    Serial.println (" bytes");
    
    fpgaInterfacePtr->WriteBytes (msgBytes, byteCount);
  
    statusMsgPtr->SetDataReceived (0);
    statusMsgPtr->SetDataReady (0);
    socketPtr->write ((char *) statusMsgPtr, statusMsgPtr->ByteCount ());       
}
