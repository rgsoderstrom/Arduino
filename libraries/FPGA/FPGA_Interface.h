
//
// FPGA_Interface.h
//
//    - bit-banging to communicate with FPGA
//

#ifndef FPGA_INTERFACE_H
#define FPGA_INTERFACE_H

#include <arduino.h>

typedef void (*InterruptCallback) (); 

class FPGA_Interface
{
  public:
    FPGA_Interface ();

    void ConfigurePins ();
    void AttachInterrupt ();
    void DetachInterrupt ();
    
    void RegisterInterruptCallback (InterruptCallback cb);

    unsigned char ReadOneByte ();        // from FPGA
    void WriteOneByte (unsigned char b); // to FPGA
    
	void WriteBytes (unsigned char *b, int count); // to FPGA
	
	int GetProfileDonePin () {return digitalRead (ProfileDone_Pin);} // for MotorsAndEncoders only
    
  private:
    static FPGA_Interface *thisPtr;
    static void InterruptHandler ();
    
    InterruptCallback callback;
    
  //
  // Arduino pins connected to FPGA
  //
    static const int DataIn_Pin     = A2; // bits shifted out of FPGA
    static const int ShiftInput_Pin = A3; // tells FPGA to shift its output word
    static const int LastBit_Pin    = A4; // true when DataIn_Pin has last bit of word
    static const int FirstBit_Pin   = 2;  // interrupts when FPGA has a word to send. First
                                          //   bit is on DataIn_Pin

    static const int DataOut_Pin     = A5; // shifted in to FPGA
    static const int ReadInput_Pin   = 6;  // tells FPGA to read input bit
    static const int DataOutDone_Pin = 5;  // tells FPGA last bit of word hase been read
	
	static const int ProfileDone_Pin = 11;//A1; // for MotorsAndEncoders only
};

#endif
