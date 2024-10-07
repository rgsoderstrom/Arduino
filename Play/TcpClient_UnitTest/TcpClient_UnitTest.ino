//
// TcpClient_UnitTest.ino 
//    - test code that builds messages from fragments
//

#include <MessageHeader.h>
#include "from_TcpClientRev2.h"

#define MaxMessageBytes 256
char messageBytes [MaxMessageBytes];

from_TcpClientRev2 *socketPtr;

void setup() 
{
    Serial.begin (9600);
    Serial.println ("============================================");
    Serial.println ("UnitTest");
    
    socketPtr = new from_TcpClientRev2 ();

    for (int j=0; j<6; j++)
    {
        bool isMessage = socketPtr->checkForMessages (messageBytes, sizeof (messageBytes));
        Serial.print ("isMessage = "); Serial.println (isMessage);
    
        if (isMessage)
        {
            MessageHeader *hdr = (MessageHeader *) messageBytes;
            int byteCount = hdr->ByteCount;
            for (int i=0; i<byteCount; i++) {Serial.print (messageBytes [i] & 0xff, HEX); Serial.print (" ");}
            Serial.println ("");        
        }
    }

    Serial.println ("Done");

    
        
}

void loop() 
{


}
