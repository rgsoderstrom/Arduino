
//
// MessageHandlers.cpp
//

#include <Arduino.h>
#include <TcpClientRev2.h>
#include <FPGA_Interface.h>

#include "FPGA_Messages.h"
#include "MessageHandlers.h"

//**************************************************************************

//void MessageHandlers::StatusRequestMsgHandler (byte msgBytes [])
//{
//    Serial.println ("StatusRequestMsgId");
//}

//**************************************************************************

void MessageHandlers::SpeedProfileSegmentMsgHandler (byte msgBytes []) 
{ 
    Serial.println ("SpeedProfileSegmentMsgId");

    SpeedProfileSegmentMsg *msg = new SpeedProfileSegmentMsg (msgBytes);
    
    int select = msg->GetMotorID () == 1 ? 0 : 1;

    profileMessage.Fields.profile [msg->GetIndex ()][select][0] = msg->GetSpeed ();
    profileMessage.Fields.profile [msg->GetIndex ()][select][1] = msg->GetDuration ();

    delete msg;
}

//**************************************************************************

void MessageHandlers::ClearMotorProfileMsgHandler (byte msgBytes []) 
{ 
    Serial.println ("ClearMotorProfileMsgId");
    
    profileMessage.Clear ();

    HeaderMessage *msg = new HeaderMessage (ClearProfileMsgID);

    for (int i=0; i<msg->GetByteCount (); i++)
    {
        fpgaPtr->WriteOneByte (msg->ByteStream [i]);
        Serial.println (msg->ByteStream [i]);
    }
    
    delete msg;


    
    statusDataPtr->SetReadyToRun (false);
    StatusMessage statusMsg (statusDataPtr);
    socketPtr->write ((char *) &statusMsg, statusMsg.ByteCount ());    
}

//**************************************************************************

void MessageHandlers::TransferProfileMsgHandler (byte msgBytes []) 
{
    Serial.println ("TransferProfileMsgId");

    for (int i=0; i<profileMessage.GetByteCount (); i++)
    {
        fpgaPtr->WriteOneByte (profileMessage.ByteStream [i]);
        Serial.println (profileMessage.ByteStream [i]);
    }

//    Serial.println ("------------------------------------");
//    
//    Serial.println (profileMessage.Fields.sync);
//    Serial.println (profileMessage.Fields.id);
//    Serial.println (profileMessage.Fields.byteCount);
//    Serial.println (profileMessage.GetByteCount ());
//
//    Serial.println ("------------------------------------");
        
    statusDataPtr->SetReadyToRun (true);
    StatusMessage statusMsg (statusDataPtr);
    socketPtr->write ((char *) &statusMsg, statusMsg.ByteCount ());
}

//**************************************************************************

void MessageHandlers::RunMotorsMsgHandler (byte msgBytes []) 
{
    Serial.println ("RunMotorsMsgId");

    motorBoardPtr->EnableMotors ();

    HeaderMessage runMessage (RunProfileMsgID);        

    for (int i=0; i<runMessage.GetByteCount (); i++)
    {
      fpgaPtr->WriteOneByte (runMessage.ByteStream [i]);
      Serial.println (runMessage.ByteStream [i]);
    }
     
    statusDataPtr->SetMotorsRunning (true);
    StatusMessage statusMsg (statusDataPtr);
    socketPtr->write ((char *) &statusMsg, statusMsg.ByteCount ());    

//    Serial.println (runMessage.Fields.sync);
//    Serial.println (runMessage.Fields.id);
//    Serial.println (runMessage.Fields.byteCount);
}

void MessageHandlers::SlowStopMotorsMsgHandler (byte msgBytes []) 
{ 
    Serial.println ("SlowStopMotorsMsgId");

    HeaderMessage msg (StopProfileMsgID);        

    for (int i=0; i<msg.GetByteCount (); i++)
    {
      fpgaPtr->WriteOneByte (msg.ByteStream [i]);
      Serial.println (msg.ByteStream [i]);
    }
     
    statusDataPtr->SetMotorsRunning (false);
    StatusMessage statusMsg (statusDataPtr);
    socketPtr->write ((char *) &statusMsg, statusMsg.ByteCount ());

//    Serial.println (msg.Fields.sync);
//    Serial.println (msg.Fields.id);
//    Serial.println (msg.Fields.byteCount);
}

void MessageHandlers::FastStopMotorsMsgHandler (byte msgBytes []) 
{ 
    Serial.println ("FastStopMotorsMsgId");


    motorBoardPtr->DisableMotors ();



    
    statusDataPtr->SetMotorsRunning (false);
    StatusMessage statusMsg (statusDataPtr);
    socketPtr->write ((char *) &statusMsg, statusMsg.ByteCount ());
}

//void MessageHandlers::SendFirstCollectionMsgHandler (byte msgBytes []) 
//{ 
//    Serial.println ("SendFirstCollectionMsgId");
//}
//
//void MessageHandlers::SendNextCollectionMsgHandler (byte msgBytes []) 
//{ 
//    Serial.println ("SendNextCollectionMsgId");
//}
