
//
// Utils.h
//

#ifndef UTILS_H
#define UTILS_H

class Utils
{
    public:
  		static char* FloatToString (double number, char obuf [])
	  	{
	        int integerPart = (int) number;
          double fraction = fabs (number - integerPart);
          
          sprintf (obuf, "%d", integerPart);

          int i = strlen (obuf);
          obuf [i++] = '.';

          for (int j=0; j<3; j++)
          {
              fraction *= 10;
              obuf [i++] = (int) fraction + '0';
              fraction -= (int) fraction;
          }

          obuf [i] = '\0';
	        return obuf;
		  }
};

#endif
