
#define BitsPerChannel 10

union Received
{
  unsigned long Raw;

  struct
  {
    int flag2 : 1; // raw's LSB goes here
    int flag1 : 1;
    int Chan2 : BitsPerChannel;
    int Chan1 : BitsPerChannel;
    int unused : (32 - 2 * BitsPerChannel - 2); // raw's MSBs go here
  } Formatted;
};


unsigned long pattern = 0x123456L;

Received received;

void setup() 
{
    char str [30];
    Serial.begin (9600);

    received.Raw = pattern;
  
    sprintf (str, "raw = 0x%lx", received.Raw);
    Serial.println (str);
    
    sprintf (str, "chan 1 = 0x%x", received.Formatted.Chan1);
    Serial.println (str);
    
    sprintf (str, "chan 2 = 0x%x", received.Formatted.Chan2);
    Serial.println (str);
    
    sprintf (str, "flag 1 = 0x%x", received.Formatted.flag1 & 1);
    Serial.println (str);
    
    sprintf (str, "flag 2 = 0x%x", received.Formatted.flag2 & 1);
    Serial.println (str);
    
    sprintf (str, "unused = 0x%x", received.Formatted.unused);
    Serial.println (str);
}

void loop() 
{
  // put your main code here, to run repeatedly:

}
