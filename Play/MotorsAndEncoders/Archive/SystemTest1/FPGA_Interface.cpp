
//
// FPGA_Interface.cpp
//
//    - messages and bit-banging to communicate with FPGA
//

#include "FPGA_Interface.h"

FPGA_Interface *FPGA_Interface::thisPtr = NULL;

FPGA_Interface::FPGA_Interface ()
{
    thisPtr = this;
    motorCallback = NULL;
    encoderCallback = NULL;
}

//*******************************************************************

void FPGA_Interface::ConfigurePins ()
{
    pinMode (DataIn_Pin,         INPUT);
    pinMode (ShiftInput_Pin,     OUTPUT);
    pinMode (SampleEncoders_Pin, OUTPUT);
    pinMode (DataOut_Pin,        OUTPUT);
    pinMode (ReadInput_Pin,      OUTPUT);
    pinMode (LoadPWM_Pin,        OUTPUT);
    pinMode (EncHalfFull_Pin,    INPUT_PULLUP);
  
    digitalWrite (ShiftInput_Pin,     0);
    digitalWrite (SampleEncoders_Pin, 0);
    digitalWrite (DataOut_Pin,   0);
    digitalWrite (ReadInput_Pin, 0);
    digitalWrite (LoadPWM_Pin,   0);  
}


void FPGA_Interface::AttachInterrupt ()
{
  //
  // clear any pending interrupt
  //
    InputWord received; 
    digitalWrite (SampleEncoders_Pin, 1);
    digitalWrite (SampleEncoders_Pin, 0);
    received.packed = thisPtr->ReadDataIn (); 
    
    attachInterrupt (digitalPinToInterrupt (EncHalfFull_Pin), InterruptHandler, RISING);
}

void FPGA_Interface::DetachInterrupt ()
{
    detachInterrupt (digitalPinToInterrupt (EncHalfFull_Pin)); // , InterruptHandler, RISING);
}

//*******************************************************************

void FPGA_Interface::RegisterMotorCallback (MotorCallback mc)
{
    motorCallback = mc;
}

void FPGA_Interface::RegisterEncoderCallback (EncoderCallback ec)
{
   encoderCallback = ec;
}

//*******************************************************************

//void FPGA_Interface::SetMotorSpeed (int select, int speed)
//{
//  
//}

void FPGA_Interface::SetMotorSpeed (int speed1, int speed2)
{
    unsigned char both = (speed1 << 4) | speed2;

    ShiftDataOut (both);
    digitalWrite (LoadPWM_Pin, 1);  
    digitalWrite (LoadPWM_Pin, 0);    
}

//*******************************************************************

void FPGA_Interface::InterruptHandler ()
{
    InputWord received; // bits received from FPGA
    
    digitalWrite (SampleEncoders_Pin, 1);
    digitalWrite (SampleEncoders_Pin, 0);

    received.packed = thisPtr->ReadDataIn ();

    if (thisPtr->encoderCallback != NULL)
      (*thisPtr->encoderCallback) (millis (), received.unpacked.Encoder1, received.unpacked.Encoder2);

    //if (motorCallback)
    //  (*motorCallback) (????);
}

//*******************************************************************

const int BitsPerSerialRead = 22;

unsigned long FPGA_Interface::ReadDataIn ()
{
  unsigned long data = 0;

  for (int i=0; i<BitsPerSerialRead; i++)
  {
      int b = digitalRead (DataIn_Pin);
      b &= 1;

      if (b != 0)
        data |= (1L << (21 - i));

      digitalWrite (ShiftInput_Pin, 1);
      digitalWrite (ShiftInput_Pin, 0);    
  }

  return data;
}
  
//*******************************************************************

// Shift out one byte, MSB first

void FPGA_Interface::ShiftDataOut (unsigned char b)
{
  for (int i=0; i<8; i++)
  {
    int mask = 1 << (7 - i);
    int thisBit = (b & mask) == 0 ? 0 : 1;

    digitalWrite (DataOut_Pin, thisBit == 0 ? 0 : 1);
    digitalWrite (ReadInput_Pin, 1);
    digitalWrite (ReadInput_Pin, 0);
  }
}
