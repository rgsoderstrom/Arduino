
//
// Messages.h
//

#ifndef ACK_MESSAGE_H
#define ACK_MESSAGE_H

#include <Arduino.h>
#include "MessageIDs.h"
#include "MessageHeader.h"

class AcknowledgeMessage
{
    struct AckData
    {
        unsigned short MsgSequenceNumber;
    };

  public:
    AcknowledgeMessage (unsigned short seqNumber);
    AcknowledgeMessage (byte msgBytes []);

    void ToConsole ();
    void ToBytes (byte *);
    int ByteCount () {return header.ByteCount;}
    
  private:
    MessageHeader  header;
    AckData        data;
};

#endif
