
//
// MessageHandlers.h
//

#ifndef MESSAGEHANDLERS_H
#define MESSAGEHANDLERS_H

#include "src/StatusMessage.h"

class MessageHandlers
{
  public:
    MessageHandlers ();
    void Initialize (StatusMessage *, TcpClientRev2 *);

    void StatusRequestMsgHandler (byte msgBytes []);
    void LoopbackDataMsgHandler  (byte msgBytes []);
    void RunLoopbackTestMsgHandler  (byte msgBytes []);
    void SendLoopbackTestResultsMsgHandler  (byte msgBytes []);

  private:
    StatusMessage *statusMsgPtr;
    TcpClientRev2 *socketPtr;
};

#endif
