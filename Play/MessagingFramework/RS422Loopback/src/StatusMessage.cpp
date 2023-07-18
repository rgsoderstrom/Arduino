
//
// StatusMessage.cpp
//

#include "StatusMessage.h"

StatusMessage::StatusMessage ()
{
    header.MsgId = StatusMsgId;
    header.ByteCount = sizeof (header) + sizeof (data);
    
    memset (&data, 0, sizeof (StatusData));
}

void  StatusMessage::SetName (char *str) {strncpy (data.name, str, data.MaxNameLength - 1);}        
char *StatusMessage::GetName () {return data.name;}
        
void StatusMessage::SetDataReceived (bool s) {data.DataReceived = (s == true ? 1 : 0);}
bool StatusMessage::GetDataReceived () {return data.DataReceived == 1 ? true : false;}

void StatusMessage::SetDataReady (bool s) {data.DataReady = (s == true ? 1 : 0);}
bool StatusMessage::GetDataReady () {return data.DataReady == 1 ? true : false;}

void StatusMessage::ToConsole ()
{
    header.ToString ();
    
    Serial.print ("Status for: "); Serial.println (GetName ());  
    Serial.print ("Data Rcvd:  "); Serial.println (GetDataReceived ());  
    Serial.print ("Data Ready: "); Serial.println (GetDataReady ());  
}
        
