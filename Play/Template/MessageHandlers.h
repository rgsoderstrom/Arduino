
//
// MessageHandlers.h
//

#ifndef MESSAGEHANDLERS_H
#define MESSAGEHANDLERS_H

#include <JobQueue.h>

#include "src/Button1Msg_Auto.h"
#include "src/Button2Msg_Auto.h"
#include "src/Button3Msg_Auto.h"

class MessageHandlers
{
  public:

    MessageHandlers ();
    void Initialize (TcpClientRev2 *, PeriodicJobQueue *, JobQueue *);

    void Button1MsgHandler (byte msgBytes []);
    void Button2MsgHandler (byte msgBytes []);
    void Button3MsgHandler (byte msgBytes []);

    static MessageHandlers* thisPtr; // used by interrupt routine

  private:
    TcpClientRev2    *socketPtr;
    PeriodicJobQueue *periodicJobsPtr;
    JobQueue         *jobsPtr;

};

#endif
