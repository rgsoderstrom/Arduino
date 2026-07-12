/*
    IgnitionTiming
*/

#define N 10

// digital pin numbers
const int PointsPin  = 3; // goes low when points close  

int debounceDelay = 5; // millis

volatile bool IntFlag = false;

void setup() 
{
    Serial.begin (9600);

    pinMode (PointsPin,  INPUT_PULLUP); 
    attachInterrupt (digitalPinToInterrupt (PointsPin), myISR, CHANGE); // FALLING);

    Serial.println ("Ready");
}

unsigned long closingTimes [N];
int put = 0;
int ignore = 3;
int count = 0;

unsigned long lastIntTime = 0;

void loop() 
{
    if (IntFlag == true)
    {
        unsigned long now = millis ();

        IntFlag = false;

        if (now > lastIntTime + debounceDelay)
        {
            lastIntTime = now;

            if (++count > ignore)
            {
                closingTimes [put++] = now;

                if (put == N)
                {
                    detachInterrupt (digitalPinToInterrupt (PointsPin));

                    for (int i=1; i<N; i++)
                        Serial.println (closingTimes [i] - closingTimes [i-1]);

                    Serial.println ("done");
                }
            }
        }
    }
}

void myISR ()
{
    IntFlag = true;
}
