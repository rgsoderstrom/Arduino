
//
// EncodersOnly - 
//

#include <JobQueue.h>
#include <FPGA_MsgBytes.h>
#include "FPGA_Messages.h"
#include "FPGA_Interface.h"

JobQueue OneTimeJobs;

FPGA_Interface fpgaInterface;
FPGA_MsgBytes  msgByteBuffer;

HeaderMessage StartCollection (StartCollectionMsgID);
HeaderMessage StopCollection  (StopCollectionMsgID);
HeaderMessage BuildCollMsg    (BuildCollMsgID);
HeaderMessage SendCollMsg     (SendCollMsgID);

char obuf [50];

//****************************************************

void ProcessMessageBytes ()
{
    EncoderCountsMessage *msg = (EncoderCountsMessage *) msgByteBuffer.GetBytes ();

    bool messageIsFull = msg->Fields.remaining >= EncoderCountsMessage::MaxSamplesPerMessage;
    
    int printLoopCount = messageIsFull 
                       ? EncoderCountsMessage::MaxSamplesPerEncoder 
                       : msg->Fields.remaining / 2; // each loop prints two

    sprintf (obuf, "Remaining = %d", msg->Fields.remaining);
    Serial.println (obuf);

    for (int i=0; i<printLoopCount; i++)
    {
        sprintf (obuf, "%x, %x", msg->Fields.samples [i][0], msg->Fields.samples [i][1]);
        Serial.println (obuf);
    }

    msgByteBuffer.Clear ();
    
    if (msg->Fields.remaining > 0)
    {
        fpgaInterface.WriteBytes (BuildCollMsg.GetBytePtr (), BuildCollMsg.GetByteCount ());
        fpgaInterface.WriteBytes (SendCollMsg.GetBytePtr (), SendCollMsg.GetByteCount ());
    }
}

//****************************************************

void ReadOneByte ()
{
	  noInterrupts ();
    
    FPGA_MsgBytes::BufferState state = msgByteBuffer.StoreByte (fpgaInterface.ReadOneByte ());

    if (state == FPGA_MsgBytes::BufferState::MsgComplete)
        OneTimeJobs.Add (ProcessMessageBytes, NULL);     
    
	  interrupts ();
}

//****************************************************

void InterruptCallbackFcn ()
{
	OneTimeJobs.Add (ReadOneByte, NULL);
}

void setup (void) 
{
		Serial.begin(9600);
		Serial.println ("EncodersOnly setup");
		fpgaInterface.RegisterInterruptCallback (InterruptCallbackFcn);
		fpgaInterface.AttachInterrupt ();

		fpgaInterface.ConfigurePins ();

	  // send StartCollection message
		fpgaInterface.WriteBytes (StartCollection.GetBytePtr (), StartCollection.GetByteCount ());
	
	  // wait for FPGA to "capture" data
		delay (1000);
  	
	  // send StopCollection message
		fpgaInterface.WriteBytes (StopCollection.GetBytePtr (), StopCollection.GetByteCount ());

	  // send BuildCollection message
    fpgaInterface.WriteBytes (BuildCollMsg.GetBytePtr (), BuildCollMsg.GetByteCount ());
    
    interrupts ();
		
	  // send SendCollection message
    fpgaInterface.WriteBytes (SendCollMsg.GetBytePtr (), SendCollMsg.GetByteCount ());

}

void loop(void) 
{
    OneTimeJobs.RunJobs (millis ());
}
