//
// auto-generated code for message TextMsg_Auto
//

#include <Arduino.h>
#include "TextMsg_Auto.h"

//
// Default constructor
//
TextMsg_Auto::TextMsg_Auto ()
{
    memset (this, 0, sizeof (TextMsg_Auto));

    header.Sync           = 4660;
    header.ByteCount      = sizeof (header) + sizeof (data);
    header.MsgId          = TextMsgId;
    header.SequenceNumber = NextSequenceNumber++;
}

//
// from-bytes constructor
//
TextMsg_Auto::TextMsg_Auto (byte *msgBytes)
{
    memset (this, 0, sizeof (TextMsg_Auto));

    header.Sync           = (msgBytes [1] << 8) | msgBytes [0];
    header.ByteCount      = (msgBytes [3] << 8) | msgBytes [2];
    header.MsgId          = (msgBytes [5] << 8) | msgBytes [4];
    header.SequenceNumber = (msgBytes [7] << 8) | msgBytes [6];

    int get = 8;

    for (int i=0; i<Data::TextBufferSize; i++)
    {
         data.text [i] = msgBytes [get]; get += 1;
    }
}

//
// member function ToBytes ()
//
void TextMsg_Auto::ToBytes (byte *byteArray)
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

    for (int i=0; i<Data::TextBufferSize; i++)
    {
        byteArray [put++] = data.text [i];
    }
}

//
// member function ToConsole () - write message content to serial port
//
void TextMsg_Auto::ToConsole ()
{
    header.ToConsole ();

    for (int i=0; i<Data::TextBufferSize; i++)
    {
        Serial.print   ("text [");
        Serial.print   (i);
        Serial.print   ("] = ");
        Serial.println (data.text [i]);
    }
}
