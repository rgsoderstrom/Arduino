
//
// Messages.h
//

//#ifndef MESSAGES_H
//#define MESSAGES_H
//
//#include <MessageHeader.h>
//
//class EncoderCountsMessage
//{
//   public:
//     struct Batch
//     {
//         struct Sample
//         {
//             byte enc1; 
//             byte enc2;
//         };
//   
//         static const int MaxNumberSamples = 16;
//   
//         short put;  // number of samples in this batch
//         short lastBatch; // non-zero means this is last batch
//         Sample counts [MaxNumberSamples];
//     };
// 
//   public:
//     EncoderCountsMessage ();
//     EncoderCountsMessage (byte fromBytes []);
//
//     bool IsLastBatch  () {return data.lastBatch != 0 ? true : false;}
//     void SetLastBatch () {data.lastBatch = 1;}
//     void Clear        () {data.put = 0; data.lastBatch = 0; memset (data.counts, 0, Batch::MaxNumberSamples * sizeof (Batch::Sample));}
//     
//     bool Add (byte e1, byte e2);
//
//     bool ToBytes  (byte byteArray [], int maxLength);
//     void ToString (); // prints to console, not a character array
//     
//   private:
//     MessageHeader header;
//     Batch         data;      
//};
//
//#endif
