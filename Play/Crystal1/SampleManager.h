
#ifndef SAMPLEMANAGER_H
#define SAMPLEMANAGER_H

#include "TcpClientRev2.h"
#include "Messages.h"

class HistorySample
{
  public:
  
    void SetTemperature (float temp)     {scaledTemp = (temp - BaseTemperature) * TemperatureScale;}
    void SetTime (unsigned long timeTag) {scaledTime = (timeTag - BaseTime) / TimeScale;}

    float         GetTemperature () {return ((float) scaledTemp / TemperatureScale) + BaseTemperature;}
    unsigned long GetTime ()        {return scaledTime * TimeScale + BaseTime;}

    static void SetBaseTime (unsigned long t) {BaseTime = t;}
    
  private:    
    short          scaledTemp;
    unsigned short scaledTime;

    static float BaseTemperature;
    static float TemperatureScale; // scaledTemp units per degree 

    static unsigned long BaseTime;  // about the time history collection starts, millis 
    static unsigned long TimeScale; // 
};

//********************************************************************************************

class SampleManager
{
  public:
    SampleManager (TcpClientRev2 *);

    void Store (unsigned long timeTag, float temperature);
    void SendPartialMessage ();

    void Clear (); 

    bool SendFirstHistoryMessage ();
    bool SendNextHistoryMessage ();
    
  private:
    TemperatureMessage message;

    static const int NumberHistorySamples = 800;
    HistorySample history [NumberHistorySamples];
    int put; 
    int get;

    void AddToMessage (unsigned long timeTag, float temperature);
    void AddToHistory (unsigned long timeTag, float temperature);
};

#endif
