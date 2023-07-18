
//
// TextMessage.cpp
//

#include "MessageIDs.h"
#include "TextMessage.h"

TextMessage::TextMessage (char txt [])
{
    header.ByteCount = 48; //                 += TextData::TextBufferSize;
    header.MsgId     = TextMsgId;
    strncpy (data.text, txt, TextData::TextBufferSize - 1);
}

//TextMessage::TextMessage (byte *msgBytes)
//{
//
//}
	
void TextMessage::ToConsole ()
{
    Serial.print ("TextMessage: ");
    Serial.println (data.text);  
}
