
// UnitTest.ino - for "Motors and Encoders" classes

#include <FPGA_MsgBytes.h>

FPGA_MsgBytes buffer;

unsigned char msg [] = {0x12, 0x34, 0xab, 0x11, 4, 0x22, 0x33, 0x44, 0x55};
//unsigned char msg [] = {0xab, 0x11, 7, 0x22, 0x33, 0x44, 0x55};
int count = sizeof (msg) / sizeof (msg [0]);

void setup() 
{
    Serial.begin(9600);
    Serial.println ("UnitTest setup");

    for (int i=0; i<count; i++)
    {
      char obuf [100];
      FPGA_MsgBytes::BufferState state = buffer.StoreByte (msg [i]);
      sprintf (obuf, "byte = %x, status = %d", msg [i], state);
      Serial.println (obuf);
    }

    unsigned char *bb = buffer.GetBytes ();
    
    for (int i=0; i<count; i++)
    {
      char obuf [100];
      sprintf (obuf, "0x%x, %d", bb [i], bb [i]);
      Serial.println (obuf);
    }
}




void loop() 
{


}
