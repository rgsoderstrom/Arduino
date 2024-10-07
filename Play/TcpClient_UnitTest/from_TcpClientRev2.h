
//
// from_TcpClientRev2.h - 
//

#ifndef from_TCPCLIENTREV2_H
#define from_TCPCLIENTREV2_H

#include <stdint.h>
#include <MessageHeader.h>

// for messages received from PC
#define ByteBufferSize 128

//**********************************************************************************

struct SocketBytes
{
    char  bytes [24]; 
    int   count;
};

class from_TcpClientRev2
{
    public:
        from_TcpClientRev2 ();
        
      // return complete message, including header
        bool checkForMessages (char *bytes, int maxByteCount); 

    private:
		
        char byteBuffer [ByteBufferSize]; // at least as large as longest incoming message
        unsigned int put;
        
        int  read  (uint8_t*, unsigned int maxByteCount);

      // messages
        static SocketBytes M1; 
        static SocketBytes M2;
        static SocketBytes M3;

      // trash in between messages
        static SocketBytes T1; 
        
        static SocketBytes *MsgSequence [];
		    //static int   N;
};

#endif
