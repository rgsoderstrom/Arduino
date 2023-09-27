
//
// FPGA_MsgBytes.cpp
//    - buffer for bytes received from FPGA. 
//    - begins storing when SyncByte received
//    - indicates when it has a complete message
//

#include "FPGA_MsgBytes.h"

unsigned char FPGA_MsgBytes::SyncByte1 = 0x34; // sync pattern = 0x1234
unsigned char FPGA_MsgBytes::SyncByte2 = 0x12;
unsigned char FPGA_MsgBytes::HeaderSize = 8;

FPGA_MsgBytes::FPGA_MsgBytes ()
{
	Clear ();
}

void FPGA_MsgBytes::Clear ()
{
	ByteBuffer.Packed.Put = 0;
	state = WaitingForSync1;
}

FPGA_MsgBytes::BufferState FPGA_MsgBytes::StoreByte (unsigned char newByte)
{
	// short aliases for ByteBuffer members
	int& put = ByteBuffer.Packed.Put;
	unsigned char *byteArray = ByteBuffer.Packed.ByteArray;
	
	switch (state)
	{
		case WaitingForSync1: 
			if (newByte == SyncByte1)
			{
				byteArray [0] = newByte;
				put = 1;
				state = VerifySyncByte2;
			}
			break;

		case VerifySyncByte2: 
			if (newByte == SyncByte2)
			{
				byteArray [put++] = newByte;
				state = InHeader;
			}
			else
			{
				state = WaitingForSync1;
			}
			break;
		
		case InHeader: 
			byteArray [put++] = newByte;
			
			// don't look at header content until we have the whole thing
			if (put == HeaderSize)
			{
				if (GetByteCount () == HeaderSize)
				    state = MsgComplete;
				else
					state = InMsgData;			
			}
			
			break;
					
		case InMsgData: 
			if (put < BufferSize) 
				byteArray [put++] = newByte;
			else
				state = WaitingForSync1;
			
			if (put == GetByteCount ())
				state = MsgComplete;
			
			break;
			
		case MsgComplete: // stays here until Clear () invoked
			break;
			
		default:
			state = WaitingForSync1;
			break;
	}
	return state;
}

