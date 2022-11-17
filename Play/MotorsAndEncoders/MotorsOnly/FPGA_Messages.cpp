
//
// FPGA_Messages.cpp
//

#include <FPGA_MsgBytes.h>
#include "FPGA_Messages.h"

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
