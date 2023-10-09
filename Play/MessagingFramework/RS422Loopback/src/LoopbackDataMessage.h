
//
// LoopbackDataMessage.h
//

// Derived from auto-generated base class

#ifndef LOOP_MESSAGE_H
#define LOOP_MESSAGE_H

#include <Arduino.h>
#include "LoopbackDataMsg_Auto.h"

class LoopbackDataMsg : public LoopbackDataMsg_Auto
{                    
    public:
		LoopbackDataMsg () : LoopbackDataMsg_Auto () {}
        LoopbackDataMsg (byte *fromBytes) : LoopbackDataMsg_Auto (fromBytes) {}
};

#endif
