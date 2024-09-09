
//
// MessageHandlers.h
//

#ifndef MESSAGEHANDLERS_H
#define MESSAGEHANDLERS_H

#include "src/ClearMsg_Auto.h"
#include "src/CollectMsg_Auto.h"
#include "src/SendMsg_Auto.h"
#include "src/SampleDataMsg_Auto.h"
#include "src/SampleRateMsg_Auto.h"
#include "src/AnalogGainMsg_Auto.h"

class MessageHandlers
{
  public:
    static const int BufferSize = 1024;
    
    MessageHandlers ();
    void Initialize (TcpClientRev2 *);

    void ClearMsgHandler   (byte msgBytes []);
    void CollectMsgHandler (byte msgBytes []);
    void SendMsgHandler    (byte msgBytes []);
    void AnalogGainMsgHandler (byte msgBytes []);
    void SampleRateMsgHandler (byte msgBytes []);

  private:
    TcpClientRev2  *socketPtr;

    SampleDataMsg_Auto sampleMsg;

    int nextSample;
    
    int SampleBuffer [BufferSize];
    int get;
};

#endif
