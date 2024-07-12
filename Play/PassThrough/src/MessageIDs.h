
//
// MessagesIDs.h
//

#ifndef MESSAGES_IDS_H
#define MESSAGES_IDS_H

//
// Message IDs
//

// Arduino -> PC
#define AcknowledgeMsgId   10
#define TextMsgId          8
//#define StatusMsgId        3

// PC -> Arduino
#define KeepAliveMsgId            1  // no action required, just keeps socket open
//#define StatusRequestMsgId        2

//#define LoopbackDataMsgId         100
//#define RunLoopbackTestMsgId      101 // command to run the loopback test
//#define SendLoopbackDataMsgId     102 // command to send data read back from FPGA

//#define DisconnectMsgId          99

#endif
