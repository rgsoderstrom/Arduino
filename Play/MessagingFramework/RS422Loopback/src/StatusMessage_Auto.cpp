//
// auto-generated code for message StatusMessage
//

#include <Arduino.h>
#include "StatusMessage.h"

StatusMessage::StatusMessage (byte *msgBytes)
{
    memset (this, 0, sizeof (StatusMessage));

    header.Sync           = (msgBytes [1] << 8) | msgBytes [0];
    header.ByteCount      = (msgBytes [3] << 8) | msgBytes [2];
    header.MsgId          = (msgBytes [5] << 8) | msgBytes [4];
    header.SequenceNumber = (msgBytes [7] << 8) | msgBytes [6];

    int get = 8;

    for (int i=0; i<data.MaxNameLength; i++)
    {
         data.name [i] = msgBytes [get]; get += 1;
    }

    data.DataReceived = msgBytes [get]; get += 1;

    data.DataReady = msgBytes [get]; get += 1;

}

void StatusMessage::ToBytes (byte *byteArray)
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

    for (int i=0; i<data.MaxNameLength; i++)
    {
        byteArray [put++] = data.name [i];
    }

    byteArray [put++] = data.DataReceived;

    byteArray [put++] = data.DataReady;

}
