
// RegisterPlay2 - Timer B0

void setup() 
{
    Serial.begin (9600);
    
    pinMode (2,  OUTPUT);

  // https://forum.arduino.cc/index.php?topic=614128.0
    TCB0.CTRLB = TCB_CNTMODE_INT_gc; // Use "Periodic Interrupt" mode

    // input clock rate (from TCA0) is 250kHz
    TCB0.CCMP = 2500; //25000; // Value to compare with. This is 1/10th of the tick rate, so 10 Hz
    TCB0.INTCTRL = TCB_CAPT_bm; // Enable the interrupt
    TCB0.CTRLA = TCB_CLKSEL_CLKTCA_gc | TCB_ENABLE_bm; // Use Timer A as clock, enable timer

}

int obit = 0;
unsigned long count = 0;

ISR (TCB0_INT_vect)
{
    digitalWrite (2, obit & 1);
    obit ^= 1;
    count++;
    
   // Clear interrupt flag
   TCB0.INTFLAGS = TCB_CAPT_bm;
}

void loop() 
{
    delay (1000);
    Serial.println (count);
}
