
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

// buffer for bytes received from FPGA
FPGA_MsgBytes fpgaByteBuffer;

FPGA_Interface fpgaInterface;

//**********************
unsigned char LoadDataMsgBytes [] = {0x34, 0x12, 32, 0, 0x64, 0, 1, 0, 
                                     1, 0, 2, 0, 3, 0, 4, 0, 5, 0, 6, 0, 7, 0, 8, 0, 9, 0, 10, 0, 11, 0, 12, 0};

unsigned char RunProcMsgBytes [] = {0x34, 0x12, 8, 0, 0x65, 0, 2, 0};

unsigned char SendDataMsgBytes [] = {0x34, 0x12, 8, 0, 0x66, 0, 3, 0};


void setup() 
{
    Serial.begin (9600);
    Serial.println ("starting");
  
    fpgaByteBuffer.Clear ();

    fpgaInterface.ConfigurePins ();
    fpgaInterface.AttachInterrupt ();
    fpgaInterface.RegisterInterruptCallback (InterruptCallbackFcn);

//    interrupts ();

    Serial.println ("Sending LoadDataMsgBytes msg to FPGA");
    fpgaInterface.WriteBytes (LoadDataMsgBytes, sizeof (LoadDataMsgBytes) / sizeof (LoadDataMsgBytes [0]));
    
    Serial.println ("Sending RunProcessing msg to FPGA");
    fpgaInterface.WriteBytes (RunProcMsgBytes,  sizeof (RunProcMsgBytes)  / sizeof (RunProcMsgBytes [0]));
}


void loop() 
{
    OneTimeJobs.RunJobs (millis ());
}

void InterruptProcessing (void *ptr)
{
    unsigned char ch = fpgaInterface.ReadOneByte ();
    Serial.print ("Int: 0x");
    Serial.println ((int) ch, HEX); 

    FPGA_MsgBytes::BufferState st = fpgaByteBuffer.StoreByte (ch);

    if (st == FPGA_MsgBytes::BufferState::MsgComplete)
    {
        Serial.print ("Message ID ");
        Serial.print (fpgaByteBuffer.GetMessageID ());
        Serial.println (" received");

        fpgaByteBuffer.Clear ();
        
        if (fpgaByteBuffer.GetMessageID () == 104)
        {
            Serial.println ("sending SendDataMsgBytes to FPGA");
            fpgaInterface.WriteBytes (SendDataMsgBytes, sizeof (SendDataMsgBytes) / sizeof (SendDataMsgBytes [0]));
        }
    }
}

void InterruptCallbackFcn ()
{
    OneTimeJobs.Add (InterruptProcessing, NULL);
}
