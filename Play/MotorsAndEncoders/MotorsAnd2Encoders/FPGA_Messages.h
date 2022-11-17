
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

#define ClearProfileMsgID 1
#define LoadProfileMsgID  2
#define RunProfileMsgID   3
#define StopProfileMsgID  4

#define fpgaStartCollectionMsgID 5
#define fpgaStopCollectionMsgID  6
#define BuildCollMsgID       7
#define SendCollMsgID        8

#define ByteCountOffset 2

union ProfileMessage
{
    ProfileMessage ();
    void Clear ();
    int  GetByteCount () {return Fields.byteCount;}
    
    static const int NumbMotors = 2;
    static const int MaxNumbSegments = 5;
    static const int NumbParameters = 2; // one byte for each of speed & duration

    //
    // member data
    //
    byte ByteStream [1];

    struct
    {
        byte sync;
        byte id;
        byte byteCount;
        byte profile [MaxNumbSegments][NumbMotors][NumbParameters];
    } Fields; 
};

union HeaderMessage
{
    HeaderMessage (byte id);
    int GetByteCount () {return Fields.byteCount;}
	  unsigned char *GetBytePtr () {return ByteStream;}

    //
    // member data
    //
    byte ByteStream [1];

    struct
    {
        byte sync;
        byte id;
        byte byteCount;
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
	  static const int MaxSamplesPerEncoder = 16;
    static const int MaxSamplesPerMessage = NumbEncoders * MaxSamplesPerEncoder;

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

        unsigned int  remaining; // includes those in this message
		
        unsigned char samples [MaxSamplesPerEncoder][NumbEncoders];
    } Fields; 
};



#endif
