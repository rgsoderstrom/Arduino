
//
// StatusMessage.cpp
//

#include "StatusMessage.h"

void  StatusMessage::SetName (char *str) {strncpy (data.name, str, data.MaxNameLength - 1);}        
char *StatusMessage::GetName () {return data.name;}
        
void StatusMessage::SetDataReceived (bool s) {data.DataReceived = (s == true ? 1 : 0);}
bool StatusMessage::GetDataReceived () {return data.DataReceived == 1 ? true : false;}

void StatusMessage::SetDataReady (bool s) {data.DataReady = (s == true ? 1 : 0);}
bool StatusMessage::GetDataReady () {return data.DataReady == 1 ? true : false;}
