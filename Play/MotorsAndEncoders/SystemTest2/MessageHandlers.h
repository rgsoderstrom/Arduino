
//
// MessageHandlers.h
//

#ifndef MESSAGEHANDLERS_H
#define MESSAGEHANDLERS_H

#include "MotorBoard.h"
#include "FPGA_Interface.h"
#include "Messages.h"

class MessageHandlers
{
  public:
    //void StatusRequestMsgHandler       (byte msgBytes []);
    void SpeedProfileSegmentMsgHandler (byte msgBytes []);
    void ClearMotorProfileMsgHandler   (byte msgBytes []);
    void TransferProfileMsgHandler     (byte msgBytes []);
    
    void RunMotorsMsgHandler (byte msgBytes []);
    
    void SlowStopMotorsMsgHandler (byte msgBytes []);
    void FastStopMotorsMsgHandler (byte msgBytes []);
    //void SendFirstCollectionMsgHandler (byte msgBytes []);
    //void SendNextCollectionMsgHandler  (byte msgBytes []);

    ProfileMessage profileMessage;

    static StatusMessage::StatusData *statusDataPtr;
    static TcpClientRev2             *socketPtr;
    static FPGA_Interface            *fpgaPtr;    
    static MotorBoard                *motorBoardPtr;
};

#endif
