
//
// Messages.cpp
//

#include <string.h>

#include "Messages.h"

//***********************************************************************************************

TextMessage::TextMessage (char txt [])
{
    int charCount = strlen (txt);
    int copyCount = (charCount < TextBufferSize ? charCount : TextBufferSize);
    
    header.ByteCount = sizeof (header) + copyCount; 
    header.MsgId = TextMsgId;

    strncpy (text, txt, copyCount);  
    //text [copyCount - 1] = '\0'; 
}

//***********************************************************************************************

BufferStatusMessage::BufferStatusMessage (int a)
{
    header.ByteCount = sizeof (BufferStatusMessage); 
    header.MsgId = BufferStatusMsgId;
    data = (a != 0 ? 1 : 0);
}

//***********************************************************************************************

CollectionDataMessage::CollectionDataMessage ()
{
    header.ByteCount = sizeof (CollectionDataMessage); 
    header.MsgId = CollectionDataMsgId;
    Clear ();
}

void CollectionDataMessage::Clear ()
{
    data.put = 0;
}

bool CollectionDataMessage::Add (unsigned long tt, int en1, int en2, char sp1, char sp2)
{
    int& p = data.put;
    
    if (p < MsgBufferSize)
    {
        data.counts [p].timeTag = tt;
        data.counts [p].enc1 = en1;
        data.counts [p].enc2 = en2;
        data.counts [p].s1 = sp1;
        data.counts [p].s2 = sp2;
        p++;

        return true;
    }

    return false;
}

bool CollectionDataMessage::Add (struct EncoderCounts ec)
{
    int& p = data.put;
    
    if (p < MsgBufferSize)
    {
        data.counts [p] = ec;
        p++;

        return true;
    }

    return false;
}

//***********************************************************************************************

CollSendCompleteMessage::CollSendCompleteMessage ()
{
    header.ByteCount = sizeof (CollSendCompleteMessage); 
    header.MsgId = CollSendCompleteMsgId;
}
