
//
// LoopbackTestFPGA.h
//

#ifndef LOOPBACK_TEST_FPGA_H
#define LOOPBACK_TEST_FPGA_H

#include <FPGA_Interface.h>
//#include <FPGA_MsgBytes.h>

class LoopbackTestFPGA
{
    public:
        LoopbackTestFPGA (byte msgBytes []);
        void RunTest     (byte msgBytes []);
        void SendResults (byte msgBytes []);
        
    protected:
//    static FPGA_MsgBytes  *fpgaByteBufferPtr;
      static FPGA_Interface *fpgaInterfacePtr;
};

#endif
