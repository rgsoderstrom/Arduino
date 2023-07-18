
//
// MessagesIDs.h
//

#ifndef MESSAGES_IDS_H
#define MESSAGES_IDS_H

//
// Message IDs
//

// Arduino -> PC
#define AcknowledgeMsgId   1
#define TextMsgId          2
#define StatusMsgId        3

// PC -> Arduino
#define KeepAliveMsgId            1  // no action required, just keeps socket open
#define StatusRequestMsgId        2

#define LoopbackDataMsgId         9
#define RunLoopbackTestMsgId      10 // command to run the loopback test
#define SendLoopbackDataMsgId     11 // command to send data read back from FPGA

//#define DisconnectMsgId          99

#endif
