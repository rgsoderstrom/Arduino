
//
// LoopbackDataMessage.cpp
//

#include "LoopbackDataMessage.h"

LoopbackDataMsg::LoopbackDataMsg ()
{
    header.MsgId = LoopbackDataMsgId;
    header.ByteCount = sizeof (header) + sizeof (Data); 

    data.source = 0;
    data.dataByteCount = 0;
}

LoopbackDataMsg::LoopbackDataMsg (Data& srcData)
{
    header.MsgId = LoopbackDataMsgId;
    header.ByteCount = sizeof (header) + sizeof (Data); 

    data.source = srcData.source;
    data.dataByteCount = srcData.dataByteCount;

    for (int i=0; i<srcData.dataByteCount; i++)
      data.dataBytes [i] = srcData.dataBytes [i];
}

void LoopbackDataMsg::ToConsole ()
{
    Serial.print ("Sync ");           Serial.println (header.Sync);           
    Serial.print ("ByteCount ");      Serial.println (header.ByteCount);           
    Serial.print ("MsgId ");          Serial.println (header.MsgId);           
    Serial.print ("SequenceNumber "); Serial.println (header.SequenceNumber);           
    
    Serial.print ("source "); Serial.println (data.source);
    Serial.print ("count "); Serial.println (data.dataByteCount);

    for (int i=0; i<Data::MaxCount; i++)
    {
        Serial.print (data.dataBytes [i]); 
        Serial.print (", ");

        if ((i & 7) == 7)
          Serial.println ("");
    }
}
