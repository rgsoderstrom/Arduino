
//
// DallasTemperatureSim.h
//

/****
 
 
#ifndef DALLASTEMPERATURE_SIM
#define DALLASTEMPERATURE_SIM

#include "OneWireSim.h"

class DallasTemperature
{
  public:
    DallasTemperature (OneWire* oneWire);

    void begin () {}

    void setResolution (int r) {}
    
    void requestTemperatures () {} // command to get temperatures
    
    float getTempCByIndex (int index);

  private:
    static unsigned long startTime; // milliseconds
    float period; // seconds

};

#endif

****/
