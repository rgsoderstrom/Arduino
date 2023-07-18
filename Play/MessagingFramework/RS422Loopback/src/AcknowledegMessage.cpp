
//
// AcknowledgeMessage.cpp
//

#include "AcknowledgeMessage.h"

AcknowledgeMessage::AcknowledgeMessage (unsigned short seqNumber)
{
    header.MsgId = AcknowledgeMsgId;
    header.ByteCount = sizeof (*this);

    data.MsgSequenceNumber = seqNumber;
}

//***********************************************************************************

void AcknowledgeMessage::ToConsole ()
{
    header.ToString ();
    Serial.print ("MsgSequenceNumber = ");  Serial.println (data.MsgSequenceNumber); //, HEX);
}
