
//#include "Messages.h"
//
//EncoderCountsMessage::EncoderCountsMessage ()
//{
//    header.MsgId = EncoderCountsMsgId;
//    header.ByteCount = sizeof (*this);
//    Clear ();
//}
//
//EncoderCountsMessage::EncoderCountsMessage (byte msgBytes []) : header (msgBytes)
//{
//    int get = offsetof (EncoderCountsMessage, data);
//
//    data.put       = (msgBytes [get + 1] << 8) | msgBytes [get]; get += 2;
//    data.lastBatch = (msgBytes [get + 1] << 8) | msgBytes [get]; get += 2;
//    
//    for (int i=0; i<Batch::MaxNumberSamples; i++)
//    {
//        data.counts [i].enc1 = msgBytes [get++];
//        data.counts [i].enc2 = msgBytes [get++];
//    }
//}
//
//bool EncoderCountsMessage::Add (byte e1, byte e2)
//{
//    if (data.put < Batch::MaxNumberSamples)
//    {
//        data.counts [data.put].enc1 = e1;
//        data.counts [data.put].enc2 = e2;
//        data.put++;
//    }
//
//    return (data.put < Batch::MaxNumberSamples); // false when no more room
//}
//
//bool EncoderCountsMessage::ToBytes  (byte byteArray [], int maxLength)
//{
//    if (maxLength < sizeof (StatusMessage))
//        return false;
//
//    header.ToBytes (byteArray, maxLength);
//    
//    int put = sizeof (MessageHeader);
//
//    byteArray [put++] = data.put;
//    byteArray [put++] = data.put >> 8;
//    byteArray [put++] = data.lastBatch;
//    byteArray [put++] = data.lastBatch >> 8;
//
//    for (int i=0; i<Batch::MaxNumberSamples; i++)
//    {
//        byteArray [put++] = data.counts [i].enc1;
//        byteArray [put++] = data.counts [i].enc2;
//    }
//
//    return true;
//}
//  
//void EncoderCountsMessage::ToString () // prints to console, not a character array
//{
//    header.ToString ();
//    Serial.print ("put = "); Serial.println (data.put);
//    Serial.print ("last = "); Serial.println (data.lastBatch);
//
//    for (int i = 0; i<Batch::MaxNumberSamples; i++)
//    {
//        Serial.print   (data.counts [i].enc1); 
//        Serial.print   (", "); 
//        Serial.println (data.counts [i].enc2);
//    }
//}
