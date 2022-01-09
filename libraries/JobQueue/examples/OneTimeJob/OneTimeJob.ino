
//
// OneTimeJob.ino - demonstrate JobQueue for one-shot tasks
//

//#include <JobQueue.h>
#include "JobQueue.h"

JobQueue OneTimeJobs;

char *text = "ABCD";

void setup (void) 
{
    Serial.begin (9600);
    Serial.println ("setup");

    OneTimeJobs.Add (PrintJob0, "Hello");    
    OneTimeJobs.Add (PrintJob0, "Late Hello", 10000);    
    OneTimeJobs.Add (PrintJob0, text, 15000);    
}

void loop(void) 
{
    OneTimeJobs.RunJobs (millis ());
}

void PrintJob0 (char *msg)
{
    Serial.print   ("PrintJob - ");  
    Serial.println (msg);  
}
