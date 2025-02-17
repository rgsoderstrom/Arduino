//
// DerivedMsg.h
//

#ifndef DERIVEDMSG_H
#define DERIVEDMSG_H

#include <Arduino.h>
#include "MessageIDs.h"
#include "MessageHeader.h"

#include "HeaderOnlyMsg_Auto.h"

class DerivedMsg : public HeaderOnlyMsg_Auto
{
    public:
		DerivedMsg () : HeaderOnlyMsg_Auto () {header.MsgId = 0x4321;}
};
#endif
