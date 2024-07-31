
//
// PassThrough_Sim -
//    - accept messages received from PC (over WIFI) intended for FPGA
//    - simulate FPGA behavior and send messages the FPGA would have
//

#include <MessageHeader.h>
//#include <Secret.h>
#include <TcpClientRev2.h>

#include <JobQueue.h>

#include "src/MessageIDs.h"
#include "src/Messages.h"
#include "MessageHandlers.h"

JobQueue OneTimeJobs;

//
// Client socket that will connect to PC
//
TcpClientRev2 *socketPtr; 

MessageHandlers messageHandler;

void setup() 
{
    Serial.begin (9600);
    Serial.println ("Passthrough_Sim");
    
    OneTimeJobs.Clear ();
   
    socketPtr = new TcpClientRev2 ();
    messageHandler.Initialize (socketPtr);
    
    // tell laptop that Arduino is ready
    ReadyMsg_Auto msg1;
    socketPtr->write ((char *) &msg1, msg1.header.ByteCount);

    char readyText [60] = "SSID ";
    strcat (readyText, WiFi.SSID());
    
    TextMessage msg2 (readyText);
    socketPtr->write ((char *) &msg2, msg2.ByteCount ());
}

//*****************************************************************

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

            //Serial.print ("PC MessageID ");
            //Serial.println (header->MsgId);
            
            switch (header->MsgId)
            {                
                case KeepAliveMsgId:
                         break;
                         
                case ClearMsgId:   messageHandler.ClearMsgHandler   (messageBytes); break;
                case CollectMsgId: messageHandler.CollectMsgHandler (messageBytes); break;
                case SendMsgId:    messageHandler.SendMsgHandler    (messageBytes); break;               
                
                default: Serial.print   ("Unexpected msg ID ");
                         Serial.println (header->MsgId);
                         break;
            }

            AcknowledgeMsg ack (header->SequenceNumber);
            socketPtr->write ((char *) &ack, ack.ByteCount ());
        }
    }
}
