
//
// MessagesIDs.h
//

#ifndef MESSAGES_IDS_H
#define MESSAGES_IDS_H

//
// Message IDs
//

// Common to almost all applications
#define KeepAliveMsgId   1
#define TextMsgId        8
#define AcknowledgeMsgId 10


// Arduino -> PC
#define SampleDataMsgId 200
#define ReadyMsgId      201
#define AllSentMsgId    202 


// PC -> Arduino
#define ClearMsgId      100
#define CollectMsgId    101
#define SendMsgId       102
#define AnalogGainMsgId 103
#define SampleRateMsgId 104

#endif



    
