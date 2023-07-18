
//
// LoopbackTestFPGA.h
//

#ifndef LOOPBACK_TEST_FPGA_H
#define LOOPBACK_TEST_FPGA_H

#include "src/LoopbackDataMessage.h"

class LoopbackTestFPGA
{
    public:
        LoopbackTestFPGA (LoopbackDataMsg::Data data);
        void RunTest ();
        LoopbackDataMsg::Data& GetResults ();
        
    private:
        LoopbackDataMsg::Data testInput;
        LoopbackDataMsg::Data testOutput;
};

#endif
