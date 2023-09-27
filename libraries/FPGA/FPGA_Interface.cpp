
//
// FPGA_Interface.cpp
//
//    - bit-banging to communicate with FPGA
//

#include "FPGA_Interface.h"

// static InterruptHandler needs a copy of "this"
FPGA_Interface *FPGA_Interface::thisPtr = NULL;

FPGA_Interface::FPGA_Interface ()
{
    thisPtr = this;
    callback = NULL; // Interrupt Callback
}

//*******************************************************************

void FPGA_Interface::ConfigurePins ()
{
    pinMode (DataIn_Pin,     INPUT_PULLUP);  // bit from FPGA -> Arduino
    pinMode (ShiftInput_Pin, OUTPUT);
    pinMode (LastBit_Pin,    INPUT_PULLUP);
    pinMode (FirstBit_Pin,   INPUT_PULLUP);

    pinMode (DataOut_Pin,     OUTPUT); // bit from Arduino -> FPGA
    pinMode (ReadInput_Pin,   OUTPUT);
    pinMode (DataOutDone_Pin, OUTPUT);
  
    digitalWrite (ShiftInput_Pin,  0);
    digitalWrite (DataOut_Pin,     0);
    digitalWrite (ReadInput_Pin,   0);
    digitalWrite (DataOutDone_Pin, 0);  
}


void FPGA_Interface::AttachInterrupt ()
{
    attachInterrupt (digitalPinToInterrupt (FirstBit_Pin), InterruptHandler, RISING);
}

void FPGA_Interface::DetachInterrupt ()
{
    detachInterrupt (digitalPinToInterrupt (FirstBit_Pin));
}

//*******************************************************************

void FPGA_Interface::RegisterInterruptCallback (InterruptCallback cb)
{
   callback = cb;
}

//*******************************************************************

//
// InterruptHandler is a static function, so it uses copy of "this" to reference instance variables
//
void FPGA_Interface::InterruptHandler ()
{
    if (thisPtr->callback != NULL)
        thisPtr->callback ();
}

//*******************************************************************

// Shift in one byte, MSB first. Called to handle a "FirstBit" interrupt

byte FPGA_Interface::ReadOneByte ()
{
	byte receivedByte = 0;

	for (int i=0; i<8; i++)
	{
		int b = digitalRead (DataIn_Pin);
		b &= 1;

		if (b != 0)
			receivedByte |= (1 << (7 - i));

		digitalWrite (ShiftInput_Pin, 1);
		digitalWrite (ShiftInput_Pin, 0);    
	}
  
	return receivedByte;
}
  
//*******************************************************************

// Shift out one byte, MSB first

void FPGA_Interface::WriteOneByte (unsigned char b)
{
	for (int i=0; i<8; i++)
	{
		int mask = 1 << (7 - i);
		int thisBit = (b & mask) == 0 ? 0 : 1;

		digitalWrite (DataOut_Pin, thisBit == 0 ? 0 : 1);
		digitalWrite (ReadInput_Pin, 1);
		digitalWrite (ReadInput_Pin, 0);
	}

	digitalWrite (DataOutDone_Pin, 1);
	digitalWrite (DataOutDone_Pin, 0);
}

void FPGA_Interface::WriteBytes (unsigned char *bytes, int count)
{
    for (int i=0; i<count; i++)
      WriteOneByte (bytes [i]);  
}
