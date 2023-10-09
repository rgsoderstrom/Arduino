//
// auto-generated code for message StatusMsg_Auto
//

#include <Arduino.h>
#include "StatusMsg_Auto.h"

//
// Default constructor
//
StatusMsg_Auto::StatusMsg_Auto ()
{
    memset (this, 0, sizeof (StatusMsg_Auto));

    header.Sync           = 4660;
    header.ByteCount      = sizeof (header) + sizeof (data);
    header.MsgId          = StatusMsgId;
    header.SequenceNumber = NextSequenceNumber++;
}

//
// from-bytes constructor
//
StatusMsg_Auto::StatusMsg_Auto (byte *msgBytes)
{
    memset (this, 0, sizeof (StatusMsg_Auto));

    header.Sync           = (msgBytes [1] << 8) | msgBytes [0];
    header.ByteCount      = (msgBytes [3] << 8) | msgBytes [2];
    header.MsgId          = (msgBytes [5] << 8) | msgBytes [4];
    header.SequenceNumber = (msgBytes [7] << 8) | msgBytes [6];

    int get = 8;

    for (int i=0; i<Data::MaxNameLength; i++)
    {
         data.name [i] = msgBytes [get]; get += 1;
    }

    data.DataReceived = msgBytes [get]; get += 1;

    data.DataReady = msgBytes [get]; get += 1;
}

//
// member function ToBytes ()
//
void StatusMsg_Auto::ToBytes (byte *byteArray)
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

    for (int i=0; i<Data::MaxNameLength; i++)
    {
        byteArray [put++] = data.name [i];
    }

    byteArray [put++] = data.DataReceived;

    byteArray [put++] = data.DataReady;
}

//
// member function ToConsole () - write message content to serial port
//
void StatusMsg_Auto::ToConsole ()
{
    header.ToConsole ();

    for (int i=0; i<Data::MaxNameLength; i++)
    {
        Serial.print   ("name [");
        Serial.print   (i);
        Serial.print   ("] = ");
        Serial.println (data.name [i]);
    }

    Serial.print   ("DataReceived = ");
    Serial.println (data.DataReceived);

    Serial.print   ("DataReady = ");
    Serial.println (data.DataReady);
}
