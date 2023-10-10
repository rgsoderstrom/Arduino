
//
// MessageHandlers.h
//

#ifndef MESSAGEHANDLERS_H
#define MESSAGEHANDLERS_H

#include <FPGA_Interface.h>
#include "src/StatusMessage.h"

class MessageHandlers
{
  public:
    MessageHandlers ();
    void Initialize (StatusMessage *, FPGA_Interface *, TcpClientRev2 *);

    void StatusRequestMsgHandler (byte msgBytes []);
    void LoopbackDataMsgHandler  (byte msgBytes []);
    void RunLoopbackTestMsgHandler  (byte msgBytes []);
    void SendLoopbackTestResultsMsgHandler  (byte msgBytes []);

  private:
    StatusMessage  *statusMsgPtr;
    FPGA_Interface *fpgaInterfacePtr;
    TcpClientRev2  *socketPtr;
};

#endif
