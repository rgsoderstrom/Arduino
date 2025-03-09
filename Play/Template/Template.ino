
//
// Template.ino - bare-bones example of a sketch to communicate with a PC
//

#include <JobQueue.h>
#include <TcpClientRev2.h>

#include "src/MessageIDs.h"
#include "src/Messages.h"
#include "MessageHandlers.h"

PeriodicJobQueue PeriodicJobs;
JobQueue         OneTimeJobs;

TcpClientRev2 *socketPtr; // Client socket that will connect to PC

MessageHandlers messageHandler; // contains most of the application-specific processing

//****************************************************************************

void setup() 
{
    //
    // Standard initializations
    //
    Serial.begin (9600);
    Serial.println ("PioneerSensors");
    
    OneTimeJobs.Clear ();
    PeriodicJobs.Clear ();
   
    socketPtr = new TcpClientRev2 ();
    messageHandler.Initialize (socketPtr, &PeriodicJobs, &OneTimeJobs);
    
    // tell laptop that Arduino is ready
    ReadyMsg_Auto msg1;
    socketPtr->write ((char *) &msg1, msg1.header.ByteCount);

    char readyText [60] = "SSID ";
    strcat (readyText, WiFi.SSID());
    
    TextMessage msg2 (readyText);
    socketPtr->write ((char *) &msg2, msg2.ByteCount ());   
}

//****************************************************************************

#define MaxMessageBytes 256
char messageBytes [MaxMessageBytes]; // messages received from PC
 
void loop() 
{
    unsigned long now = millis ();
    PeriodicJobs.RunJobs (now);
    OneTimeJobs.RunJobs (now);
    
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

          AcknowledgeMsg ack (header->SequenceNumber);
          socketPtr->write ((char *) &ack, ack.ByteCount ());
            
          switch (header->MsgId)
          {                
            case KeepAliveMsgId: break;
                       
            case Button1MsgId: messageHandler.Button1MsgHandler (messageBytes); break;
            case Button2MsgId: messageHandler.Button2MsgHandler (messageBytes); break;
            case Button3MsgId: messageHandler.Button3MsgHandler (messageBytes); break;
                
            default: 
              //Serial.print   ("Unexpected msg ID "); Serial.println (header->MsgId); 
              break;
          }

          ReadyMsg_Auto msg2;
          socketPtr->write ((char *) &msg2, msg2.header.ByteCount);
        }
    }
}
