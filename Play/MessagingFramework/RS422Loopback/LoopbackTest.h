
//
// LoopbackTest.h
//

#ifndef LOOPBACK_TEST_H
#define LOOPBACK_TEST_H

#include "src/LoopbackDataMessage.h"

class LoopbackTest
{
    public:
        LoopbackTest (LoopbackDataMsg::Data data);
        void RunTest ();
        LoopbackDataMsg::Data& GetResults ();
        
    private:
        LoopbackDataMsg::Data testInput;
        LoopbackDataMsg::Data testOutput;
};

#endif
