//
// AcknowledgeMsg_Auto.h
//   - auto generated code

#ifndef ACKNOWLEDGEMSG_AUTO_H
#define ACKNOWLEDGEMSG_AUTO_H

#include <Arduino.h>
#include "MessageIDs.h"
#include "MessageHeader.h"

class AcknowledgeMsg_Auto
{
    public:
        struct Data
        {

            unsigned short MsgSequenceNumber;
        };

		AcknowledgeMsg_Auto ();
		AcknowledgeMsg_Auto (byte *msgBytes);
		void ToBytes    (byte *byteArray);
		void ToConsole ();

		MessageHeader header;
		Data          data;
};
#endif
