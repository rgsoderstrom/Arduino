//
// RS422Loopback - WiFi version
//

#include <JobQueue.h>
#include <TcpClientRev2.h>
#include <FPGA_Interface.h>
#include <FPGA_MsgBytes.h>

#include "MessageHandlers.h"
#include "src/MessageIDs.h"
#include "src/AcknowledgeMessage.h"
#include "src/TextMessage.h"
#include "src/StatusMessage.h"

JobQueue OneTimeJobs;

//
// Client socket that will connect to PC
//
TcpClientRev2 *socketPtr; 

//
// FPGA Interface
//
FPGA_MsgBytes  fpgaByteBuffer; // accumulate bytes received from FPGA here
FPGA_Interface fpgaInterface;

//
// System Status stored here, to be sent to PC
//
StatusMessage statusMsg;

char *ThisName = "Arduino & FPGA";

//
// MessageHandler
//
MessageHandlers messageHandler;

void setup() 
{
    Serial.begin (9600);
    Serial.println ();
	  OneTimeJobs.Clear ();
   
	  socketPtr = new TcpClientRev2 ();

    //**********************************************************************
    
    fpgaByteBuffer.Clear ();
    fpgaInterface.ConfigurePins ();
    fpgaInterface.AttachInterrupt ();
    fpgaInterface.RegisterInterruptCallback (InterruptCallbackFcn);
 
    //**********************************************************************
    
    messageHandler.Initialize (&statusMsg, &fpgaInterface, socketPtr);

    // tell laptop that Arduino is ready, with a text message and a status message
    TextMessage msg ("Arduino Ready");
    socketPtr->write ((char *) &msg, msg.ByteCount ());

    statusMsg.SetName (ThisName);
    statusMsg.SetDataReceived (false);
    statusMsg.SetDataReady (false);
    socketPtr->write ((char *) &statusMsg, statusMsg.ByteCount ());
}

#define MaxMessageBytes 256
char messageBytes [MaxMessageBytes];

void loop() 
{
    OneTimeJobs.RunJobs (millis ());
    
    noInterrupts ();
    bool isConnected = socketPtr->IsConnected ();
    interrupts ();
    
    if (isConnected)
    {
        noInterrupts ();
        bool isMessage = socketPtr->checkForMessages (messageBytes, sizeof (messageBytes));
        interrupts ();
        
        if (isMessage)
        {
            MessageHeader *header = (MessageHeader *) messageBytes;

            Serial.print ("PC MessageID ");
            Serial.println (header->MsgId);
            
            switch (header->MsgId)
            {                
                case StatusRequestMsgId:    messageHandler.StatusRequestMsgHandler           (messageBytes); break;
                case LoopbackDataMsgId:     messageHandler.LoopbackDataMsgHandler            (messageBytes); break;
                case RunLoopbackTestMsgId:  messageHandler.RunLoopbackTestMsgHandler         (messageBytes); break;               
                case SendLoopbackDataMsgId: messageHandler.SendLoopbackTestResultsMsgHandler (messageBytes); break;

                case KeepAliveMsgId:  /*Serial.println ("KeepAliveMsgId");*/  break;
//                default:  /*Serial.println ("???");*/ break;
            }

            AcknowledgeMsg ack (header->SequenceNumber);
            socketPtr->write ((char *) &ack, ack.ByteCount ());
        }
    }
}

//***********************************************************************************

void InterruptProcessing (void *ptr)
{
    unsigned char ch = fpgaInterface.ReadOneByte ();
//    Serial.print ("FPGA byte: 0x");
//    Serial.println ((int) ch, HEX); 

    fpgaByteBuffer.StoreByte (ch);

    if (fpgaByteBuffer.MessageComplete ())
    {
        Serial.print ("FPGA Message ID ");
        Serial.print (fpgaByteBuffer.GetMessageID ());
        Serial.println (" received from FPGA");

        // forward message on to PC
        if (fpgaByteBuffer.GetMessageID () == 100) // loopback test data bytes
        {
          socketPtr->write (fpgaByteBuffer.GetMsgBytes (), fpgaByteBuffer.GetByteCount ());
          statusMsg.SetDataReady (0);
          socketPtr->write ((char *) &statusMsg, statusMsg.ByteCount ());         
        }
        
        if (fpgaByteBuffer.GetMessageID () == 104)
        {
            statusMsg.SetDataReady (1);
            socketPtr->write ((char *) &statusMsg, statusMsg.ByteCount ());         
        }

        // prepare for next message
        fpgaByteBuffer.Clear ();              
    }
}

//
// Interrupt Callback - called when FPGA has a byte to send
//
void InterruptCallbackFcn ()
{
    OneTimeJobs.Add (InterruptProcessing, NULL);
}
