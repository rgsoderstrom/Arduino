
//
// MessagesUT
//

#include "StatusMessage.h"

StatusMessage msg1;

void setup() 
{
    Serial.begin (9600);
    Serial.println ("MessagesUT");

    msg1.SetName ("abcdefghijk");
    msg1.SetDataReady (true);
    
    msg1.ToConsole ();
    
    byte messageBytes [sizeof (StatusMessage)]; // size must be known at compile time
    msg1.ToBytes (messageBytes);
  
    for (int i=0; i<msg1.ByteCount (); i++)
    {
        Serial.print (i);
        Serial.print (": ");
        Serial.println (messageBytes [i], HEX);
    }
      
    StatusMessage *msg2 = new StatusMessage (messageBytes);
    msg2->ToConsole ();
    delete msg2;
}

void loop() 
{


}
