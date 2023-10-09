//
// LoopbackDataMsg_Auto.h
//   - auto generated code

#ifndef LOOPBACKDATAMSG_AUTO_H
#define LOOPBACKDATAMSG_AUTO_H

#include <Arduino.h>
#include "MessageIDs.h"
#include "MessageHeader.h"

class LoopbackDataMsg_Auto
{
    public:
        struct Data
        {
            static const unsigned int MaxCount = 12;

            int dataWords [Data::MaxCount];
        };

		LoopbackDataMsg_Auto ();
		LoopbackDataMsg_Auto (byte *msgBytes);
		void ToBytes    (byte *byteArray);
		void ToConsole ();

		MessageHeader header;
		Data          data;
};
#endif
