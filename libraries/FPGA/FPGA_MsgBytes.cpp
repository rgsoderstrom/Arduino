
//
// FPGA_MsgBytes.cpp
//    - buffer for bytes received from FPGA. 
//    - begins storing when SyncByte received
//    - indicates when it has a complete message
//

#include "FPGA_MsgBytes.h"

unsigned char FPGA_MsgBytes::SyncByte = 0xAB;
unsigned int  FPGA_MsgBytes::ByteCountOffset = 2;

FPGA_MsgBytes::FPGA_MsgBytes ()
{
	Clear ();
}

void FPGA_MsgBytes::Clear ()
{
	put = 0;
	state = WaitingForSync;
	byteBuffer [ByteCountOffset] = 255;
}

FPGA_MsgBytes::BufferState FPGA_MsgBytes::StoreByte (unsigned char newByte)
{
	switch (state)
	{
		case WaitingForSync: 
			if (newByte == SyncByte)
			{
				byteBuffer [0] = newByte;
				put = 1;
				state = InMessage;
			}
			break;
		
		case InMessage: 
			if (put < BufferSize) 
				byteBuffer [put++] = newByte;
			
			if (put == byteBuffer [ByteCountOffset])
				state = MsgComplete;
			
			break;
			
		case MsgComplete:
			break;
			
		default:
			break;
	}
	
	return state;
}

unsigned char FPGA_MsgBytes::GetBytes ()
{
	return byteBuffer;
}
