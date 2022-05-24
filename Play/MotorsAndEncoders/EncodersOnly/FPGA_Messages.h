
//
// FPGA_Messages.h
//
//    - messages to communicate with FPGA
//

#ifndef FPGA_MESSAGES_H
#define FPGA_MESSAGES_H

#include <arduino.h>

//
// Arduino to FPGA
//

#define SyncByte         0xAB
#define ClearProfileMsgID 1
#define LoadProfileMsgID  2
#define RunProfileMsgID   3
#define StopProfileMsgID  4

#define StartCollectionMsgID 5
#define StopCollectionMsgID  6
#define BuildCollMsgID       7
#define SendCollMsgID        8



union ProfileMessage
{
    ProfileMessage ();
    void Clear ();
    int  GetByteCount () {return Fields.byteCount;}
    
    static const int NumbMotors = 2;
    static const int MaxNumbSegments = 5;
    static const int NumbParameters = 2; // one unsigned char for each of speed & duration

    //
    // member data
    //
    unsigned char ByteStream [1];

    struct
    {
        unsigned char sync;
        unsigned char id;
        unsigned char byteCount;
        unsigned char profile [MaxNumbSegments][NumbMotors][NumbParameters];
    } Fields; 
};

union HeaderMessage
{
    HeaderMessage (unsigned char id);
    int GetByteCount () {return Fields.byteCount;}
	unsigned char *GetBytePtr () {return ByteStream;}

    //
    // member data
    //
    unsigned char ByteStream [1];

    struct
    {
        unsigned char sync;
        unsigned char id;
        unsigned char byteCount;
    } Fields;
};

//**************************************************************************************

//
// FPGA to Arduino
//

union EncoderCountsMessage
{
    EncoderCountsMessage ();
    void Clear ();
    int  GetByteCount () {return Fields.byteCount;}
	unsigned char *GetBytePtr () {return ByteStream;}
    
    static const int NumbEncoders = 2;
	static const int SamplesPerEncoder = 3;

    //
    // member data
    //
    unsigned char ByteStream [1];

    struct
    {
        unsigned char sync;
        unsigned char id;
        unsigned char byteCount;
		unsigned char sequence;
		
		unsigned char remainingLow;
		unsigned char remainingHigh;
		
        unsigned char profile [SamplesPerEncoder][NumbEncoders];
    } Fields; 
};



#endif
