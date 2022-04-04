
//
// Motors and drive wheel encoders on chassis. Passed a speed vs. time profile for each motor, runs it
// and sends the measured motor speeds back to PC
//

#include <TcpClientRev2.h>
#include <JobQueue.h>

#include "MotorBoard.h"
#include "FPGA_Interface.h"
#include "Messages.h"
#include "MessageHandlers.h"

JobQueue OneTimeJobs;

//
// Communicate with motor driver board
//
MotorBoard motorBoard;

//
// Communicate with FPGA
//  
FPGA_Interface fpgaInterface;

//
// Client socket that will connect to PC
//
TcpClientRev2 socket;

//
// network and socket names and encryption key
//
#include <Secret.h>

//
// System Status stored here, to be sent to PC
//
StatusMessage::StatusData statusData ("Arduino"); 

//
// allocate MessageHandler static variables here. They
// are initialized in setup ()
//
StatusMessage::StatusData *MessageHandlers::statusDataPtr = 0;
TcpClientRev2             *MessageHandlers::socketPtr = 0;
FPGA_Interface            *MessageHandlers::fpgaPtr = 0;
MotorBoard                *MessageHandlers::motorBoardPtr = 0;

//*********************************************************************
//*********************************************************************
//*********************************************************************

void setup() 
{
    MessageHandlers::statusDataPtr = &statusData;
    MessageHandlers::socketPtr     = &socket;
    MessageHandlers::fpgaPtr       = &fpgaInterface;
    MessageHandlers::motorBoardPtr = &motorBoard;
    
    motorBoard.ConfigurePins (MotorBoard::ControlOptions::EnableOnly);
    
    int retryCounter = 5;
    bool success = false;
 
    Serial.begin (9600);
    Serial.print (F("Connecting to network "));
    Serial.print (SSID);
    
    do 
    {
        success = TcpClientRev2::ConnectToNetwork (SSID, PASS);
        if (success == false) Serial.print (F("."));
    
    } while ((success == false) && (retryCounter-- > 0));

    if (success)
        Serial.println (F(" succeeded"));
    else
    {
        Serial.println (F(" failed, stopping"));

        while (1)
        ;
    }
    
  //******************************************************************

    Serial.println (F("Get server address"));
    IPAddress ipa;
    success = WiFi.hostByName (server, ipa); // returns 1 if successful

    if (success)
    {
        Serial.print ("Server address: ");
        Serial.println (ipa);
    }
    else
    {
        Serial.println (F(" failed to get server address, stopping"));
        while (1)
        ;
    }
    
    Serial.println (F("Connecting to server"));
    retryCounter = 5;
    
    do
    {
        success = socket.ConnectToServer (server, 11000);
        if (success == false) Serial.print (F("."));
    
    } while ((success == false) && (retryCounter-- > 0));

    if (success)
        Serial.println (F(" succeeded"));
    else
    {
        Serial.println (F(" failed, stopping"));
        while (1)
        ;
    }

    //
    // tell laptop that Arduino is ready, with a text message and a status message
    //
    TextMessage msg ("Arduino Ready");
    socket.write ((char *) &msg, msg.ByteCount ());
    
    statusData.readyForMessages = 1;
    StatusMessage statusMsg (statusData);
    socket.write ((char *) &statusMsg, statusMsg.ByteCount ());
    
    fpgaInterface.ConfigurePins ();
    fpgaInterface.RegisterInterruptCallback (InterruptCallbackFcn);
    fpgaInterface.AttachInterrupt ();

    interrupts ();
}

//*********************************************************************
//*********************************************************************
//*********************************************************************

void InterruptProcessing ()
{
//    unsigned long w = fpgaInterface.ReadOneWord ();
//
//    Serial.print ("fpga byte: ");
//    Serial.println (w);
}

void InterruptCallbackFcn ()
{
    OneTimeJobs.Add (InterruptProcessing, NULL);
}

//*********************************************************************
//*********************************************************************
//*********************************************************************
//
// buffer for received messages
//
#define MaxMessageBytes 64
char messageBytes [MaxMessageBytes];

MessageHandlers msgHandler;

void loop() 
{
    OneTimeJobs.RunJobs (millis ());

    noInterrupts ();
    bool isConnected = socket.IsConnected ();
    interrupts ();
    
    if (isConnected)
    {
        noInterrupts ();
        bool isMessage = socket.checkForMessages (messageBytes, sizeof (messageBytes));
        interrupts ();
        
        if (isMessage)
        {
            MessageHeader *header = (MessageHeader *) messageBytes;

            switch (header->MsgId)
            {                
              //case StatusRequestMsgId:       msgHandler.StatusRequestMsgHandler       (messageBytes); break;
                case SpeedProfileSegmentMsgId: msgHandler.SpeedProfileSegmentMsgHandler (messageBytes); break;
                case ClearMotorProfileMsgId:   msgHandler.ClearMotorProfileMsgHandler   (messageBytes); break;  
                case TransferSpeedProfileMsgId:msgHandler.TransferProfileMsgHandler     (messageBytes); break;                
                case RunMotorsMsgId:           msgHandler.RunMotorsMsgHandler           (messageBytes); break;                
                case SlowStopMotorsMsgId:      msgHandler.SlowStopMotorsMsgHandler      (messageBytes); break;
                case FastStopMotorsMsgId:      msgHandler.FastStopMotorsMsgHandler      (messageBytes); break;
              //case SendFirstCollectionMsgId: msgHandler.SendFirstCollectionMsgHandler (messageBytes); break;
              //case SendNextCollectionMsgId:  msgHandler.SendNextCollectionMsgHandler  (messageBytes); break;

                case DisconnectMsgId: /*Serial.println ("DisconnectMsgId");*/ break;
                case KeepAliveMsgId:  /*Serial.println ("KeepAliveMsgId");*/  break;
                default:  /*Serial.println ("???");*/ break;
            }

            AcknowledgeMessage ack (header->SequenceNumber);
            socket.write ((char *) &ack, ack.ByteCount ());
        }
    }
}
