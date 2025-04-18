
//
// TcpClientRev2.cpp - Wrapper for socket functions
//                     to provide TCP client interface
//

//
// This version for Arduino Uno WIFI Rev 2 board
//

#include "TcpClientRev2.h"

#define MaxAttempts     5


//**************************************************************************************************

// ctor

TcpClientRev2::TcpClientRev2 ()
{
}

//**************************************************************************************************

bool TcpClientRev2::ConnectToNetwork (const char *ssid, const char *passwd)
{
  int status = WL_IDLE_STATUS;
  int count = 0;
  
  while (1)
  {
    Serial.print ("Attempting to connect to network: ");
    Serial.println (ssid);

    status = WiFi.begin (ssid, passwd);

    if (status == WL_CONNECTED)
      break;

    if (++count == MaxAttempts)
      break;
      
    delay (3000);
  }

  if (status == WL_CONNECTED)
    WiFi.setDNS (IPAddress (192, 168, 1, 1));  // router
    
  return (status == WL_CONNECTED);
}

//**************************************************************************************************

bool TcpClientRev2::ConnectToServer (const char *server, int port)
{
  //IPAddress addr = getHostByName (server);

  
  return client.connect (server, port);
}

bool TcpClientRev2::ConnectToServer (IPAddress& server, int port)
{
  return client.connect (server, port);
}

//**************************************************************************************************

void TcpClientRev2::write (char* bytes, unsigned int byteCount)
{
    client.write (bytes, byteCount);
}

//**************************************************************************************************

// LIMITATIONS:
//      - this code assumes entire message fits in local buffer
//      - should be called faster than message rate, if several messages 
//        pile up in buffer last one may not fit

bool TcpClientRev2::checkForMessages (char *messageBytes, int maxByteCount)
{
    if (client.available ())
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

//**************************************************************************************************

int TcpClientRev2::read (uint8_t* bytes, unsigned int maxByteCount)
{
    int count = 0;
    
    noInterrupts ();
    count = client.read (bytes, maxByteCount);
    interrupts ();

    return count;
}

//**************************************************************************************************

void TcpClientRev2::close ()
{
   // WiFi.end ();
   client.stop (); //.close ();
}

//**************************************************************************************************

void TcpClientRev2::disconnect ()
{
    client.stop ();
}
