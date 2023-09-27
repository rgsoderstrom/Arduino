
//
// FPGA_MsgBytes.h 
//    - buffer for bytes received from FPGA. 
//    - begins storing when SyncByte received
//    - indicates when it has a complete message
//

// Message Header Format: (this is the same as PC <-> Arduino messages)
//    Sync Low Byte  = 0x34
//    Sync High Byte = 0x12
//    Msg ID Low byte
//    Msg ID High byte
//    Byte Count Low byte
//    Byte Count High byte
//    Sequence Number Low byte
//    Sequence Number High byte


#ifndef FPGA_MSGBYTES_H
#define FPGA_MSGBYTES_H

class FPGA_MsgBytes
{		
    public:
        enum BufferState {WaitingForSync1,
		                  VerifySyncByte2,
						  InHeader,
		                  InMsgData, 
						  MsgComplete};
						  
        FPGA_MsgBytes ();
        void Clear ();
        BufferState StoreByte (unsigned char ch);
		
        unsigned char *GetDataBytes () {return ByteBuffer.Unpacked.MsgDataBytes;}	
		
		unsigned int   GetMessageID () {return ByteBuffer.Unpacked.MsgID;}		
		unsigned int   GetByteCount () {return ByteBuffer.Unpacked.ByteCount;}		
		unsigned int   GetSeqNumber () {return ByteBuffer.Unpacked.SequenceNumber;}
		
		
		static unsigned char SyncByte1;
		static unsigned char SyncByte2;
		static unsigned char HeaderSize;
		
    private:
		static const int BufferSize = 256;

		union msgByteUnion
		{
			struct
			{
				unsigned int  SyncPattern; // = 0x1234
				unsigned int  MsgID;
				unsigned int  ByteCount;
				unsigned int  SequenceNumber;
				unsigned char MsgDataBytes [1];
			} Unpacked;
			
			struct
			{
				unsigned char ByteArray [BufferSize];
				int Put;
			} Packed;
			
		} ByteBuffer;
				
		BufferState state;
};

#endif
