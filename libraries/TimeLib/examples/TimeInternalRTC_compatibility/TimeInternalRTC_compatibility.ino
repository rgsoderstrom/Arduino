/*
Example of code running in same conditions with ATMEGA and MEGAAVR 0 series indifferently

On the MEGAAVR 0 architecture, the internal clock RTC is used to provide high clock accuracy.
*/

#include <TimeLib.h>

unsigned long actualTime;
unsigned long oldTime;

void setup() {

  setTime(8,15,00,24,04,1981);          // set the current system timestamp from arbitrary date/time

  Serial.begin(9600);                   // init serial
  pinMode(LED_BUILTIN, OUTPUT);         // init built-in led

}

void loop() {

  actualTime = now(); // get actual system timestamp

  if( actualTime != oldTime ) { // make actions if flag is fired:
 
    oldTime = actualTime;

    // display current system date and time:
    Serial.print(year(actualTime));
    Serial.print("-");
    Serial.print(month(actualTime));
    Serial.print("-");
    Serial.print(day(actualTime));
    Serial.print(" ");
    Serial.print(hour(actualTime));
    Serial.print(":");
    Serial.print(minute(actualTime));
    Serial.print(":");
    Serial.println(second(actualTime));

    // blink led:
    digitalWrite(LED_BUILTIN, HIGH);
    delay(250);
    digitalWrite(LED_BUILTIN, LOW);

  }

}
