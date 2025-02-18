
// Flasher.h - LED flasher

//#include "WProgram.h"
#include "arduino.h"

class Flasher
{
	public:
		Flasher (int _pin, int _duration);
		void flash (int count);

	private:
		int pin;
		int duration;
};
