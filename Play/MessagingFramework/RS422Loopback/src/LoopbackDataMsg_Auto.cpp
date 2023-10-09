//
// auto-generated code for message LoopbackDataMsg_Auto
//

#include <Arduino.h>
#include "LoopbackDataMsg_Auto.h"

//
// Default constructor
//
LoopbackDataMsg_Auto::LoopbackDataMsg_Auto ()
{
    memset (this, 0, sizeof (LoopbackDataMsg_Auto));

    header.Sync           = 4660;
    header.ByteCount      = sizeof (header) + sizeof (data);
    header.MsgId          = LoopbackDataMsgId;
    header.SequenceNumber = NextSequenceNumber++;
}

//
// from-bytes constructor
//
LoopbackDataMsg_Auto::LoopbackDataMsg_Auto (byte *msgBytes)
{
    memset (this, 0, sizeof (LoopbackDataMsg_Auto));

    header.Sync           = (msgBytes [1] << 8) | msgBytes [0];
    header.ByteCount      = (msgBytes [3] << 8) | msgBytes [2];
    header.MsgId          = (msgBytes [5] << 8) | msgBytes [4];
    header.SequenceNumber = (msgBytes [7] << 8) | msgBytes [6];

    int get = 8;

    for (int i=0; i<Data::MaxCount; i++)
    {
        *(((byte *) &data.dataWords [i]) + 0) = msgBytes [get]; get += 1;
        *(((byte *) &data.dataWords [i]) + 1) = msgBytes [get]; get += 1;
    }
}

//
// member function ToBytes ()
//
void LoopbackDataMsg_Auto::ToBytes (byte *byteArray)
{
    int put = 0;
    byteArray [put++] = header.Sync;
    byteArray [put++] = header.Sync >> 8;
    byteArray [put++] = header.ByteCount;
    byteArray [put++] = header.ByteCount >> 8;
    byteArray [put++] = header.MsgId;
    byteArray [put++] = header.MsgId >> 8;
    byteArray [put++] = header.SequenceNumber;
    byteArray [put++] = header.SequenceNumber >> 8;

    for (int i=0; i<Data::MaxCount; i++)
    {
        byteArray [put++] = data.dataWords [i];
        byteArray [put++] = data.dataWords [i] >> 8;
    }
}

//
// member function ToConsole () - write message content to serial port
//
void LoopbackDataMsg_Auto::ToConsole ()
{
    header.ToConsole ();

    for (int i=0; i<Data::MaxCount; i++)
    {
        Serial.print   ("dataWords [");
        Serial.print   (i);
        Serial.print   ("] = ");
        Serial.println (data.dataWords [i]);
    }
}
