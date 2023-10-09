//
// TextMsg_Auto.h
//   - auto generated code

#ifndef TEXTMSG_AUTO_H
#define TEXTMSG_AUTO_H

#include <Arduino.h>
#include "MessageIDs.h"
#include "MessageHeader.h"

class TextMsg_Auto
{
    public:
        struct Data
        {
            static const int TextBufferSize = 40;

            char text [Data::TextBufferSize];
        };

		TextMsg_Auto ();
		TextMsg_Auto (byte *msgBytes);
		void ToBytes    (byte *byteArray);
		void ToConsole ();

		MessageHeader header;
		Data          data;
};
#endif
