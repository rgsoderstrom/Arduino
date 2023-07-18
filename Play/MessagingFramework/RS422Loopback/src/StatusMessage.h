
//
// StatusMessage.h
//

#ifndef STATUS_MESSAGE_H
#define STATUS_MESSAGE_H

#include <Arduino.h>

#include <MessageHeader.h>
#include "MessageIDs.h"

class StatusMessage
{
    private:
        struct StatusData
        {
            static const int MaxNameLength = 18;

            char name [MaxNameLength];
            byte DataReceived;
            byte DataReady;
        };

   public:
        StatusMessage ();
        StatusMessage (byte msgBytes []);        
        void ToBytes (byte *);        
        void ToConsole ();        
        int ByteCount () {return header.ByteCount;}

        void  SetName (char *);
        char *GetName ();
        
        void SetDataReceived (bool s);
        bool GetDataReceived ();
        
        void SetDataReady (bool s);
        bool GetDataReady ();
         
    private:
        MessageHeader header;
        StatusData    data;
};

#endif
