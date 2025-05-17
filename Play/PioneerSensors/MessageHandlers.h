
//
// MessageHandlers.h
//

#ifndef MESSAGEHANDLERS_H
#define MESSAGEHANDLERS_H

#include <JobQueue.h>

#include "src/StartSamplingMsg_Auto.h"
#include "src/SendSamplesMsg_Auto.h"
#include "src/SensorDataMsg_Auto.h"

class MessageHandlers
{
  public:
    static const int SampleBufferSize = 800;
    
    MessageHandlers ();
    void Initialize (TcpClientRev2 *, PeriodicJobQueue *, JobQueue *);

    bool SamplingInProgress;

    void StartSamplingMsgHandler (byte msgBytes []);
    void SendSamplesMsgHandler   (byte msgBytes []);

    static MessageHandlers* thisPtr; // used by interrupt routine

  private:
    TcpClientRev2    *socketPtr;
    PeriodicJobQueue *periodicJobsPtr;
    JobQueue         *jobsPtr;

    static void IntHandler (void);
    static void RecordSensors (void);

    int PressureHist [SampleBufferSize];
    int AngleHist    [SampleBufferSize];
    //int TimeHist     [SampleBufferSize];
    unsigned int put;
    unsigned int get;
   
    static unsigned int StartMillis; // time when sampling started
    
    SensorDataMsg_Auto sampleMsg;
    
    static const int PressurePin; // analog pin connected to pressure transducer
    static const int PhaseAPin;   // digital pins connected to shaft encoder
    static const int PhaseBPin;
    static       int interruptPin; // = PhaseAPin;

    static volatile int AngleCounts;// = 0; // accumulates shaft encoder pulse count
    static char *RecordSensorsJobName;

};

#endif
