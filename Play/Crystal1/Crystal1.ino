
// Crystal1

// read and report temperatures via WiFi

//#include <EEPROM.h>
#include <JobQueue.h>

#include <OneWire.h>
#include <DallasTemperature.h>
//#include "OneWireSim.h"
//#include "DallasTemperatureSim.h"

#include "Messages.h"
#include "SampleManager.h"
#include "TcpClientRev2.h"

#include <Secret.h>

TcpClientRev2 socket;
SampleManager sampleManager (&socket);

JobQueue         oneTimeJobs;
PeriodicJobQueue periodicJobs;

//********************************************************************************************

// Temperature Sensor

// Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS 15 /*2*/

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire (ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature temperatureSensor (&oneWire);

// buffer for received messages
#define MaxMessageBytes 64
char messageBytes [MaxMessageBytes];

//***********************************************************************************

void setup() 
{
    // start serial port
    Serial.begin(9600);
    Serial.println ("");
    Serial.println (F("Crystal1 - read and report temperatures"));
    Serial.println ("");

  //******************************************************************
  
    bool success = TcpClientRev2::ConnectToNetwork (SSID, PASS);

    if (success) 
        Serial.println (F("Connected to network"));
    else
    {
      Serial.println (F("Failed to connect to network"));
      while (1)
      ;
    }

  //******************************************************************
  
    success = socket.ConnectToServer (server, 11000);
    
    if (success) 
        Serial.println (F("Connected to server"));
    else
    {
      Serial.println (F("Failed to connect to server"));
      while (1)
      ;
    }

  //******************************************************************

  // Start up the temperature sensor
    temperatureSensor.begin();
    temperatureSensor.setResolution (12);
}

//********************************************************************************************

void loop(void)
{
    unsigned long now = millis ();
    oneTimeJobs.RunJobs (now);
    periodicJobs.RunJobs (now);
    
    if (socket.IsConnected ())
    {
        if (socket.checkForMessages (messageBytes, sizeof (messageBytes)))
        {
            MessageHeader *header = (MessageHeader *) messageBytes;
           
            switch (header->MsgId)
            {
                case SendStatusMsgId:    SendStatus (); break;                                   
                case StartSamplingMsgId: StartSampling ((StartSamplingMsg *) messageBytes); break;
                case StopSamplingMsgId:  StopSampling  ((StopSamplingMsg *)  messageBytes); break;    

                case ClearHistoryMsgId:  ClearHistory (); break;
                case SendHistoryMsgId:   SendHistory (); break;
                
                case DisconnectMsgId:    Disconnect (); break;
                
                default: //Serial.println ("Unsupported message received"); 
                     break;                    
            }   
        }
    }
}

//***************************************************************************************

// Send Status command handler

StatusMessage status;

void SendStatus ()
{
    if (socket.IsConnected ())
        socket.write ((char *) &status, status.header.ByteCount);
}

//***************************************************************************************

void TakeSample (void)
{
    temperatureSensor.requestTemperatures(); // Send the command to get temperatures
    float temperature = temperatureSensor.getTempCByIndex (0) * 9.0/5 + 32; // returns float degrees C, convert to Fahrenheit   
    
    sampleManager.Store (millis (), temperature);
}
//***************************************************************************************

// Start Sampling command handler

char *TakeSampleName = "Sample";

void StartSampling (StartSamplingMsg *msg)
{
    //Serial.print ("Period: "); Serial.println (msg->period);
    
    status.sampling = 1;
    SendStatus ();

    periodicJobs.Remove (TakeSampleName); // only want one at a time
    periodicJobs.Add (TakeSample, NULL, TakeSampleName, msg->period * 1000);
}

void StopSampling (StopSamplingMsg *)
{
    status.sampling = 0;
    SendStatus ();

    periodicJobs.Remove (TakeSampleName);
    sampleManager.SendPartialMessage ();
}

//***************************************************************************************

void ClearHistory ()
{
    sampleManager.Clear ();  
}

//***************************************************************************************

char *sendHistoryJobName = "SendHistory";

void SendHistory ()
{
    sampleManager.SendFirstHistoryMessage ();
    periodicJobs.Add (SendNextHistoryMessage, NULL, sendHistoryJobName, 100);
}

void SendNextHistoryMessage (void *)
{
    bool done = sampleManager.SendNextHistoryMessage ();

    if (done)
        periodicJobs.Remove (sendHistoryJobName);
}

//***************************************************************************************

// Disconnect command handler

void Disconnect ()
{
    socket.close ();
    //TcpClientRev2::disconnect(); 
    //Serial.println (F("Disconnect"));              

    oneTimeJobs.Add (Reconnect, NULL, 39600000UL); // 60UL * 1000UL);  --------------------  11 hours
}

//***************************************************************************************

// Reconnect - queued after disconnecting

void Reconnect (void *)
{
    //Serial.println ("reconnect");
    status.sampling = 0;
    periodicJobs.Remove (TakeSampleName);
    socket.ConnectToServer (server, 11000);
    SendStatus ();
}






    
