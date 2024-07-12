
// FPGA_UT - FPGA interface unit test
#include <JobQueue.h>
#include <FPGA_Interface.h>
#include <FPGA_MsgBytes.h>

//  loadMsgID = 16'd100; // messages from Arduino
//  runMsgID  = 16'd101;
//  sendMsgID = 16'd102;
    
//  dataMsgID = 16'd103; // messages to Arduino
//  rdyMsgID  = 16'd104;

JobQueue OneTimeJobs;

// 
// FPGA Interface
//
FPGA_MsgBytes  fpgaByteBuffer;
FPGA_Interface fpgaInterface;

//**********************
unsigned char ClearSamplesMsgBytes   [] = {0x34, 0x12, 8, 0, 100, 0, 1, 0};
unsigned char CollectSamplesMsgBytes [] = {0x34, 0x12, 8, 0, 101, 0, 2, 0};
unsigned char SendSamplesMsgBytes    [] = {0x34, 0x12, 8, 0, 102, 0, 3, 0};   


void setup() 
{
    Serial.begin (9600);
    Serial.println ("starting");
    OneTimeJobs.Clear ();
    
    fpgaByteBuffer.Clear ();

    fpgaInterface.ConfigurePins ();
    fpgaInterface.AttachInterrupt ();
    fpgaInterface.RegisterInterruptCallback (InterruptCallbackFcn);

    interrupts ();

    Serial.println ("Sending msg to FPGA");
    fpgaInterface.WriteBytes (ClearSamplesMsgBytes, 8); // sizeof (LoadDataMsgBytes) / sizeof (LoadDataMsgBytes [0]));
//  fpgaInterface.WriteBytes (CollectSamplesMsgBytes, 8); 
//  fpgaInterface.WriteBytes (SendSamplesMsgBytes, 8);
}


void loop() 
{
    OneTimeJobs.RunJobs (millis ());
}

void InterruptProcessing (void *ptr)
{
    unsigned char ch = fpgaInterface.ReadOneByte ();
    Serial.print ("FPGA: 0x");
    Serial.println ((int) ch, HEX); 

    FPGA_MsgBytes::BufferState st = fpgaByteBuffer.StoreByte (ch);

    if (st == FPGA_MsgBytes::BufferState::MsgComplete)
    {
        Serial.print ("Message ID ");
        Serial.print (fpgaByteBuffer.GetMessageID ());
        Serial.println (" received");

        fpgaByteBuffer.Clear ();
    }
}

void InterruptCallbackFcn ()
{
    OneTimeJobs.Add (InterruptProcessing, NULL);
}
