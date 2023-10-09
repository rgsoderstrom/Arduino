//
// StatusMsg_Auto.h
//   - auto generated code

#ifndef STATUSMSG_AUTO_H
#define STATUSMSG_AUTO_H

#include <Arduino.h>
#include "MessageIDs.h"
#include "MessageHeader.h"

class StatusMsg_Auto
{
    public:
        struct Data
        {
            static const int MaxNameLength = 18;

            char name [Data::MaxNameLength];
            byte DataReceived;
            byte DataReady;
        };

		StatusMsg_Auto ();
		StatusMsg_Auto (byte *msgBytes);
		void ToBytes    (byte *byteArray);
		void ToConsole ();

		MessageHeader header;
		Data          data;
};
#endif
