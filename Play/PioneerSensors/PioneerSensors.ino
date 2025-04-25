
//
// PioneerSensors
//

#include <JobQueue.h>
#include <TcpClientRev2.h>

#include "src/MessageIDs.h"
#include "src/Messages.h"
#include "MessageHandlers.h"

PeriodicJobQueue PeriodicJobs;
JobQueue         OneTimeJobs;

TcpClientRev2 *socketPtr; // Client socket that will connect to PC

MessageHandlers messageHandler; // most app-specific processing

//****************************************************************************

void OpenSocket ()
{
    socketPtr = new TcpClientRev2 ();
    messageHandler.Initialize (socketPtr, &PeriodicJobs, &OneTimeJobs);
    
    // tell laptop that Arduino is ready
    ReadyMsg_Auto msg1;
    socketPtr->write ((char *) &msg1, msg1.header.ByteCount);

    char readyText [60] = "SSID ";
    strcat (readyText, WiFi.SSID());
    
    TextMessage msg2 (readyText);
    socketPtr->write ((char *) &msg2, msg2.ByteCount ());   
	
	// char *fv = WiFi.firmwareVersion ();
	
    // TextMessage vmsg (fv);
    // socketPtr->write ((char *) &vmsg, vmsg.ByteCount ());   
}

//****************************************************************************

void setup() 
{
    Serial.begin (9600);
    Serial.println ("PioneerSensors");
    
    OneTimeJobs.Clear ();
    PeriodicJobs.Clear ();

    OpenSocket ();
}

//***********************************************************************************

#define MaxMessageBytes 256
char messageBytes [MaxMessageBytes]; // messages received from PC

void loop() 
{
    unsigned long now = millis ();
    PeriodicJobs.RunJobs (now);
    OneTimeJobs.RunJobs (now);
    
    noInterrupts ();
    bool isConnected = socketPtr->IsConnected ();
//    bool isConnected = true;// socketPtr->IsConnected ();
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

            AcknowledgeMsg ack (header->SequenceNumber);
            socketPtr->write ((char *) &ack, ack.ByteCount ());
            
            switch (header->MsgId)
            {                
                case KeepAliveMsgId: break;
                         
                case StartSamplingMsgId: messageHandler.StartSamplingMsgHandler (messageBytes); break;
                case SendSamplesMsgId:   messageHandler.SendSamplesMsgHandler   (messageBytes); break;
                
                default: 
                {
                    char text [80];
                    sprintf (text, "Unexpected msg ID %d", header->MsgId);
                    TextMessage msg (text); 
                    socketPtr->write ((char *) &msg, msg.ByteCount ());   
                }
                break;
            }

            Serial.println ("Sending ReadyMsg");

            ReadyMsg_Auto rdy;
            socketPtr->write ((char *) &rdy, rdy.header.ByteCount);
        }
    }
}
