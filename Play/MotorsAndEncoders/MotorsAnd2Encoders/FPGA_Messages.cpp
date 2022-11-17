
//
// FPGA_Messages.cpp
//

#include <TcpClientRev2.h>
#include <FPGA_MsgBytes.h>
#include "FPGA_Messages.h"

#include "Messages.h"

ProfileMessage::ProfileMessage ()
{
    Fields.sync      = FPGA_MsgBytes::SyncByte;
    Fields.id        = LoadProfileMsgID;
    Fields.byteCount = sizeof (ProfileMessage);
    Clear ();
}

void ProfileMessage::Clear ()
{
    int count = NumbMotors * MaxNumbSegments * NumbParameters;
    byte *ptr = &Fields.profile [0][0][0];

    for (int i=0; i<count; i++)
      *ptr++ = 0;
}

//************************************************************************

HeaderMessage::HeaderMessage (byte id)
{
    Fields.sync      = FPGA_MsgBytes::SyncByte;
    Fields.id        = id;
    Fields.byteCount = sizeof (HeaderMessage);
}

//************************************************************************
//************************************************************************
//************************************************************************

extern TcpClientRev2 socket;

EncoderCountsMessage::EncoderCountsMessage ()
{
    TextMessage msg ("encoder counts");
    socket.write ((char *) &msg, msg.ByteCount ());  

//    if (remaining > 16)
//    {
//        fpgaPtr->WriteBytes (BuildCollMsg.GetBytePtr (), BuildCollMsg.GetByteCount ());
//        fpgaPtr->WriteBytes (SendCollMsg.GetBytePtr (),  SendCollMsg.GetByteCount ());
//    }
}
