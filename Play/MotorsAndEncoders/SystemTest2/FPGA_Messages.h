
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

//
// FPGA to Arduino
//


#endif
