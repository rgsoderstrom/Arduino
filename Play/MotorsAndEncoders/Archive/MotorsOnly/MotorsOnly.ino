
//
// MotorsOnly
//


#include <MessageHeader.h>
#include <TcpClientRev2.h>
#include <JobQueue.h>

#include "Messages.h"

#include "DualMC33926MotorShield.h"

DualMC33926MotorShield md;

PeriodicJobQueue PeriodicJobs;

//
// Client socket that will connect to PC
//
TcpClientRev2 socket;

//*********************************************************************

// socket

#include <Secret.h>

//*********************************************************************

#define MaxProfileLength  150
int leftSpeedProfile [MaxProfileLength];
int rightSpeedProfile [MaxProfileLength];

int profileLength = 0;
int profileReadIndex = 0;

//*********************************************************************

void setup() 
{
    Serial.begin (9600);
    md.init ();
    
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

    TextMessage msg ("Arduino Ready");
    socket.write ((char *) &msg, msg.header.ByteCount);

}

char messageBytes [120];
    
void loop() 
{
    unsigned long timeNow = millis ();
    PeriodicJobs.RunJobs (timeNow);


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
                case ClearProfileMsgId:
                {
                    TextMessage tm ("Clear Profile");
                    socket.write ((char *) &tm, tm.header.ByteCount);

                    for (int i=0; i<MaxProfileLength; i++)
                    {
                      leftSpeedProfile [i] = 0;
                      rightSpeedProfile [i] = 0;
                    }

                    profileLength = 0;
                }
                break;

                case ProfileSectionMsgId: 
                { 
                    MotorSpeedMsg *msg = (MotorSpeedMsg *) messageBytes;                    
                    /************
                    char obuf [30];
                    sprintf (obuf, "section %d, %d", msg->data.index, msg->data.numberValues);
                    TextMessage tm (obuf);
                    socket.write ((char *) &tm, tm.header.ByteCount);
                    *************/                    

                    for (int i=0; i<msg->data.numberValues; i++)
                    {
                        if (msg->data.index + i >= MaxProfileLength)
                          break;
                          
                        leftSpeedProfile  [msg->data.index + i] = msg->data.LeftSpeed [i];  
                        rightSpeedProfile [msg->data.index + i] = msg->data.RightSpeed [i];  
                    }

                    profileLength = max (profileLength, msg->data.index + msg->data.numberValues);
                    
                    ProfileSectionRcvdMessage msg2;
                    socket.write ((char *) &msg2, msg2.header.ByteCount);
                }
                break;

                case RunProfileMsgId:
                {
                    TextMessage tm ("Run Profile");
                    socket.write ((char *) &tm, tm.header.ByteCount);

                    profileReadIndex = 0;
                    PeriodicJobs.Add (SendSpeedToMotors, NULL, "SendSpeed", 100);
                }
                break;
                
                case KeepAliveMsgId:
                {
                }
                break;

                default:
                {
                    TextMessage tm ("Unrecognized MsgId");
                    socket.write ((char *) &tm, tm.header.ByteCount);
                }
                break;
            }
        }
    }    
}

//*****************************************************************************************************

void SendSpeedToMotors ()
{
    if (profileReadIndex < profileLength)
    {
        md.setM1Speed (leftSpeedProfile [profileReadIndex]);
        md.setM2Speed (rightSpeedProfile [profileReadIndex]);
        profileReadIndex++;
    }

    else // if (profileReadIndex == profileLength)
    {
        md.setM1Speed (0);
        md.setM2Speed (0);
        PeriodicJobs.Remove ("SendSpeed");      
        TextMessage tm ("Done");
        socket.write ((char *) &tm, tm.header.ByteCount);
    }  
}
