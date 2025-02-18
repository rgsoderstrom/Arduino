
// RegisterPlay - port IO configuration

void setup() 
{
    char obuf [50];
    
    pinMode (2,  OUTPUT);
    pinMode (3,  OUTPUT);
    pinMode (4,  OUTPUT);
    pinMode (5,  OUTPUT);
    pinMode (6,  OUTPUT);
    pinMode (7,  OUTPUT);
    pinMode (8,  OUTPUT);
    pinMode (9,  OUTPUT);
    pinMode (10, OUTPUT);
    pinMode (11, OUTPUT);
    pinMode (12, OUTPUT);
    pinMode (13, OUTPUT);

/***
    pinMode (3, INPUT);
    pinMode (4, INPUT);
    pinMode (5, INPUT);
    pinMode (6, INPUT);
    pinMode (7, INPUT);
    pinMode (8, INPUT);
***/

    Serial.begin (9600);
    
    uint8_t pa = PORTA.DIR;
    uint8_t pb = PORTB.DIR;
    uint8_t pc = PORTC.DIR;
    uint8_t pd = PORTD.DIR;
    uint8_t pe = PORTE.DIR;
    uint8_t pf = PORTF.DIR;

    sprintf (obuf, "port A = %x", pa); Serial.println (obuf); // 195
    sprintf (obuf, "port B = %x", pb); Serial.println (obuf); // 31
    sprintf (obuf, "port C = %x", pc); Serial.println (obuf); // 64
    sprintf (obuf, "port D = %x", pd); Serial.println (obuf); // 64
    sprintf (obuf, "port E = %x", pe); Serial.println (obuf); // 15
    sprintf (obuf, "port F = %x", pf); Serial.println (obuf); // 52

    Serial.println ("---------------------------");
    
    pinMode (2,  INPUT);
    pinMode (3,  INPUT);
    pinMode (4,  INPUT);
    pinMode (5,  INPUT);
    pinMode (6,  INPUT);
    pinMode (7,  INPUT);
    pinMode (8,  INPUT);
    pinMode (9,  INPUT);
    pinMode (10, INPUT);
    pinMode (11, INPUT);
    pinMode (12, INPUT);
    pinMode (13, INPUT);

    pa = PORTA.DIR;
    pb = PORTB.DIR;
    pc = PORTC.DIR;
    pd = PORTD.DIR;
    pe = PORTE.DIR;
    pf = PORTF.DIR;

    PORTA.PIN0CTRL |= PORT_PULLUPEN_bm;

    sprintf (obuf, "port A = %x", pa); Serial.println (obuf); // 192
    sprintf (obuf, "port B = %x", pb); Serial.println (obuf); // 24
    sprintf (obuf, "port C = %x", pc); Serial.println (obuf); // 0
    sprintf (obuf, "port D = %x", pd); Serial.println (obuf); // 64
    sprintf (obuf, "port E = %x", pe); Serial.println (obuf); // 0
    sprintf (obuf, "port F = %x", pf); Serial.println (obuf); // 4
}

void loop() 
{
    
}
