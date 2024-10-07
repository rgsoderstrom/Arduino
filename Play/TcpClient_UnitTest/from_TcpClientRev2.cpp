
//
// from_TcpClientRev2.cpp 
//

#include <Arduino.h>
#include <stdlib.h>
#include "from_TcpClientRev2.h"

// messages
SocketBytes from_TcpClientRev2::M1 = {{0x34, 0x12, 8, 0, 0x11, 0x22, 0x33, 0x44}, 8};
SocketBytes from_TcpClientRev2::M2 = {{0x34, 0x12, 8, 0, 0x55, 0x66, 0x77, 0x88}, 8};
SocketBytes from_TcpClientRev2::M3 = {{0x34, 0x12, 8, 0, 0x99, 0xaa, 0xbb, 0xcc}, 8};

// trash - to test re-sync
SocketBytes from_TcpClientRev2::T1 = {{0xab, 0xcd, 0xef}, 3};

SocketBytes *from_TcpClientRev2::MsgSequence [] = {&M1, &T1, &M2, &M3};

char     *ByteSequence;     // all message and trash bytes concatenated
int       ByteCount    = 0; // number of bytes in ByteSequence
const int BytesPerRead = 16;
int       ByteGet      = 0; // index into ByteSequence

//************************************************************************************

from_TcpClientRev2::from_TcpClientRev2 ()
{
	put = 0;

  // count the total number of socket bytes
  for (int i=0; i<sizeof (from_TcpClientRev2::MsgSequence) / sizeof (from_TcpClientRev2::MsgSequence [0]); i++)
    ByteCount += MsgSequence [i]->count;

  // concatenate then
  ByteSequence = malloc (ByteCount);
  
  if (ByteSequence == NULL)
    Serial.println ("malloc failed");

  char *bytePut = ByteSequence;

  for (int i=0; i<sizeof (from_TcpClientRev2::MsgSequence) / sizeof (from_TcpClientRev2::MsgSequence [0]); i++)
  {
      for (int j=0; j<MsgSequence [i]->count; j++)
          *bytePut++ = MsgSequence [i]->bytes [j];
  }
  
}

//************************************************************************************

bool from_TcpClientRev2::checkForMessages (char *messageBytes, int maxByteCount)
{
    if (1) // client.available ())
        put += read ((uint8_t *) &byteBuffer [put], ByteBufferSize - put);        
 //
 // see if we have a complete message
 //
    if (put < sizeof (MessageHeader)) // if not enough bytes then return
        return false;
       
 // find sync word
    int get;
    for (get=0; get + sizeof (MessageHeader) <= put; get++)
    {
        MessageHeader *header = (MessageHeader *) &byteBuffer [get];

        if (header->Sync == SyncWord)
        {
         // do we have a complete message?
           int bytesBuffered = put - get;

           if (bytesBuffered < header->ByteCount)
                return false; // found a header but have not received entire message
           
         // have a full message. copy bytes to caller's buffer
            int count = header->ByteCount < maxByteCount ? header->ByteCount : maxByteCount;

            for (int i=0; i<count; i++)
                messageBytes [i] = byteBuffer [get++];

         // move any remaining bytes to start
            int bytesRemaining = put - get;

            for (int i=0; i<bytesRemaining; i++)
                byteBuffer [i] = byteBuffer [get + i];

            put = bytesRemaining;
            return true;
        }
    }

  // if we get here no sync word found. clear buffer and return
    put = 0;
    return false;
}

//*************************************************************************************************
int from_TcpClientRev2::read (uint8_t* bytes, unsigned int maxByteCount)
{
    if (ByteGet == ByteCount)
      return 0;  // all bytes sent

    int available = ByteCount - ByteGet;
    int count = available < BytesPerRead ? available : BytesPerRead;

    for (int i=0; i<count; i++)
      bytes [i] = ByteSequence [ByteGet++];

    return count;
}

//**************************************************************************************************
