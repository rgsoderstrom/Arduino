
//
// Messages.cpp
//

#include "Messages.h"

//***********************************************************************************************

MessageHeader::MessageHeader ()
{
    Sync      = SyncWord;
    ByteCount = 0;
    MsgId     = -1;
}

//***********************************************************************************************

StatusMessage::StatusMessage ()
{
    header.ByteCount = sizeof (*this);
    header.MsgId = StatusMsgId;
    
    sampling = 0;
    numberRamSamples = 0;
}

//***********************************************************************************************

TemperatureMessage::TemperatureMessage () 
{
    header.ByteCount = sizeof (TemperatureMessage); 
    header.MsgId = TemperatureMsgId;
    NumberTemperatures = 0;
}

// returns TRUE if message data array full
bool TemperatureMessage::Add (unsigned long ts, float temp)
{
    unsigned int& put = NumberTemperatures;

    if (put == MaxNumberTemperatures) // already full
        return true;

    samples [put].timeTag = ts;
    samples [put].temperature = temp;

    return ++put == MaxNumberTemperatures;
}

//***********************************************************************************************
