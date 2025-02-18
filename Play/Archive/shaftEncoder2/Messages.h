
//
// Messages.h
//

#ifndef MESSAGES_H
#define MESSAGES_H

#include <MessageHeader.h>

//*********************************************************************************

//
// Message IDs
//

// Arduino -> PC
#define ShaftEncCountMsgId  2

// PC -> Arduino
#define KeepAliveMsgId      0

//*********************************************************************************
//*********************************************************************************
//*********************************************************************************

// PC to Arduino

//struct StartSamplingMsg
//{
//    MessageHeader header;
//    unsigned short period; // sample period in seconds
//};

//*********************************************************************************

//struct StopSamplingMsg
//{
//    MessageHeader header;
//};

//*********************************************************************************

// Arduino to PC

struct EncoderCounts
{
    unsigned long timeTag; // millis ()
    int Shaft1;
    int Shaft2;
};

struct EncoderCountsMessage
{
  // methods
    EncoderCountsMessage ();

    void SetTime   (unsigned long time);
    void SetShaft1 (int p1);
    void SetShaft2 (int p2);

  // member variables      
    MessageHeader     header;
    EncoderCounts     counts;
};

#endif
