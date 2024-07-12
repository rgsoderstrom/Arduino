
//
// Messages.h
//

#ifndef ACK_MESSAGE_H
#define ACK_MESSAGE_H

#include "AcknowledgeMsg_Auto.h"

class AcknowledgeMsg : public AcknowledgeMsg_Auto
{
  public:  
      AcknowledgeMsg () : AcknowledgeMsg_Auto () {}
	  AcknowledgeMsg (byte *msgBytes) : AcknowledgeMsg_Auto (msgBytes) {}

	  AcknowledgeMsg (unsigned int Seq) : AcknowledgeMsg_Auto () {data.MsgSequenceNumber = Seq;}
	  int ByteCount () {return header.ByteCount;}
};

#endif
