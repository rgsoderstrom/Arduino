
// TextMessage.h

#ifndef TEXT_MESSAGE_H
#define TEXT_MESSAGE_H

#include <Arduino.h>

#include "MessageHeader.h"

class TextMessage
{
    public:
        struct TextData
        {
            static const int TextBufferSize = 40;
            char text [TextBufferSize];         
        };
        
        TextMessage (char txt []);
        void ToConsole ();
        void ToBytes (byte *);

        int ByteCount () {return header.ByteCount;}
    
    private:
        MessageHeader header;
        TextData      data;
};

#endif
