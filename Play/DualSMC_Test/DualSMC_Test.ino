
//
// Dual_SMC_Test
//

int ProfileDonePin       = 2;
int MotorShieldEnablePin = 4;
int MotorShieldFaultPin  = 12; // low => fault
int RunPin               = 5;
int StopPin              = 6;
int ClearProfilePin      = A4;
int LoadPin              = A5;

void setup() 
{
  Serial.begin (9600);

  for (int i=0; i<=19; i++)
    pinMode (i, INPUT);

  pinMode      (ProfileDonePin, INPUT); 

  pinMode      (MotorShieldEnablePin, OUTPUT); 
  digitalWrite (MotorShieldEnablePin, 0);

  pinMode      (MotorShieldFaultPin, INPUT_PULLUP);
  
  pinMode      (RunPin, OUTPUT); 
  digitalWrite (RunPin, 0);

  pinMode      (StopPin, OUTPUT); 
  digitalWrite (StopPin, 0);

  pinMode      (ClearProfilePin, OUTPUT); 
  digitalWrite (ClearProfilePin, 0);

  pinMode      (LoadPin, OUTPUT); 
  digitalWrite (LoadPin, 0);

  ShowMenu ();
}

void ShowMenu ()
{
    Serial.println (" ");
    Serial.println ("1: ClearProfilePin");
    Serial.println ("2: LoadPin");
    Serial.println ("3: StopPin");
    Serial.println ("4: RunPin");
    Serial.println ("5: Enable = 1");
    Serial.println ("6: Enable = 0");
}

unsigned long startTime = 0;
bool lookingForDone = false;
char ostr [20];

bool motorFault = false;

void loop() 
{
    int faultBar = digitalRead (MotorShieldFaultPin);

    if (faultBar == 0)
    {
        if (motorFault == false)
        {
            motorFault = true;
            digitalWrite (MotorShieldEnablePin, 0); 
            Serial.println ("Motor Fault");
        }
    }
    else if (motorFault)
    {
        motorFault = false;
        Serial.println ("Motor Fault cleared");
    }

    //********************************************************************
    
    if (lookingForDone)
    {
        int d = digitalRead (ProfileDonePin);

        if (d == 1)
        {
            lookingForDone = false;
            unsigned long dur = millis () - startTime;
            sprintf (ostr, "%d", dur / 1000);
            Serial.print ("Done: ");
            Serial.println (ostr);            
        }
    }
    
    int count = Serial.available ();

    if (count > 0)
    {
        int pin = -1; //pin to be pulsed
        char c = Serial.read ();

        switch (c)
        {
           case '1': pin = ClearProfilePin; break;
           case '2': pin = LoadPin; break;
           case '3': pin = StopPin; lookingForDone = false; break;
           case '4': pin = RunPin; startTime = millis (); lookingForDone = true; break;
           
           case '5': digitalWrite (MotorShieldEnablePin, 1); break;
           case '6': digitalWrite (MotorShieldEnablePin, 0); break;
           
           default: break;
        }
    
        if (pin != -1)
        {
            digitalWrite (pin, 1);
            delay (10);
            digitalWrite (pin, 0);
        }
        
        ShowMenu ();
    }
}
