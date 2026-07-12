
/*
	IgnitionDisplay.cpp
*/

#include <string.h>

#include "IgnitionDisplay.h"
 
IgnitionDisplay::IgnitionDisplay ()
{
}

void IgnitionDisplay::Init ()
{
    lcd = Adafruit_RGBLCDShield ();
    lcd.begin (ColCount, RowCount);
    //lcd.clear() ();
    lcd.print("Hello, TIM-6!");

    strcpy (delay1Text, "Delay1");
    strcpy (dwellText,  "Dwell");
    strcpy (sparksText, "Sparks");
    strcpy (delay2Text, "Delay2");

	delay1Millis = 10;
	dwellMillis  = 2;
	sparkCount   = 3;
	delay2Millis = 100; 

    text [0] = delay1Text;
    text [1] = dwellText;
    text [2] = sparksText;
    text [3] = delay2Text;

    data [0] = &delay1Millis;
    data [1] = &dwellMillis;
    data [2] = &sparkCount;
    data [3] = &delay2Millis;
     
    select = 0;
    ShowSelection ();
}

int IgnitionDisplay::ButtonState ()
{
    return lcd.readButtons ();
}

void IgnitionDisplay::NextMenuItem ()
{
    if (++select == NumberItems) select = 0;
    ShowSelection ();
}
		
void IgnitionDisplay::PrevMenuItem ()
{
    if (--select < 0) select = NumberItems - 1;
    ShowSelection ();
}

void IgnitionDisplay::IncrDataItem ()
{
    (*data [select])++;
    ShowSelection ();
}

void IgnitionDisplay::DecrDataItem ()
{
    (*data [select])--;
    ShowSelection ();
}

void IgnitionDisplay::ShowSelection ()
{
    char obuf [MaxLineLength];
    lcd.clear ();
    lcd.setCursor (0, 0); // (col, row)
    lcd.print (text [select]);
    lcd.setCursor (0, 1);
    sprintf (obuf, "%2d", *data [select]);
    lcd.print (obuf);
}    	
