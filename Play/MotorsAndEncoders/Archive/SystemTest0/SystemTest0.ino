
//
// SystemTest0 - test messages and protocols
//

#include <MessageHeader.h>
#include <TcpClientRev2.h>
#include <JobQueue.h>

#include "Messages.h"

JobQueue OneTimeJobs;

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

#define HistorySize 200  /* was 20 */
int historyPut = 0;
//int historyGet = 0;
EncoderCounts History [HistorySize];


void setup() 
{
  Serial.begin (9600);

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

    do
    {  
        success = (WiFi.hostByName (server, ipa) == 1);
   
        if (success) {Serial.print (", ip = ") ; Serial.print (ipa); Serial.print (" ");}
        else Serial.println (" - can't find IP address");
  
        if (success)
          success = socket.ConnectToServer (ipa, 11000);
  
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

    TextMessage msg ("Arduino Ready");
    socket.write ((char *) &msg, msg.header.ByteCount);
}

//******************************************************************

//
// build message to send to PC
//
CollectionDataMessage encoderMsg;

//
// buffer for received messages
//
#define MaxMessageBytes 64
char messageBytes [MaxMessageBytes];

//******************************************************************

void SendBufferFullMessage ()
{
    TextMessage msg1 ("Full");
    if (socket.IsConnected ()) socket.write ((char *) &msg1, msg1.header.ByteCount);            
    BufferStatusMessage msg2 (1); // buffer full
    if (socket.IsConnected ()) socket.write ((char *) &msg2, msg2.header.ByteCount);            
}

void loop() 
{
    OneTimeJobs.RunJobs (millis ());

    if (socket.IsConnected ())
    {
        if (socket.checkForMessages (messageBytes, sizeof (messageBytes)))
        {
            MessageHeader *header = (MessageHeader *) messageBytes;

            switch (header->MsgId)
            {
                case MotorSpeedMsgId: 
                {
                    MotorSpeedMsg *msg = (MotorSpeedMsg *) messageBytes;                    
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
                    FillHistory ();
                    SendBufferFullMessage ();
                }
                break;                

                case SendCollectionMsgId:
                {
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

                    CollSendCompleteMessage csc;
                    socket.write ((char *) &csc, csc.header.ByteCount);
                }
                break;                
                
                case ClearCollectionMsgId:
                {
                    BufferStatusMessage bs (0);
                    socket.write ((char *) &bs, bs.header.ByteCount);
                    TextMessage tm ("Cleared Collection");
                    socket.write ((char *) &tm, tm.header.ByteCount);
                }
                break;                
                
                case StopCollectionMsgId:
                {
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

//******************************************************************

int t = 0;
EncoderCounts cnts;

void FillHistory ()
{
    t = millis ();
    
    for (int i=0; i<HistorySize; i++)
    {
        cnts.timeTag = (unsigned long) (100 + t);
        cnts.enc1 = (int) (300 + 200 * sin (2 * PI * t / 90.0));
        cnts.enc2 = (int) (300 + 100 * sin (2 * PI * 2 * t / 100.0));
  
        cnts.s1 = (byte) (10 + 3 * sin (2 * PI * 4 * t / 90.0));
        cnts.s2 = (byte) (9 + 2 * sin (2 * PI * 6 * t / 100.0));
        
        History [i] = cnts;
        t += 1;
    } 

    historyPut = HistorySize;
}
