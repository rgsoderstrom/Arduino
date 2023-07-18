//
// auto-generated code for message AcknowledgeMessage
//

#include "AcknowledgeMessage.h"

AcknowledgeMessage::AcknowledgeMessage (byte *msgBytes)
{
    memset (this, 0, sizeof (AcknowledgeMessage));

    header.Sync           = (msgBytes [1] << 8) | msgBytes [0];
    header.ByteCount      = (msgBytes [3] << 8) | msgBytes [2];
    header.MsgId          = (msgBytes [5] << 8) | msgBytes [4];
    header.SequenceNumber = (msgBytes [7] << 8) | msgBytes [6];

    int get = 8;

    *(((byte *) &data.MsgSequenceNumber) + 0) = msgBytes [get]; get += 1;
    *(((byte *) &data.MsgSequenceNumber) + 1) = msgBytes [get]; get += 1;

}

void AcknowledgeMessage::ToBytes (byte *byteArray)
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

    byteArray [put++] = data.MsgSequenceNumber;
    byteArray [put++] = data.MsgSequenceNumber >> 8;

}
