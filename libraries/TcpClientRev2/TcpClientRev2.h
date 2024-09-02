
//
// TcpClientRev2.h - Wrapper for socket functions
//                   to provide TCP client interface
//

//
// This version for Arduino Uno WIFI Rev 2 board
//

#ifndef TCPCLIENTREV2_H
#define TCPCLIENTREV2_H

#include <SPI.h>
#include <WiFiNINA.h>

#include <MessageHeader.h>

// for messages received from PC
#define ByteBufferSize 128

//**********************************************************************************

class TcpClientRev2
{
    public:
        TcpClientRev2 ();
        
        bool IsConnected () {return client.connected ();}

        void write (char *bytes, unsigned int byteCount);

      // return complete message, including header
        bool checkForMessages (char *bytes, int maxByteCount); 
              
        void close ();
        void disconnect ();

    private:
        WiFiClient client;
        
        static bool ConnectToNetwork (const char *, const char *);
		
        bool ConnectToServer (const char *);
        bool ConnectToServer (const char *, int);
        bool ConnectToServer (IPAddress&, int);
		
        char byteBuffer [ByteBufferSize]; // at least as large as longest incoming message
        unsigned int put;
        
        int  read  (uint8_t*, unsigned int maxByteCount);
};

#endif
