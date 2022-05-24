
//
// EncodersOnly - 
//

#include <JobQueue.h>
#include "FPGA_Messages.h"
#include "FPGA_Interface.h"

JobQueue OneTimeJobs;

FPGA_Interface fpgaInterface;

HeaderMessage StartCollection (StartCollectionMsgID);
HeaderMessage StopCollection  (StopCollectionMsgID);
HeaderMessage BuildCollMsg    (BuildCollMsgID);
HeaderMessage SendCollMsg     (SendCollMsgID);

byte ReceivedBytes [100];
char obuf [50];

void ReadAndPrintByte ()
{
	  noInterrupts ();
	
  	unsigned char c = fpgaInterface.ReadOneByte ();

    sprintf (obuf, "0x%x %d", c, c);
    Serial.println (obuf); 

	  interrupts ();
}

void InterruptCallbackFcn ()
{
	OneTimeJobs.Add (ReadAndPrintByte, NULL);
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
    fpgaInterface.WriteBytes (BuildCollMsg.GetBytePtr (), BuildCollMsg.GetByteCount ());
    fpgaInterface.WriteBytes (BuildCollMsg.GetBytePtr (), BuildCollMsg.GetByteCount ());
    
    interrupts ();
		
	  // send SendCollection message
    fpgaInterface.WriteBytes (SendCollMsg.GetBytePtr (), SendCollMsg.GetByteCount ());

}

void loop(void) 
{
    OneTimeJobs.RunJobs (millis ());

}
