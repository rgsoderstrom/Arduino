//
// RS422Loopback - WiFi version
//

#include <TcpClientRev2.h>

#include "MessageHandlers.h"
#include "src/MessageIDs.h"
#include "src/AcknowledgeMessage.h"
#include "src/TextMessage.h"
#include "src/StatusMessage.h"

TcpClientRev2 *socketPtr; // Client socket that will connect to PC

//
// System Status stored here, to be sent to PC
//
StatusMessage statusMsg;

char *ThisName = "Ard 1";

//
// MessageHandler
//
MessageHandlers messageHandler;

void setup() 
{
    Serial.begin (9600);
    Serial.println ();
	
	  socketPtr = new TcpClientRev2 ();
   
    messageHandler.Initialize (&statusMsg, socketPtr);

    // tell laptop that Arduino is ready, with a text message and a status message
    TextMessage msg ("Arduino Ready");
    socketPtr->write ((char *) &msg, msg.ByteCount ());

    statusMsg.SetName (ThisName);
    statusMsg.SetDataReceived (false);
    statusMsg.SetDataReady (false);
    socketPtr->write ((char *) &statusMsg, statusMsg.ByteCount ());
    
    interrupts ();    
}

#define MaxMessageBytes 64
char messageBytes [MaxMessageBytes];

void loop() 
{
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

            Serial.print ("MessageID ");
            Serial.println (header->MsgId);
            
            switch (header->MsgId)
            {                
                case StatusRequestMsgId:   messageHandler.StatusRequestMsgHandler           (messageBytes); break;
                case LoopbackDataMsgId:    messageHandler.LoopbackDataMsgHandler            (messageBytes); break;
                case RunLoopbackTestMsgId: messageHandler.RunLoopbackTestMsgHandler         (messageBytes); break;               
                case SendLoopbackDataMsgId:messageHandler.SendLoopbackTestResultsMsgHandler (messageBytes); break;

                
                case KeepAliveMsgId:  /*Serial.println ("KeepAliveMsgId");*/  break;
//                default:  /*Serial.println ("???");*/ break;
            }

            AcknowledgeMessage ack (header->SequenceNumber);
            socketPtr->write ((char *) &ack, ack.ByteCount ());
        }
    }
}
