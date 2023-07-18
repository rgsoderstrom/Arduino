//
// auto-generated code for message LoopbackDataMsg
//

#include "LoopbackDataMessage.h"

LoopbackDataMsg::LoopbackDataMsg (byte *msgBytes)
{
    memset (this, 0, sizeof (LoopbackDataMsg));

    header.Sync           = (msgBytes [1] << 8) | msgBytes [0];
    header.ByteCount      = (msgBytes [3] << 8) | msgBytes [2];
    header.MsgId          = (msgBytes [5] << 8) | msgBytes [4];
    header.SequenceNumber = (msgBytes [7] << 8) | msgBytes [6];

    int get = 8;

    data.source = msgBytes [get]; get += 1;

    data.dataByteCount = msgBytes [get]; get += 1;

    for (int i=0; i<Data::MaxCount; i++)
    {
         data.dataBytes [i] = msgBytes [get]; get += 1;
    }

}

void LoopbackDataMsg::ToBytes (byte *byteArray)
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

    byteArray [put++] = data.source;

    byteArray [put++] = data.dataByteCount;

    for (int i=0; i<Data::MaxCount; i++)
    {
        byteArray [put++] = data.dataBytes [i];
    }

}
