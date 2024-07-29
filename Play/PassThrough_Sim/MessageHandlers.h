
//
// MessageHandlers.h
//

#ifndef MESSAGEHANDLERS_H
#define MESSAGEHANDLERS_H

#include "src/ClearMsg_Auto.h"
#include "src/CollectMsg_Auto.h"
#include "src/SendMsg_Auto.h"
#include "src/SampleDataMsg_Auto.h"
#include "src/AllSentMsg_Auto.h"

class MessageHandlers
{
  public:
    static const int BufferSize = 1024;
    
    MessageHandlers ();
    void Initialize (TcpClientRev2 *);

    void ClearMsgHandler   (byte msgBytes []);
    void CollectMsgHandler (byte msgBytes []);
    void SendMsgHandler    (byte msgBytes []);

  private:
    TcpClientRev2  *socketPtr;

    SampleDataMsg_Auto sampleMsg;
    AllSentMsg_Auto    allSentMsg;

    int nextSample;
    
    int SampleBuffer [BufferSize];
    int get;
};

#endif
