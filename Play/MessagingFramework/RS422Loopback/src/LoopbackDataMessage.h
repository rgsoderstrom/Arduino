
//
// LoopbackDataMessage.h
//

#ifndef LOOP_MESSAGE_H
#define LOOP_MESSAGE_H

#include <Arduino.h>
#include "MessageIDs.h"
#include "MessageHeader.h"

class LoopbackDataMsg // same message used both directions
{                    
    public:
        struct Data
        {
            static const int MaxCount = 32;
            
            public:
              byte source;
              byte dataByteCount;
              byte dataBytes [MaxCount];
        };
        
        LoopbackDataMsg ();
        LoopbackDataMsg (Data& srcData);
        LoopbackDataMsg (byte *fromBytes);
        
        void ToBytes (byte *);
        void ToConsole ();
        int ByteCount () {return header.ByteCount;}

        void SetSource (byte s) {data.source = s;}
        byte GetSource () {return data.source;}
		
		Data& GetData () {return data;}

		int GetByteCount () {return data.dataByteCount;}
		
        void Clear () {data.dataByteCount = 0;}
        
        void Put (byte d) {if (data.dataByteCount < LoopbackDataMsg::Data::MaxCount)
                               data.dataBytes [data.dataByteCount++] = d;}
    private:
      MessageHeader header;
      Data  data;
};

#endif
