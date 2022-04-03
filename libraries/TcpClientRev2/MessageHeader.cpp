
//
// MessageHeader.cpp
//

#include <Arduino.h>
#include "MessageHeader.h"

MessageHeader::MessageHeader ()
{
	Sync           = SyncWord;
	ByteCount      = 0;
	MsgId          = -1;
	SequenceNumber = NextSequenceNumber++;
}

MessageHeader::MessageHeader (unsigned char msgBytes [])
{    
	Sync           = (msgBytes [1] << 8) | msgBytes [0];
	ByteCount      = (msgBytes [3] << 8) | msgBytes [2];
    MsgId          = (msgBytes [5] << 8) | msgBytes [4]; 
    SequenceNumber = (msgBytes [7] << 8) | msgBytes [6];
}

bool MessageHeader::ToBytes (unsigned char byteArray [], int maxLength)
{
    if (maxLength < sizeof (MessageHeader))
        return false;
	
	byteArray [0] = Sync;
	byteArray [1] = Sync >> 8;
	byteArray [2] = ByteCount;
	byteArray [3] = ByteCount >> 8;
	byteArray [4] = MsgId;
	byteArray [5] = MsgId >> 8;
	byteArray [6] = SequenceNumber;
	byteArray [7] = SequenceNumber >> 8;
	
	return true;
}

void MessageHeader::ToString ()
{
    Serial.print ("Sync        = ");  Serial.println (Sync, HEX);
    Serial.print ("Byte Count  = ");  Serial.println (ByteCount);//, HEX);
    Serial.print ("MsgId       = ");  Serial.println (MsgId);//, HEX);
    Serial.print ("Seq. Number = ");  Serial.println (SequenceNumber);//, HEX);    
}
