
//
// Messages.h
//

#ifndef MESSAGES_H
#define MESSAGES_H

#include <MessageHeader.h>

//*********************************************************************************

//
// Message IDs
//

// Arduino -> PC
#define AcknowledgeMsgId   1
#define TextMsgId          2
#define StatusMsgId        3
#define EncoderCountsMsgId 4


// PC -> Arduino
#define KeepAliveMsgId            1  // no action required, just keeps socket open
#define StatusRequestMsgId        2

#define ClearMotorProfileMsgId    3  // command to clear all speed/durations
#define SpeedProfileSegmentMsgId  4  // one duration and speed for one motor
#define TransferSpeedProfileMsgId 5

#define RunMotorsMsgId             6  // execute previously sent MotorTimeAndSpeed profile
#define SlowStopMotorsMsgId        7  // slowly stop motors
#define FastStopMotorsMsgId        8  // immediately stop motors
//#define SendFirstCollectionMsgId 9
//#define SendNextCollectionMsgId  10
#define DisconnectMsgId            99

//*********************************************************************************
//*********************************************************************************
//*********************************************************************************

//
// PC to Arduino
//    - the messages that have a "data" section are defined here
//    - header-only messages inherit what they need from MessageHeader
//

//
// Motor Speed Profile Segment
//
class SpeedProfileSegmentMsg
{
    public:
        SpeedProfileSegmentMsg ();
        SpeedProfileSegmentMsg (byte *fromBytes);
        
        bool ToBytes  (byte byteArray [], int maxLength);
        void ToString (); // prints to console, not a character array

        class Segment
        {
            public:
              int index;     // 0 -> (MaxNumberSegments - 1)
              int motorID;   // 1 or 2, left or right
              int speed;     // -127 -> 127
              int duration;  // tenths of second, 0 -> 25.5
        };

      // setters are for unit test only
        int SetIndex    (int s) {data.index = s;}
        int SetMotorID  (int s) {data.motorID = s;}
        int SetSpeed    (int s) {data.speed = s;}
        int SetDuration (int s) {data.duration = s;}
        
        int GetIndex    () {return data.index;}
        int GetMotorID  () {return data.motorID;}
        int GetSpeed    () {return data.speed;}        
        int GetDuration () {return data.duration;}

    private:
      MessageHeader header;
      Segment       data;
};

//*********************************************************************************

//
// Arduino to PC
//

class AcknowledgeMessage
{
    struct AckData
    {
        unsigned short MsgSequenceNumber;
    };

  public:
    AcknowledgeMessage (unsigned short seqNumber);
    AcknowledgeMessage (byte msgBytes []);

    bool ToBytes  (byte byteArray [], int maxLength);
    void ToString (); // prints to console, not a character array

    int ByteCount () {return header.ByteCount;}
    
  private:
    MessageHeader  header;
    AckData        data;
};

//*********************************************************************************

class TextMessage
{
    public:
      TextMessage (char txt []);
      int ByteCount () {return header.ByteCount;}
    
    private:
      static const int TextBufferSize = 30;
      
      MessageHeader header;
      char          text [TextBufferSize];
};

//*********************************************************************************

class StatusMessage
{
   public:
        struct StatusData
        {
            StatusData ();
            StatusData (char name []);
            
            void SetReadyForMessages (bool flag) {readyForMessages = flag ? 1 : 0;}
            void SetReadyToRun       (bool flag) {readyToRun       = flag ? 1 : 0;}
            void SetMotorsRunning    (bool flag) {motorsRunning    = flag ? 1 : 0;}
            void SetReadyToSend      (bool flag) {readyToSend      = flag ? 1 : 0;} // ready to send measured speed vs. time

            static const int MaxNameLength = 8;
    
            char  name [MaxNameLength];
            short readyForMessages;
            short readyToRun; 
            short motorsRunning;
            short readyToSend;
        };
  
        StatusMessage (StatusData& data);
        StatusMessage (StatusData *dataPtr);
        StatusMessage (byte msgBytes []);

        bool ToBytes  (byte byteArray [], int maxLength);
        void ToString (); // prints to console, not a character array
        
        int ByteCount () {return header.ByteCount;}
 
    private:
        MessageHeader header;
        StatusData    data;
};

//*********************************************************************************

#endif
