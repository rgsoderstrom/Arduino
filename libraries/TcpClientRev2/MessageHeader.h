
//
// MessageHeader.h
//

#ifndef MESSAGEHEADER_H
#define MESSAGEHEADER_H

//**********************************************************************************

// same header used both directions

// Sync word used to find start of message in byte stream
#define SyncWord ((unsigned int) 0x1234)
static unsigned int NextSequenceNumber = 1;

struct MessageHeader
{
    MessageHeader ();
	MessageHeader (unsigned char fromBytes []);
	
	bool ToBytes (unsigned char byteArray [], int maxLength);
	void ToConsole ();
	void ToString (); // prints to console, not a character array
	
	//void IncrSequenceNumber () {SequenceNumber++; NextSequenceNumber = SequenceNumber + 1;}
	
    unsigned int   Sync;
    unsigned int   ByteCount;
    unsigned int   MsgId;
    unsigned int   SequenceNumber;
};

#endif

