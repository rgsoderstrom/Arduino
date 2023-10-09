
//
// MessagesUT
//

#include "src\TextMessage.h"

TextMessage msg1 ("Hello");
byte Msg1Bytes [80];

TextMessage *msg2;
byte Msg2Bytes [80];

void setup() 
{
    Serial.begin (9600);
    Serial.println ("MessagesUT");

    Serial.println ("default msg1:");
    msg1.ToConsole ();
    Serial.println ("msg1 done");
    Serial.println ("");

//    for (int i=0; i<msg1.data.MaxCount; i++)
//      msg1.data.dataWords [i] = 0x56 + i;
    
    Serial.println ("modified msg1:");
    msg1.ToConsole ();
    Serial.println ("msg1 done");
    Serial.println ("");

    msg1.ToBytes (Msg1Bytes);
    Serial.println ("msg1 bytes");
    
    for (int i=0; i<msg1.header.ByteCount; i++)
    {
        Serial.print (i);
        Serial.print (": ");
        Serial.println (Msg1Bytes [i]);
    }

    msg2 = new TextMessage (Msg1Bytes);
    Serial.println ("msg2, from msg1 bytes:");
    msg2->ToConsole ();
    Serial.println ("msg2 done");
    Serial.println ("");
    
    delete msg2;
}

void loop() 
{


}
