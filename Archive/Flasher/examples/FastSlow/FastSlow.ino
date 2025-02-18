#include <Flasher.h>

int ledPin = 13;
int slowDuration = 300;
int fastDuration = 100;

Flasher slowFlasher (ledPin, slowDuration);
Flasher fastFlasher (ledPin, fastDuration);

void setup ()
{
}

void loop ()
{
  slowFlasher.flash (5);
  fastFlasher.flash (5);
  delay (1000);
}


