
//
// SampleManager.cpp
//

#include <arduino.h>

#include "SampleManager.h"

TcpClientRev2 *socketPtr;

float HistorySample::BaseTemperature = 65;
float HistorySample::TemperatureScale = 100; // scaledTemp units per degree 

unsigned long HistorySample::BaseTime = 0;     // time hiatory collection starts, millis 
unsigned long HistorySample::TimeScale = 1000; // 

SampleManager::SampleManager (TcpClientRev2 *ptr)
{ 
    socketPtr = ptr;
    Clear ();
}

//*******************************************************************************

void SampleManager::Clear () 
{
    put = 0; 
    get = 0;

    HistorySample::SetBaseTime (millis ());
    message.Clear ();
}

//*******************************************************************************

void SampleManager::Store (unsigned long timeTag, float temperature)
{
    bool storedInMessage = false;
    
    if (socketPtr != NULL)
    {
        if (socketPtr->IsConnected ())
        {
            AddToMessage (timeTag, temperature);
            storedInMessage = true;
        }
    }

    if (storedInMessage == false)
          AddToHistory (timeTag, temperature);
}

void SampleManager::AddToMessage (unsigned long timeTag, float temperature)
{
    if (message.Full ())
        message.Clear ();

    message.Add (timeTag, temperature);
    
    if (message.Full ())
        socketPtr->write ((char *) &message, message.header.ByteCount);  




/***
    if (message.Full ())
    {
        Serial.print (message.samples [0].timeTag); Serial.print (",  "); Serial.println (message.samples [0].temperature); 

        int N = message.NumberTemperatures - 1;
        Serial.print (message.samples [N].timeTag); Serial.print (",  "); Serial.println (message.samples [N].temperature); 

        Serial.println ("==========================");
    }
***/

}

void SampleManager::AddToHistory (unsigned long timeTag, float temperature)
{
    if (put < NumberHistorySamples)
    {
        history [put].SetTemperature (temperature);
        history [put].SetTime (timeTag);
        put++;
    }
}

//*******************************************************************************

bool SampleManager::SendFirstHistoryMessage ()
{
    get = 0;
    return SendNextHistoryMessage ();     
}

bool SampleManager::SendNextHistoryMessage ()
{
    if (get >= put)
        return true;

    message.Clear ();

    bool done = false;
    
    do
    {
        unsigned long timeTag = history [get].GetTime ();
        float temperature     = history [get].GetTemperature ();
        message.Add (timeTag, temperature);
        ++get;
        
        done = message.Full () || (get == put);
        
    } while (done == false);
    
    socketPtr->write ((char *) &message, message.header.ByteCount);  
    
    return (get == put);      
}

//*******************************************************************************

void SampleManager::SendPartialMessage ()
{
    if (message.Empty () == false)
        if (socketPtr)
          socketPtr->write ((char *) &message, message.header.ByteCount);  


/****
    if (message.Empty () == false)
    {
        Serial.print (message.samples [0].timeTag); Serial.print (",  "); Serial.println (message.samples [0].temperature); 

        int N = message.NumberTemperatures - 1;
        Serial.print (message.samples [N].timeTag); Serial.print (",  "); Serial.println (message.samples [N].temperature); 

        Serial.println ("***************************88");
    }
****/




    message.Clear ();
}
 
   
