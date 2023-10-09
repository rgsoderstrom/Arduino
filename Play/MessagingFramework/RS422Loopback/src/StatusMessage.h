
//
// StatusMessage.h
//

#ifndef STATUS_MESSAGE_H
#define STATUS_MESSAGE_H

#include "StatusMsg_Auto.h"

class StatusMessage : public StatusMsg_Auto
{
   public:
        StatusMessage () : StatusMsg_Auto () {}
        StatusMessage (byte fromBytes []) : StatusMsg_Auto (fromBytes) {}
		
		void ToConsole () {StatusMsg_Auto::ToConsole ();}
		
        int ByteCount () {return header.ByteCount;}

        void  SetName (char *);
        char *GetName ();
        
        void SetDataReceived (bool s);
        bool GetDataReceived ();
        
        void SetDataReady (bool s);
        bool GetDataReady ();
};

#endif
