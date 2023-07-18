
//
// Loopback0 - test RS-485 board
//

const int Out0 = A3;
const int Out1 = A5;
const int Out2 =  5;
const int Out3 =  6;

const int In0 =  2;
const int In1 = A2;
const int In2 = A4;
const int In3 =  3;


void setup() 
{
  Serial.begin (9600);

  pinMode (Out0, OUTPUT);
  pinMode (Out1, OUTPUT);
  pinMode (Out2, OUTPUT);
  pinMode (Out3, OUTPUT);
  pinMode (In0, INPUT);
  pinMode (In1, INPUT);
  pinMode (In2, INPUT);
  pinMode (In3, INPUT);
}

const byte patterns [] = 
{
  1, 2, 4, 8
};

int index = 0;

void loop() 
{
  byte outByte = patterns [index++];
  index &= 3;

  digitalWrite (Out0, (outByte >> 0) & 1);
  digitalWrite (Out1, (outByte >> 1) & 1);
  digitalWrite (Out2, (outByte >> 2) & 1);
  digitalWrite (Out3, (outByte >> 3) & 1);
   
  byte bit0 = digitalRead (In0);
  byte bit1 = digitalRead (In1);
  byte bit2 = digitalRead (In2);
  byte bit3 = digitalRead (In3);
  
 // byte inByte = (bit3 << 3) | (bit2 << 2) | (bit1 << 1) | bit0;

  Serial.print   (outByte, HEX);
  Serial.print   (", ");
  Serial.print   (bit3);
  Serial.print   (bit2);
  Serial.print   (bit1);
  Serial.println (bit0);
 // Serial.println (inByte, HEX);

  delay (1000);
}
