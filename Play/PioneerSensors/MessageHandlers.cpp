
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

const int    MessageHandlers::PhaseAPin = 2; // digital pins connected to shaft encoder
const int    MessageHandlers::PhaseBPin = 4;
      int    MessageHandlers::interruptPin; // = PhaseAPin;
unsigned int MessageHandlers::StartMillis;  // millis when sampling starts

volatile int MessageHandlers::AngleCounts = 0; // accumulates shaft encoder pulse count

int SampleTime = 5; //10; // 50; // milliseconds between sensor reads

MessageHandlers* MessageHandlers::thisPtr = null;

//********************************************************************************

char *MessageHandlers::RecordSensorsJobName = "Sample";

void MessageHandlers::IntHandler ()
{
    int a = digitalRead (PhaseAPin) & 1;
    int b = digitalRead (PhaseBPin) & 1;

    if (a == b) thisPtr->AngleCounts++;
    else        thisPtr->AngleCounts--;
}

//******************************************************************
//
// RecordSensors - periodic task to read and record sensors
//

//bool RecordingInProg = false;

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

    if (thisPtr->put == SampleBufferSize)
    {
        thisPtr->SamplingInProgress = false;

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

    SamplingInProgress = false;
    
    //pinMode (interruptPin, INPUT_PULLUP);
    pinMode (PhaseAPin, INPUT); //   INPUT_PULLUP); --- external pull-ups added
    pinMode (PhaseBPin, INPUT); //   INPUT_PULLUP);
}

//**************************************************************************

void MessageHandlers::StartSamplingMsgHandler (byte msgBytes [])
{
    //Serial.println ("Start Sampling");  

    SamplingInProgress = true;
    
    TextMessage msg2 ("Starting sampling");
    socketPtr->write ((char *) &msg2, msg2.ByteCount ());   
    
    //RecordingInProg = false;
    AngleCounts = 0;
    put = 0;
    get = 0;  

#ifdef RealSensors    
    attachInterrupt (digitalPinToInterrupt (interruptPin), IntHandler, CHANGE); // RISING);
#endif
    
    periodicJobsPtr->Add (RecordSensors, NULL, RecordSensorsJobName, SampleTime);
    StartMillis = millis ();
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
        sampleMsg.data.Time     [i] = get * SampleTime; // TimeHist     [get];

//        if (i<3)
//        {
//            Serial.print   (PressureHist [get]); Serial.print (" ");
//            Serial.print   (AngleHist [get]);    Serial.print (" ");
//            Serial.println (TimeHist [get]);
//        }
    }

    socketPtr->write ((char *) &sampleMsg, sampleMsg.header.ByteCount);
}
