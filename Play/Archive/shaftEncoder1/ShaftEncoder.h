
//
// ShaftEncoder.h
//

#ifndef SHAFTENCODER_H
#define SHAFTENCODER_H

class ShaftEncoder
{
public:
    ShaftEncoder ();
    bool Open (int select); // select == 1 or 2

    void GetNumberOfTransitions (int& numbTrans, int& numbMissed, bool clearAfterRead = true);
    int  GetNumberOfTransitions (bool clearAfterRead = true);

    void TestForTransition ();

private:
  //
  // static variables
  //
  
  // pin numbers
    static const int encoder1InputA = 5;
    static const int encoder1InputB = 6;
    static const int encoder2InputA = 11;
    static const int encoder2InputB = 13;
    
    static const char states []; // the 4 states of inputs A & B
    static const int  N; // number of states

  //
  // instance variables
  //
    int inputA; // pin numbers
    int inputB;

    int numberTransitions = 0; // pos or neg depending on direction of rotation
    int numberMissed = 0;

    int wasA = -1;
    int wasB = -1;

  // index of state at last read
    int wasIndex; // 0 .. N-1
};

#endif
