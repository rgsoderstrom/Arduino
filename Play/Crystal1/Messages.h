
//
// Messages.h
//

#ifndef MESSAGES_H
#define MESSAGES_H

//*********************************************************************************

// Sync word
#define SyncWord ((unsigned int) 0x1234)

//
// Message IDs
//

// Arduino -> PC
#define StatusMsgId      1
#define TemperatureMsgId 2

// PC -> Arduino
#define KeepAliveMsgId      0
#define SendStatusMsgId     1
#define StartSamplingMsgId  2
#define StopSamplingMsgId   3
#define ClearHistoryMsgId   4
#define SendHistoryMsgId    5
//#define SendContinuousMsgId 6
//#define StopSendingMsgId    7
#define DisconnectMsgId     8

//*********************************************************************************

// same header used both directions

struct MessageHeader
{
    MessageHeader ();
    
    unsigned int   Sync;
    unsigned int   ByteCount;
    unsigned int   MsgId;
    unsigned int   Spare;
};

//*********************************************************************************
//*********************************************************************************
//*********************************************************************************

// PC to Arduino
struct StartSamplingMsg
{
    MessageHeader header;
    unsigned short period; // sample period in seconds
};
//*********************************************************************************

struct StopSamplingMsg
{
    MessageHeader header;
};

//*********************************************************************************

//struct SendContinuoslyMsg
//{
//    MessageHeader header;
//    unsigned short sendFlag; // 1 => send, 0 => don't send
//};

//*********************************************************************************

// Arduino to PC

#define MaxNumberTemperatures 10

struct TemperatureSample
{
    unsigned long timeTag; // millis ()
    float temperature;  
};

struct TemperatureMessage
{
  // methods
    TemperatureMessage ();
    bool Add (unsigned long time, float temperature);
    unsigned int Count () {return NumberTemperatures;}
    bool Full  () {return NumberTemperatures == MaxNumberTemperatures;}
    bool Empty () {return NumberTemperatures == 0;}
    
    void Clear () {NumberTemperatures = 0;}

  // member variables      
    MessageHeader     header;
    unsigned int      NumberTemperatures;
    TemperatureSample samples [MaxNumberTemperatures];
};

//*********************************************************************************

struct StatusMessage
{
    StatusMessage ();
    
    MessageHeader header;
    char          sampling; // non-zero => sampling in progress
    unsigned int  numberRamSamples;
};

#endif
