
//
// SystemTest1 - two motors & two encoders layed out on bench
//

#include <MessageHeader.h>
#include <TcpClientRev2.h>
#include <JobQueue.h>

#include "MotorBoard.h"
#include "FPGA_Interface.h"
#include "Messages.h"

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

//*********************************************************************

// socket

#include <Secret.h>

//*********************************************************************

//
// Encoder Counts History
//
#define HistorySize 123 /*200*/
int historyPut = 0;
int historyGet = 0;
EncoderCounts History [HistorySize];

void setup() 
{
  Serial.begin (9600);

  // FPGA pins
  fpgaInterface.ConfigurePins ();
  
  // motor driver board pins
  motorBoard.ConfigurePins ();

  //******************************************************************
  
    int retryCounter = 5;
    bool success = false;

    Serial.print (F("Connecting to network"));
    
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

    retryCounter = 5;
    
    Serial.print (F("Connecting to server"));


    IPAddress ipa; // (192, 168, 137, 1);;
    int A = WiFi.hostByName (server, ipa);
    Serial.print (A);
    Serial.print (", ");
    Serial.println (ipa);



    
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

    historyPut = 0;

    TextMessage msg ("Arduino Ready");
    socket.write ((char *) &msg, msg.header.ByteCount);

    fpgaInterface.RegisterEncoderCallback (EncoderCallbackFcn);
    //fpgaInterface.AttachInterrupt ();

    motorBoard.EnableMotors ();
    interrupts ();
}

//**************************************************************************************************
//**************************************************************************************************
//**************************************************************************************************

//
// build message to send to PC
//
CollectionDataMessage encoderMsg;

//
// buffer for received messages
//
#define MaxMessageBytes 64
char messageBytes [MaxMessageBytes];

//**************************************************************************************************

//
// Callbacks are invoked from within an interrupt routine so they should do no
// more work than necessary
//

unsigned char speed1 = 0; // last commanded speeds
unsigned char speed2 = 0;

void EncoderCallbackFcn (unsigned long tt, int e1, int e2)
{
    if (historyPut >= 0 && historyPut < HistorySize)
    {
        History [historyPut].timeTag = tt;
        History [historyPut].enc1 = e1;
        History [historyPut].enc2 = e2;
        History [historyPut].s1 = speed1;
        History [historyPut].s2 = speed2; 
        historyPut++;

        //Serial.println (speed1);
    }
    else
    {
        fpgaInterface.DetachInterrupt ();
        OneTimeJobs.Add (SendBufferFullMessage, "full");
    }
}

void MotorCallbackFcn (int sel)
{
  
}

//**************************************************************************************************

void Receiving ()
{
    TextMessage msg ("Receiving, half full");
    if (socket.IsConnected ()) socket.write ((char *) &msg, msg.header.ByteCount);            
}

void SendBufferFullMessage ()
{
    TextMessage msg1 ("Full");
    if (socket.IsConnected ()) socket.write ((char *) &msg1, msg1.header.ByteCount);            
    BufferStatusMessage msg2 (1); // buffer full
    if (socket.IsConnected ()) socket.write ((char *) &msg2, msg2.header.ByteCount);            
}

//**************************************************************************************************

bool SendDataCollectionMessage ()
{
    if (historyGet < historyPut)
    {
        encoderMsg.Clear ();

        while (historyGet < historyPut)
        {
            encoderMsg.Add (History [historyGet++]);
        
            if (encoderMsg.Full () == true)
            {
                socket.write ((char *) &encoderMsg, encoderMsg.header.ByteCount);
                encoderMsg.Clear ();
                return true;
            }
        }

        if (encoderMsg.Empty () == false)
        {
            socket.write ((char *) &encoderMsg, encoderMsg.header.ByteCount);
            encoderMsg.Clear ();
        }
    }
    
    return false;        
}

//**************************************************************************************************

void loop() 
{    
    OneTimeJobs.RunJobs (millis ());

    char messageBytes [120];

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
                case MotorSpeedMsgId: 
                {
                    MotorSpeedMsg *msg = (MotorSpeedMsg *) messageBytes;                    
                    speed1 = msg->Velocity1;
                    speed2 = msg->Velocity2;
                    fpgaInterface.SetMotorSpeed (speed1, speed2);

                    char obuf [30];
                    sprintf (obuf, "%d, %d", msg->Velocity1, msg->Velocity2);
                    TextMessage tm (obuf);
                    socket.write ((char *) &tm, tm.header.ByteCount);
                }
                break;

                case StartCollectionMsgId:
                {
                    TextMessage tm ("Start Collection");
                    socket.write ((char *) &tm, tm.header.ByteCount);
                    historyPut = 0;                    
                    BufferStatusMessage bs (0);
                    socket.write ((char *) &bs, bs.header.ByteCount);
                    fpgaInterface.AttachInterrupt ();
                }
                break;                


                case SendFirstCollectionMsgId:  // NOTE: this case falls through
                    historyGet = 0;

                case SendNextCollectionMsgId:
                {
                    noInterrupts ();
                    fpgaInterface.DetachInterrupt ();

                    if (SendDataCollectionMessage () == false)
                    {
                        CollSendCompleteMessage csc;
                        socket.write ((char *) &csc, csc.header.ByteCount);                          
                    }
                    
                    interrupts ();
                }
                break;
  




/*************
                case SendCollectionMsgId:
                {
                    noInterrupts ();
                    fpgaInterface.DetachInterrupt ();
                    encoderMsg.Clear ();

                    char obuf [30];
                    sprintf (obuf, "%d samples", historyPut);
                    TextMessage tt (obuf);
                    socket.write ((char *) &tt, tt.header.ByteCount);


                    for (int i=0; i<historyPut; i++)
                    {
                        encoderMsg.Add (History [i]);
                        
                        if (encoderMsg.Full () == true)
                        {
                            socket.write ((char *) &encoderMsg, encoderMsg.header.ByteCount);
                            encoderMsg.Clear ();
                        }                        
                    }

                    if (encoderMsg.Empty () == false)
                    {
                        socket.write ((char *) &encoderMsg, encoderMsg.header.ByteCount);
                        encoderMsg.Clear ();                      
                    }



                    TextMessage tt2 ("send complete");
                    socket.write ((char *) &tt2, tt2.header.ByteCount);
                    
                    CollSendCompleteMessage csc;
                    socket.write ((char *) &csc, csc.header.ByteCount);
                    interrupts ();
                }
                break;      *************/
                                
                case ClearCollectionMsgId:
                {
                    historyPut = 0;  
                    BufferStatusMessage bs (0);
                    socket.write ((char *) &bs, bs.header.ByteCount);
                    TextMessage tm ("Cleared Collection");
                    socket.write ((char *) &tm, tm.header.ByteCount);
                }
                break;                
                
                case StopCollectionMsgId:
                {
                    fpgaInterface.DetachInterrupt ();
                    TextMessage tm ("Stopping Collection");
                    socket.write ((char *) &tm, tm.header.ByteCount);
                }
                break;                
                
                case KeepAliveMsgId:
                {
                    //TextMessage tm ("KeepAlive");
                    //socket.write ((char *) &tm, tm.header.ByteCount);
                }
                break;                
            }
        }
    }    
}
