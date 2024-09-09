
//
// PassThrough -
//    - pass messages received from PC (over WIFI) to FPGA
//    - pass messages received from FPGA to PC
//

#include <JobQueue.h>
#include <TcpClientRev2.h>
#include <FPGA_Interface.h>
#include <FPGA_MsgBytes.h>

#include "src/MessageIDs.h"
#include "src/TextMessage.h"
#include "src/AcknowledgeMessage.h"

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

void setup() 
{
    Serial.begin (9600);
    Serial.println ("Passthrough");
    
    OneTimeJobs.Clear ();
   
    socketPtr = new TcpClientRev2 ();

    //**********************************************************************
    
    fpgaByteBuffer.Clear ();
    fpgaInterface.ConfigurePins ();
    fpgaInterface.AttachInterrupt ();
    fpgaInterface.RegisterInterruptCallback (InterruptCallbackFcn);
 
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

            AcknowledgeMsg ack (header->SequenceNumber);
            socketPtr->write ((char *) &ack, ack.ByteCount ());
            
            switch (header->MsgId)
            {                
                case KeepAliveMsgId:
                         break;
                
				        case 100:
				        case 101:
                case 102:
                case 103:
                case 104:
				                 fpgaInterface.WriteBytes (messageBytes, header->ByteCount);                
						             break;
                
                default:
                      { 
                            const int N = 60;
                            char text1 [N];
                            snprintf (text1, N, "Unexpected msg ID: %d", header->MsgId);
                            TextMessage msg3 (text1);
                            socketPtr->write ((char *) &msg3, msg3.ByteCount ());
                  
                          //Serial.print   ("Unexpected msg ID ");
                          //Serial.println (header->MsgId);
                            break;
                      }
            }
        }
    }
}

//***********************************************************************************

void InterruptProcessing (void *ptr)
{
    unsigned char ch = fpgaInterface.ReadOneByte ();
    //Serial.print ("FPGA byte: 0x");
    //Serial.println ((int) ch, HEX); 

    fpgaByteBuffer.StoreByte (ch);

    if (fpgaByteBuffer.MessageComplete ())
    {
        Serial.print   ("From FPGA, Message ID ");
        Serial.println (fpgaByteBuffer.GetMessageID ());

        // forward message on to PC
        socketPtr->write (fpgaByteBuffer.GetMsgBytes (), fpgaByteBuffer.GetByteCount ());
        
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
