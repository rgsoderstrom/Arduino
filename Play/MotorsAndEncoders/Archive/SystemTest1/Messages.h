
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
#define TextMsgId             2
#define BufferStatusMsgId     3
#define CollectionDataMsgId   4
#define CollSendCompleteMsgId 5

// PC -> Arduino
#define KeepAliveMsgId       0
#define MotorSpeedMsgId      1
#define StartCollectionMsgId 2
#define StopCollectionMsgId  3
#define ClearCollectionMsgId 4
#define SendFirstCollectionMsgId 5
#define SendNextCollectionMsgId  6
#define DisconnectMsgId      8

//*********************************************************************************
//*********************************************************************************
//*********************************************************************************

// PC to Arduino

struct MotorSpeedMsg
{
    MessageHeader header;
    int           Velocity1;  // signed motor speed, -15 -> 15
    int           Velocity2;
};

//*********************************************************************************
//*********************************************************************************
//*********************************************************************************

// Arduino to PC

#define TextBufferSize 50

struct TextMessage
{
    TextMessage (char txt []);
    MessageHeader header;
    char          text [TextBufferSize];
};

//*********************************************************************************

struct BufferStatusMessage 
{
    BufferStatusMessage (int a);

    MessageHeader header;
    char          data; // 0 => empty, 1 => full
};

//*********************************************************************************

#define MsgBufferSize 10

struct EncoderCounts
{
    unsigned long timeTag;
    int enc1;
    int enc2;  
    char s1;
    char s2;
};

struct Data
{
  int put;
  EncoderCounts counts [MsgBufferSize];
};

struct CollectionDataMessage
{
  // methods
    CollectionDataMessage ();

    bool Full  () {return data.put == MsgBufferSize;}
    bool Empty () {return data.put == 0;}
    
    void Clear ();
    bool Add (unsigned long tt, int enc1, int enc2, char s1, char s2);
    bool Add (struct EncoderCounts ec);
    
  // member variables      
    MessageHeader header;
    Data          data;
};

//*********************************************************************************

struct CollSendCompleteMessage
{  
    CollSendCompleteMessage ();

    MessageHeader header;
};

#endif
