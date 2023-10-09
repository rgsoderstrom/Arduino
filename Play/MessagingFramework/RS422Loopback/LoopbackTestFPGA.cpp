
//
// LoopbackTestFPGA.cpp
//

#include "MessageHeader.h"
#include "LoopbackTestFPGA.h"


//unsigned char LoadDataMsgBytes [] = {0x34, 0x12, 32, 0, 0x64, 0, 1, 0, 
//                                     1, 0, 2, 0, 3, 0, 4, 0, 5, 0, 6, 0, 7, 0, 8, 0, 9, 0, 10, 0, 11, 0, 12, 0};
//
//unsigned char RunProcMsgBytes []  = {0x34, 0x12, 8, 0, 0x65, 0, 2, 0};
//
//unsigned char SendDataMsgBytes [] = {0x34, 0x12, 8, 0, 0x66, 0, 3, 0};
//

LoopbackTestFPGA::LoopbackTestFPGA (byte msgBytes [])
{  
//  int byteCount = (msgBytes [3] << 8) + msgBytes [2];
    int byteCount = ((MessageHeader *) msgBytes)->ByteCount;

    Serial.println ("Passing PC LoadDataMsgBytes msg to FPGA");
    Serial.print (byteCount);
    Serial.println (" bytes");
    
    fpgaInterfacePtr->WriteBytes (msgBytes, byteCount);
}

void LoopbackTestFPGA::RunTest (byte msgBytes [])
{
    int byteCount = ((MessageHeader *) msgBytes)->ByteCount;
    
    Serial.println ("Passing PC RunTest msg to FPGA");
    Serial.print (byteCount);
    Serial.println (" bytes");
    
    fpgaInterfacePtr->WriteBytes (msgBytes, byteCount);
}
  
void LoopbackTestFPGA::SendResults (byte msgBytes [])
{
    int byteCount = ((MessageHeader *) msgBytes)->ByteCount;
    
    Serial.println ("Passing PC SendResults msg to FPGA");
    Serial.print (byteCount);
    Serial.println (" bytes");
    
    fpgaInterfacePtr->WriteBytes (msgBytes, byteCount);
}
