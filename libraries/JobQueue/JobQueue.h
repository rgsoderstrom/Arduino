
//
// JobQueue.h
//

#ifndef JOBQUEUE_H
#define JOBQUEUE_H

//
// One-time and periodic job queues for static methods
//    - static methods have no "this" pointer
//

//******************************************************************

typedef void *MethodArg;
typedef void (*Method) (MethodArg);

struct Job
{
  Job ();
  
  unsigned long scheduledTime; // run when (millis () >= scheduledTime)  
  Method        method;
  MethodArg     arg;
};

//******************************************************************

struct PeriodicJob : public Job
{
    PeriodicJob ();
    
    static const int MaxJobNameLength = 12;
  
    char name [MaxJobNameLength + 1];  // + 1 ensures room for null termination
    unsigned long period;              // milliseconds
    unsigned long stopTime;            // milliseconds
};

//******************************************************************

class JobQueue
{
  public:
    JobQueue ();

  // execute immediately
    bool Add (Method, MethodArg);

  // execute after startDelay milliseconds
    bool Add (Method, MethodArg, unsigned long startDelay);

    void RunJobs (unsigned long now);
  
  private:
    static const int NumberAperiodicJobs = 8;
    Job jobs [NumberAperiodicJobs];
};

//******************************************************************

class PeriodicJobQueue
{
  public:
    PeriodicJobQueue ();

  // start immediately, until Removed
    bool Add (Method    job,
              MethodArg arg,
              char     *name,
              unsigned long period);
              
    bool Add (Method,
              MethodArg,
              char *name,
              unsigned long period,     // millis
              unsigned long startDelay, // wait this many millis before first invocation
              unsigned long lifetime);  // millis

    bool Remove (char *name);

    void RunJobs (unsigned long now);

  private:
    static const int Forever = 0xffffffff;
    static const int Never   = 0xffffffff;
    
    static const int NumberPeriodicJobs = 6;
    PeriodicJob jobs [NumberPeriodicJobs];
};

#endif
