
//
// MessageHandlers.cpp
//

#include <Arduino.h>
#include <TcpClientRev2.h>
#include "src\DoneSamplingMsg_Auto.h"
#include "src\SensorDataMsg_Auto.h"
#include "src\TextMessage.h"

#include "MessageHandlers.h"

#define null ((void *) 0)

#define RealSensors

const int MessageHandlers::PressurePin = 0; // analog pin connected to pressure transducer

const int MessageHandlers::PhaseAPin = 2; // digital pins connected to shaft encoder
const int MessageHandlers::PhaseBPin = 4;
      int MessageHandlers::interruptPin; // = PhaseAPin;

volatile int MessageHandlers::AngleCounts = 0; // accumulates shaft encoder pulse count

int SampleTime = 50; // milliseconds between sensor reads

MessageHandlers* MessageHandlers::thisPtr = null;


//********************************************************************************

char *MessageHandlers::RecordSensorsJobName = "Sample";

void MessageHandlers::IntHandler ()
{
    int b = digitalRead (PhaseBPin) & 1;

    if (b == 1) thisPtr->AngleCounts++;
    else        thisPtr->AngleCounts--;
}

void MessageHandlers::RecordSensors (void)
{
    int p = thisPtr->put++;
    
#ifdef RealSensors  
    thisPtr->PressureHist [p] = analogRead (PressurePin);
    thisPtr->AngleHist    [p] = thisPtr->AngleCounts;
#else
    static int P = 100;
    static int A = 200;
    
    thisPtr->PressureHist [p] = P++;
    thisPtr->AngleHist    [p] = A++;
#endif

    thisPtr->TimeHist [p] = millis ();
    
    if (thisPtr->put == SampleBufferSize)
    {
        detachInterrupt (digitalPinToInterrupt (interruptPin));
        thisPtr->periodicJobsPtr->Remove (RecordSensorsJobName);

        TextMessage msg2 ("Done sampling");
        thisPtr->socketPtr->write ((char *) &msg2, msg2.ByteCount ());   
    
        DoneSamplingMsg_Auto msg;
        thisPtr->socketPtr->write ((char *) &msg, msg.header.ByteCount);        
    }
}

//**************************************************************************

MessageHandlers::MessageHandlers ()
{
    socketPtr       = null;
    periodicJobsPtr = null;
    jobsPtr         = null;
    
    interruptPin = PhaseAPin;
    thisPtr = this;
}

//**************************************************************************

void MessageHandlers::Initialize (TcpClientRev2 *sp, PeriodicJobQueue *pjq, JobQueue *jq)
{
    socketPtr       = sp;
    periodicJobsPtr = pjq;
    jobsPtr         = jq;
    
    pinMode (interruptPin, INPUT_PULLUP);
    pinMode (PhaseAPin,    INPUT_PULLUP);
    pinMode (PhaseBPin,    INPUT_PULLUP);
}

//**************************************************************************

void MessageHandlers::StartSamplingMsgHandler (byte msgBytes [])
{
    //Serial.println ("Start Sampling");  
    
    TextMessage msg2 ("Starting sampling");
    socketPtr->write ((char *) &msg2, msg2.ByteCount ());   
    
    AngleCounts = 0;
    put = 0;
    get = 0;  

#ifdef RealSensors    
    attachInterrupt (digitalPinToInterrupt (interruptPin), IntHandler, RISING);
#endif
    
    periodicJobsPtr->Add (RecordSensors, NULL, RecordSensorsJobName, SampleTime);
}

//**************************************************************************

void MessageHandlers::SendSamplesMsgHandler (byte msgBytes [])
{
    //Serial.print   ("Send ");
    //Serial.println (get);

    int remaining = SampleBufferSize - get;
    
    if (remaining < 0) 
        remaining = 0;

    int countThisMessage = remaining < SensorDataMsg_Auto::Data::MaxCount ?
                           remaining : SensorDataMsg_Auto::Data::MaxCount;

    sampleMsg.data.Count = countThisMessage;
    
    for (int i=0; i<countThisMessage; i++, get++)
    {
        sampleMsg.data.Pressure [i] = PressureHist [get];
        sampleMsg.data.Angle    [i] = AngleHist    [get];
        sampleMsg.data.Time     [i] = TimeHist     [get];

//        if (i<3)
//        {
//            Serial.print   (PressureHist [get]); Serial.print (" ");
//            Serial.print   (AngleHist [get]);    Serial.print (" ");
//            Serial.println (TimeHist [get]);
//        }
    }

    socketPtr->write ((char *) &sampleMsg, sampleMsg.header.ByteCount);
}
