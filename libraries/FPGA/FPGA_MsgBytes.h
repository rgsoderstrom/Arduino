
//
// FPGA_MsgBytes.h 
//    - buffer for bytes received from FPGA. 
//    - begins storing when SyncByte received
//    - indicates when it has a complete message
//

#ifndef FPGA_MSGBYTES_H
#define FPGA_MSGBYTES_H


class FPGA_MsgBytes
{		
    public:
        enum BufferState {WaitingForSync, InMessage, MsgComplete};
   
        FPGA_MsgBytes ();
        void Clear ();
        BufferState StoreByte (unsigned char ch);
        unsigned char GetBytes ();
		
		static unsigned char SyncByte;
		static unsigned int ByteCountOffset;
		
    private:
        static const int BufferSize = 64;
        unsigned char byteBuffer [BufferSize];
        int put;
		
		BufferState state;
};

#endif
