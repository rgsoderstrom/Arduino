
//
// FPGA_Interface.h
//
//    - messages and bit-banging to communicate with FPGA
//

#ifndef FPGA_INTERFACE_H
#define FPGA_INTERFACE_H

#include <arduino.h>

typedef union InputWord
{
  unsigned long packed;

  struct 
  {
    int PWM2     : 1;
    int PWM1     : 1;
    int Encoder2 : 10;
    int Encoder1 : 10;
    int unused   : 10;
    
  } unpacked;
};


typedef void (*MotorCallback) (int sel); 
typedef void (*EncoderCallback) (unsigned long timeTag, int e1, int e2); 


class FPGA_Interface
{
  public:
    FPGA_Interface ();

    void ConfigurePins ();
    void AttachInterrupt ();
    void DetachInterrupt ();



    
    //void SetMotorSpeed (int select,  // 0 or 1
    //                    int speed);  // -15 -> +15

    void SetMotorSpeed (int speed1, int speed2);




                        
    void RegisterMotorCallback (MotorCallback mc);
    void RegisterEncoderCallback (EncoderCallback ec);                    

  private:
    static FPGA_Interface *thisPtr;
    static void InterruptHandler ();
    
    unsigned long ReadDataIn ();
    void ShiftDataOut (unsigned char b);

    MotorCallback motorCallback;
    EncoderCallback encoderCallback;
    
  //
  // Arduino pins connected to FPGA
  //
    static const int DataIn_Pin          = 13; // bits shifted out of FPGA
    static const int ShiftInput_Pin      = 11; // tells FPGA to shift its output word
    static const int SampleEncoders_Pin  = 6;  // copies encoder counts to serializer and clears counters
    static const int EncHalfFull_Pin     = 2;  // interrupt when either encoder count at half its full range

    static const int DataOut_Pin   = 5;  // shifted in to FPGA
    static const int ReadInput_Pin = 3;  // tells FPGA to read input bit
    static const int LoadPWM_Pin   = A2; // FPGA transfers motor speeds to PWMs
};

#endif
