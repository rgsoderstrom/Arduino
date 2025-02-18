
//
// Encoders.h
//

#ifndef ENCODERS_H
#define ENCODERS_H

#include "TcpClientRev2.h"

//***************************************************************************

#define BitsPerChannel 12

union Received
{
  unsigned long Raw;

  struct
  {
    int Chan2 : BitsPerChannel;
    int Chan1 : BitsPerChannel;
    int unused : (32 - 2 * BitsPerChannel);
  } Formatted;
};

//***************************************************************************

class Encoders
{
  public:
    Encoders ();

    bool Setup ();
    void Loop (TcpClientRev2 socket);

    void ReadCounts (int& chan1, int& chan2);
    
  private:
    int ReadChannel ();
    
    static const int LoadSR  = 4; // output
    static const int ShiftSR = 6; // output
    static const int ReadSR  = 5; // input

    int Channel1;
    int Channel2;

    
    EncoderCountsMessage message; // TEMP
};

#endif
