
//
// Encoders.cpp
//

#include <Arduino.h>

#include "Messages.h"
#include "Encoders.h"

Encoders::Encoders ()
{
  
}

bool Encoders::Setup ()
{
    pinMode (LoadSR,  OUTPUT);
    pinMode (ShiftSR, OUTPUT);
    pinMode (ReadSR,  INPUT);
    
    digitalWrite (LoadSR, 0);
    digitalWrite (ShiftSR, 0);
}

void Encoders::Loop (TcpClientRev2 socket)
{
    char str [20];
    
    digitalWrite (LoadSR, 1);    
    digitalWrite (LoadSR, 0);

    int chan1 = ReadChannel ();
    int chan2 = ReadChannel ();

    Channel1 += chan1;
    Channel2 += chan2;


    /**/
    if (abs (Channel1) > 10 || abs (Channel2) > 10)
    {
        message.SetTime (millis ());
        message.SetShaft1 (Channel1);
        message.SetShaft2 (Channel2);

        Channel1 = Channel2 = 0;
        
        socket.write ((char *) &message, message.header.ByteCount);  
        message.header.IncrSequenceNumber ();
    } /**/
}

int Encoders::ReadChannel ()
{
    int value = 0;
    
    for (int i=0; i<BitsPerChannel; i++)
    {
        int bit = digitalRead (ReadSR) & 1;
        bit <<= (15 - i);
        value |= bit;
        
        digitalWrite (ShiftSR, 1);
        digitalWrite (ShiftSR, 0);
    }

    value >>= (16 - BitsPerChannel);
    
    return value;
}
