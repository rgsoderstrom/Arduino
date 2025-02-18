
//
// DallasTemperatureSim.cpp
//

/****
 
#include <Arduino.h>
#include <math.h>
#include "DallasTemperatureSim.h"

unsigned long DallasTemperature::startTime = 0;

DallasTemperature::DallasTemperature (OneWire* oneWire)
{
  startTime = millis();
  period = 6; // minutes
}

//*******************************************************************************

float DallasTemperature::getTempCByIndex (int index) 
{
  unsigned long timeNow = millis ();
  unsigned long t = timeNow - startTime;
  float minutes = (t / 1000.0) / 60;
  
  return 10.0 + 5 * sin (2 * PI * minutes / period);
}

****/
