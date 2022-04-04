
//
// Messages.cpp
//

#include <Arduino.h>
#include <stddef.h>
#include <string.h>

#include "Messages.h"

//***********************************************************************************
//***********************************************************************************
//***********************************************************************************

SpeedProfileSegmentMsg::SpeedProfileSegmentMsg ()
{
   header.MsgId     = SpeedProfileSegmentMsgId;
   header.ByteCount = sizeof (*this);  
}

SpeedProfileSegmentMsg::SpeedProfileSegmentMsg (byte msgBytes []) : header (msgBytes)
{
    int get = offsetof (SpeedProfileSegmentMsg, data);
    
    data.index    = (msgBytes [get + 1] << 8) | msgBytes [get]; get += 2;    
    data.motorID  = (msgBytes [get + 1] << 8) | msgBytes [get]; get += 2;    
    data.speed    = (msgBytes [get + 1] << 8) | msgBytes [get]; get += 2;    
    data.duration = (msgBytes [get + 1] << 8) | msgBytes [get];
}

//***********************************************************************************

bool SpeedProfileSegmentMsg::ToBytes (byte byteArray [], int maxLength)
{
    if (maxLength < sizeof (SpeedProfileSegmentMsg))
        return false;

    header.ToBytes (byteArray, maxLength);
    
    int put = sizeof (MessageHeader);
    
    byteArray [put++] = data.index;         // 0 -> (MaxNumberSegments - 1)
    byteArray [put++] = data.index >> 8;
    byteArray [put++] = data.motorID;       // 1 or 2, left or right
    byteArray [put++] = data.motorID >> 8;
    byteArray [put++] = data.speed;         // -15 -> 15
    byteArray [put++] = data.speed >> 8;
    byteArray [put++] = data.duration;      // tenths of second, 0 -> 25.5
    byteArray [put++] = data.duration >> 8; 
    
    return true;
}

//***********************************************************************************
//
// prints to console, not a character array
// 
void SpeedProfileSegmentMsg::ToString ()
{
    header.ToString ();
    Serial.print ("Index    = ");  Serial.println (data.index); //, HEX);
    Serial.print ("MotorID  = ");  Serial.println (data.motorID); //, HEX);
    Serial.print ("Speed    = ");  Serial.println (data.speed); //, HEX);
    Serial.print ("Duration = ");  Serial.println (data.duration); //, HEX);    
}

//***********************************************************************************
//***********************************************************************************
//***********************************************************************************

//
// Arduino to PC
//

TextMessage::TextMessage (char txt [])
{
    int charCount = strlen (txt);
    int copyCount = (charCount < TextBufferSize ? charCount : TextBufferSize);
    
    header.MsgId = TextMsgId;
    header.ByteCount = sizeof (header) + copyCount; 

    strncpy (text, txt, copyCount);  
}

//***********************************************************************************
//***********************************************************************************
//***********************************************************************************

AcknowledgeMessage::AcknowledgeMessage (unsigned short seqNumber)
{
    header.MsgId = AcknowledgeMsgId;
    header.ByteCount = sizeof (*this);

    data.MsgSequenceNumber = seqNumber;
}

//***********************************************************************************

AcknowledgeMessage::AcknowledgeMessage (byte msgBytes []): header (msgBytes)
{
    int get = offsetof (AcknowledgeMessage, data);
    data.MsgSequenceNumber = (msgBytes [get + 1] << 8) | msgBytes [get];
}

//***********************************************************************************

bool AcknowledgeMessage::ToBytes  (byte byteArray [], int maxLength)
{
    if (maxLength < sizeof (AcknowledgeMessage))
        return false;

    header.ToBytes (byteArray, maxLength);
    
    int put = sizeof (MessageHeader);
    
    byteArray [put++] = data.MsgSequenceNumber;
    byteArray [put++] = data.MsgSequenceNumber >> 8;

    return true;
}
   
//***********************************************************************************

void AcknowledgeMessage::ToString ()  // prints to console, not a character array
{
    header.ToString ();
    Serial.print ("MsgSequenceNumber = ");  Serial.println (data.MsgSequenceNumber); //, HEX);
}

//***********************************************************************************
//***********************************************************************************
//***********************************************************************************

StatusMessage::StatusMessage (StatusData& source)
{
    header.MsgId = StatusMsgId;
    header.ByteCount = sizeof (*this);

    strncpy (data.name, source.name, StatusData::MaxNameLength);
    data.readyForMessages = source.readyForMessages;
    data.readyToRun       = source.readyToRun; 
    data.motorsRunning    = source.motorsRunning;
    data.readyToSend      = source.readyToSend;
}

StatusMessage::StatusMessage (StatusData *source)
{
    header.MsgId = StatusMsgId;
    header.ByteCount = sizeof (*this);

    strncpy (data.name, source->name, StatusData::MaxNameLength);
    data.readyForMessages = source->readyForMessages;
    data.readyToRun       = source->readyToRun; 
    data.motorsRunning    = source->motorsRunning;
    data.readyToSend      = source->readyToSend;
}

StatusMessage::StatusData::StatusData ()
{
    memset (this, 0, sizeof (*this));
}

StatusMessage::StatusData::StatusData (char _name [])
{
    strncpy (name, _name, StatusData::MaxNameLength);
    readyForMessages = 0;
    readyToRun       = 0; 
    motorsRunning    = 0;
    readyToSend      = 0;
}

StatusMessage::StatusMessage (byte msgBytes []) : header (msgBytes)
{
    int get = offsetof (StatusMessage, data);
    
    strncpy (data.name, &msgBytes [get], StatusData::MaxNameLength);
    get += StatusData::MaxNameLength;
    
    data.readyForMessages = (msgBytes [get + 1] << 8) | msgBytes [get]; get += 2;
    data.readyToRun       = (msgBytes [get + 1] << 8) | msgBytes [get]; get += 2;
    data.motorsRunning    = (msgBytes [get + 1] << 8) | msgBytes [get]; get += 2;
    data.readyToSend      = (msgBytes [get + 1] << 8) | msgBytes [get]; get += 2;
}

bool StatusMessage::ToBytes  (byte byteArray [], int maxLength)
{
    if (maxLength < sizeof (StatusMessage))
        return false;

    header.ToBytes (byteArray, maxLength);
    
    int put = sizeof (MessageHeader);

    for (int i=0; i<StatusData::MaxNameLength; i++)
      byteArray [put++] = data.name [i];
    
    byteArray [put++] = data.readyForMessages;
    byteArray [put++] = data.readyForMessages >> 8;
    byteArray [put++] = data.readyToRun;
    byteArray [put++] = data.readyToRun >> 8;
    byteArray [put++] = data.motorsRunning;
    byteArray [put++] = data.motorsRunning >> 8;
    byteArray [put++] = data.readyToSend;
    byteArray [put++] = data.readyToSend >> 8;

    return true;
}
  
void StatusMessage::ToString () // prints to console, not a character array
{
    header.ToString ();
    Serial.print ("name = ");             Serial.println (data.name); //, HEX);
    Serial.print ("readyForMessages = "); Serial.println (data.readyForMessages); //, HEX);
    Serial.print ("readyToRun = ");       Serial.println (data.readyToRun); //, HEX);
    Serial.print ("motorsRunning = ");    Serial.println (data.motorsRunning); //, HEX);
    Serial.print ("readyToSend = ");      Serial.println (data.readyToSend); //, HEX);
}

//***********************************************************************************
//***********************************************************************************
//***********************************************************************************

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
