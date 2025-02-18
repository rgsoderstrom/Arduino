
//
// shaftEncoder2 - read encoder counts, send over WiFi
//

#include <math.h>
#include <TcpClientRev2.h>
#include "Messages.h"

const int LoadSR = 4;    // output
const int ShiftSR = 6;   // output
const int ReadSR = 5;    // input

//***************************************************************************

// socket

#include <Secret.h>

TcpClientRev2 socket;
EncoderCountsMessage message;

//***************************************************************************

#define BitsPerChannel 12

union Received
{
  unsigned long Raw;

  struct
  {
    int Chan2 : BitsPerChannel;
    int Chan1 : BitsPerChannel;
    int unused : (32 - 2 * BitsPerChannel);
  } Formatted;
};

//***************************************************************************

void setup() 
{
    Serial.begin (115200);

    pinMode (LoadSR,  OUTPUT);
    pinMode (ShiftSR, OUTPUT);
    pinMode (ReadSR,  INPUT);
    
    digitalWrite (LoadSR, 0);
    digitalWrite (ShiftSR, 0);
    

    char str [20];
    sprintf (str, "sizeof (Received) = %d", sizeof (Received));
    Serial.println (str);

    //return;

    
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
}

//***************************************************************************

int D = 1;  // milliseconds

void loop () 
{
    char str [20];
    
    digitalWrite (LoadSR, 1);
    delay (D);     // milliseconds
    
    digitalWrite (LoadSR, 0);
    delay (D);    

    int chan1 = ReadChannel ();
    int chan2 = ReadChannel ();

/***
    sprintf (str, "chan 1 %x, %d", chan1, chan1);
    Serial.println (str);
    
    sprintf (str, "chan 2 %x, %d", chan2, chan2);
    Serial.println (str);
    
    Serial.println ("-------------------");
***/

    message.SetTime (millis ());
    message.SetShaft1 (chan1);
    message.SetShaft2 (chan2);
    
    socket.write ((char *) &message, message.header.ByteCount);  
    
    delay (100);    
}

int ReadChannel ()
{
    int value = 0;
    
    for (int i=0; i<BitsPerChannel; i++)
    {
        int bit = digitalRead (ReadSR) & 1;
        bit <<= (15 - i);
        value |= bit;
        
        digitalWrite (ShiftSR, 1);
        delay (D);   
        
        digitalWrite (ShiftSR, 0);
        delay (D);    
    }

    value >>= (16 - BitsPerChannel);
    
    return value;
}
