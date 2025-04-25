
//
// MessagesIDs.h
//

#ifndef MESSAGES_IDS_H
#define MESSAGES_IDS_H

//
// Message IDs
//

// Arduino -> PC
#define AcknowledgeMsgId 10
#define TextMsgId         8
#define ReadyMsgId      100

// PC -> Arduino
#define KeepAliveMsgId    1  // no action required, just keeps socket open



#define  BeginPingCycleMsgId 99
#define  AnalogGainMsgId     99
#define  BeginSamplingMsgId  99
#define  ClearSamplesMsgId   99
#define  PingReturnDataMsgId 99
#define  SampleDataMsgId     99
#define SampleRateMsgId      99
#define SonarParametersMsgId 99
#define SendSamplesMsgId 99

#endif
