
//
// JobQueue.cpp
//

#include <arduino.h>

#include "JobQueue.h"

//*****************************************************************************************************

Job::Job ()
{
    scheduledTime = 0;
    method = NULL; 
    arg = NULL;
};

//*****************************************************************************************************

JobQueue::JobQueue ()
{
}

//*****************************************************************************************************

// Add - one-time task that runs immediately

bool JobQueue::Add (Method job, MethodArg arg)
{
    return Add (job, arg, 0);
}
    
//*****************************************************************************************************

// Add - one-time task that runs after some delay

bool JobQueue::Add (Method job, MethodArg arg, unsigned long delayTime)
{
    for (int i=0; i<NumberAperiodicJobs; i++)
    {
        if (jobs [i].method == NULL)
        {
            jobs [i].method = job;
            jobs [i].arg = arg;
            jobs [i].scheduledTime = millis () + delayTime;

            return true;
        }
    }
                    
    return false;
}

//*****************************************************************************************************

void JobQueue::Clear ()
{
    for (int i=0; i<NumberAperiodicJobs; i++)
    {
		jobs [i].method = NULL;
    }
}

//*****************************************************************************************************

// RunJobs - check scheduled time of all in queue. Run any scheduled for now or in the past

void JobQueue::RunJobs (unsigned long now)
{
    for (int i=0; i<NumberAperiodicJobs; i++)
    {
        if (jobs [i].method != NULL)
        {
            bool run = (now >= jobs [i].scheduledTime);

            if (run)
            {                
                jobs [i].method (jobs [i].arg);
                jobs [i].method = NULL;
            }
        }
    }
}

//*****************************************************************************************************
//*****************************************************************************************************
//*****************************************************************************************************

PeriodicJob::PeriodicJob ()
{
    name [0] = '\0';
    period = 0;
    stopTime = 0;
}

//*****************************************************************************************************

PeriodicJobQueue::PeriodicJobQueue ()
{
}

//*****************************************************************************************************

bool PeriodicJobQueue::Add (Method    task, 
                            MethodArg arg,
                            char *name, 
                            unsigned long period)
{
    return Add (task, arg, name, period, 0, Forever);
}

//*****************************************************************************************************

bool PeriodicJobQueue::Add (Method    task, 
                            MethodArg arg,
                            char *name, 
                            unsigned long period, 
                            unsigned long startDelay, 
                            unsigned long lifeTime)
{
    for (int i=0; i<NumberPeriodicJobs; i++)
    {
        if (jobs [i].method == NULL)
        {
            PeriodicJob& job = jobs [i];
            unsigned long now = millis ();
            
            job.method = task;
            job.arg    = arg;
            job.scheduledTime = now + startDelay; // time of first invocation
            job.period = period;
            
            strncpy (job.name, name, PeriodicJob::MaxJobNameLength);
            job.name [PeriodicJob::MaxJobNameLength] = '\0';

            if (lifeTime == Forever) job.stopTime = Never;
            else                     job.stopTime = now + startDelay + lifeTime;;
                
            return true;
        }
    }

    return false;
}

//*****************************************************************************************************

void PeriodicJobQueue::Clear ()
{
    for (int i=0; i<NumberPeriodicJobs; i++)
    {
		jobs [i].method = NULL;
    }
}

//*****************************************************************************************************

bool PeriodicJobQueue::Remove (char *name)
{
    for (int i=0; i<NumberPeriodicJobs; i++)
    {
        if (strcmp (name, jobs [i].name) == 0)
        {
            jobs [i].name [0] = '\0';
            jobs [i].method = NULL;
            return true;
        }
    }
    
    return false;
}

//*****************************************************************************************************

// RunJobs - check scheduled time of all in queue. Run any scheduled for now or in the past.
//           Update scheduled time if it will run again

void PeriodicJobQueue::RunJobs (unsigned long now)
{

  //  bool (PeriodicJobQueue::) (*qqq) (char *) = Remove;

    

  
    for (int i=0; i<NumberPeriodicJobs; i++)
    {
        if (jobs [i].method != NULL)
        {
            PeriodicJob& job = jobs [i];
            
            bool run  = (now >= job.scheduledTime);
            bool stop = (now >= job.stopTime);

            if (stop)
            {
               job.method = NULL;
            }
            else if (run)
            {
                job.method (job.arg);
                job.scheduledTime += job.period;
            }
        }
    }
}
