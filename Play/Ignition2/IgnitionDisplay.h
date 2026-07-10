
/*
	IgnitionDisplay.h - display for ignition controller
*/

#ifndef IGNITIONDISPLAY_H
#define IGNITIONDISPLAY_H

#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>

// these two determined by LCD hardware
#define RowCount 2
#define ColCount 16

#define NumberItems 4
#define MaxLineLength ColCount

class IgnitionDisplay
{
	public:
		IgnitionDisplay ();
		void Init ();

		int ButtonState ();

		void NextMenuItem ();
		void PrevMenuItem ();

		void IncrDataItem ();
		void DecrDataItem ();

		int Delay1 () {return delay1Millis;}
		int Dwell  () {return dwellMillis;}
		int Sparks () {return sparkCount;}
		int Delay2 () {return delay2Millis;}

	private:
		void ShowSelection ();


		char delay1Text  [MaxLineLength];
		char dwellText   [MaxLineLength];
		char sparksText  [MaxLineLength];
		char delay2Text  [MaxLineLength];

		int delay1Millis;
		int dwellMillis;
		int sparkCount;
		int delay2Millis;

		char *text [NumberItems];
		int  *data [NumberItems];

		int select; 
		Adafruit_RGBLCDShield lcd;
};

#endif

	