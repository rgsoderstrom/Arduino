

// Flasher.cpp

#include "Flasher.h"

Flasher::Flasher (int p, int d)
{
	pin = p;
	duration = d;
}

void Flasher::flash (int count)
{
	for (int i=0; i<count; i++)
	{
		digitalWrite (pin, HIGH);
		delay (duration);
		digitalWrite (pin, LOW);
		delay (duration);
	}
}

