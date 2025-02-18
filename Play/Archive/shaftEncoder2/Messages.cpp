
//
// Messages.cpp
//

#include "Messages.h"

//***********************************************************************************************

EncoderCountsMessage::EncoderCountsMessage ()
{
    header.ByteCount = sizeof (EncoderCountsMessage); 
    header.MsgId = ShaftEncCountMsgId;
}

void EncoderCountsMessage::SetTime (unsigned long time)
{
    counts.timeTag = time;  
}
    
void EncoderCountsMessage::SetShaft1 (int p1)
{
    counts.Shaft1 = p1;
}

void EncoderCountsMessage::SetShaft2 (int p2)
{
    counts.Shaft2 = p2;
}
