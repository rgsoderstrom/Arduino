
//
// Loopback
//

//
// "in" and "out" are relative to the FPGA, not the Arduino!
//

const int BitsPerWord = 8;

const int OutputDataBit    = A2;
const int OutputShiftClock = A3;
const int LastBit          = A4;
const int FirstBit         = 2;

const int InputDataBit    = A5;
const int InputShiftClock = 6;
const int InputDone       = 5;

void setup() 
{
  Serial.begin (9600);

  pinMode (OutputDataBit,    INPUT);
  pinMode (OutputShiftClock, OUTPUT);
  pinMode (FirstBit, INPUT);
  pinMode (LastBit,  INPUT);

  pinMode (InputDataBit,    OUTPUT);
  pinMode (InputShiftClock, OUTPUT);
  pinMode (InputDone,       OUTPUT);

  digitalWrite (OutputShiftClock, 0);
  digitalWrite (InputDataBit,     0);
  digitalWrite (InputShiftClock,  0);
  digitalWrite (InputDone,        0);
}

const byte patterns [] = 
{
  0x34, 0xcd, 0x65, 0x0fe
};

const unsigned int NumberPatterns = sizeof (patterns) / sizeof (patterns [0]);

int sel = 0;
  
void loop() 
{
  for (sel=0; sel<NumberPatterns; sel++)
  {
    for (int i=0; i<BitsPerWord; i++)
    {
      unsigned int mask = 1 << (BitsPerWord - 1 - i);
      unsigned int b = patterns [sel] & mask;
      
      digitalWrite (InputDataBit, b != 0 ? 1 : 0);
      digitalWrite (InputShiftClock, 1);
      digitalWrite (InputShiftClock, 0);        
    }
  
    digitalWrite (InputDone, 1);
    digitalWrite (InputDone, 0);    
  }
  


  while (digitalRead (FirstBit) != 0)
  {
    unsigned int received = 0;
    
    for (int i=0; i<BitsPerWord; i++)
    {
      unsigned int bit = digitalRead (OutputDataBit);
      bit &= 1;
  
      if (bit != 0)
        received |= (1 << (BitsPerWord - 1 - i));
        
      digitalWrite (OutputShiftClock, 1);
      digitalWrite (OutputShiftClock, 0);    
    }    
  
    Serial.println (received, HEX);
  }

  Serial.println ("----------------");
  
  delay (1000);
}
