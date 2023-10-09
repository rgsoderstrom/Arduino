
// TextMessage.h

#ifndef TEXT_MESSAGE_H
#define TEXT_MESSAGE_H

#include "TextMsg_Auto.h"

class TextMessage : public TextMsg_Auto
{
    public:
        TextMessage (char txt []);
        int ByteCount () {return header.ByteCount;}
};

#endif
