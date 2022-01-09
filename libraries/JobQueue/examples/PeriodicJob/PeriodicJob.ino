
//
// PeriodicJob.ino - demonstrate periodic job queue
//

#include "JobQueue.h"

PeriodicJobQueue RepeatingJobs;

Method task;
char  *taskArg  = "Text To Print";
char  *taskName = "P2";

void setup (void) 
{
    Serial.begin(9600);
    Serial.println ("setup");

    task = PrintJob;
    
    RepeatingJobs.Add (task, taskArg, taskName, 
                       1000,     // period, milliseconds
                       5000,     // delay before starting, milliseconds
                       5000);    // lifetime, milliseconds
}

void loop(void) 
{
    RepeatingJobs.RunJobs (millis ());
}

void PrintJob (char *msg)
{
    Serial.print   ("PrintJob - ");  
    Serial.println (msg);  
}
