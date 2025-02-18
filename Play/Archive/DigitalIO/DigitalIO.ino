

// DigitalIO

// pwm 3 5 9 10

// C:\Program Files (x86)\Arduino\hardware\tools\avr\avr\include\avr\iom4809.h

void setup() 
{  
  for (int i=0; i<=19; i++)
    pinMode (i, INPUT);

  int pin1 = 9;
  int pin2 = 10;
  
  unsigned int B0H = TCB0.CCMPH; 
  unsigned int B0L = TCB0.CCMPL; 
  unsigned int B1H = TCB1.CCMPH; 
  unsigned int B1L = TCB1.CCMPL; 
  unsigned int B2H = TCB2.CCMPH; 
  unsigned int B2L = TCB2.CCMPL; 
  unsigned int B3H = TCB3.CCMPH; 
  unsigned int B3L = TCB3.CCMPL; 

  Serial.begin (9600);

  Serial.println ("Before:");
  Serial.print ("B0H: "); Serial.println (B0H, HEX);
  Serial.print ("B0L: "); Serial.println (B0L, HEX);
  Serial.print ("B1H: "); Serial.println (B1H, HEX);
  Serial.print ("B1L: "); Serial.println (B1L, HEX);
  Serial.print ("B2H: "); Serial.println (B2H, HEX);
  Serial.print ("B2L: "); Serial.println (B2L, HEX);
  Serial.print ("B3H: "); Serial.println (B3H, HEX);
  Serial.print ("B3L: "); Serial.println (B3L, HEX);

  
 // pinMode (pin1, OUTPUT);
 // pinMode (pin2, OUTPUT);
  
  //analogWrite (pin1, 10);
  //analogWrite (pin2, 20);

  B0H = TCB0.CCMPH; 
  B0L = TCB0.CCMPL; 
  B1H = TCB1.CCMPH; 
  B1L = TCB1.CCMPL; 
  B2H = TCB2.CCMPH; 
  B2L = TCB2.CCMPL; 
  B3H = TCB3.CCMPH; 
  B3L = TCB3.CCMPL; 

  Serial.println ("\nAfter:");
  Serial.print ("B0H: "); Serial.println (B0H, HEX);
  Serial.print ("B0L: "); Serial.println (B0L, HEX);
  Serial.print ("B1H: "); Serial.println (B1H, HEX);
  Serial.print ("B1L: "); Serial.println (B1L, HEX);
  Serial.print ("B2H: "); Serial.println (B2H, HEX);
  Serial.print ("B2L: "); Serial.println (B2L, HEX);
  Serial.print ("B3H: "); Serial.println (B3H, HEX);
  Serial.print ("B3L: "); Serial.println (B3L, HEX);


  Serial.println ("============================================");


  Serial.println (" ");
}

int f = 0;

void loop() 
{
    f ^= 1;

//    if (f) PORTB.OUTSET = 1;
//    else   PORTB.OUTCLR = 1;
}
