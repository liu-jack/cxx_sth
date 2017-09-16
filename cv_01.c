/*------------------------------ cv_01.c --------------------------------*
On Linux, compile with: 
cc -std=c99 -pthread cv_01.c -o cv_01
 
Check your system documentation how to enable C99 and POSIX threads on 
other Un*x systems.

Copyright Loic Domaigne. 
Licensed under the Apache License, Version 2.0.

This file can check when we call  pthread_cond_signal or call pthread_cond_broadcast,
do we should release pthread mutex first?
http://www.domaigne.com/blog/computing/condvars-signal-with-mutex-locked-or-not/
*--------------------------------------------------------------------------*/

#define _POSIX_C_SOURCE 200112L // use IEEE 1003.1-2004
#include <pthread.h> 
#include <unistd.h>  // sleep()
#include <stdio.h>   
#include <stdlib.h>  // EXIT_SUCCESS
#include <string.h>  // strerror() 
#include <errno.h>

/***************************************************************************/
/* our macro for errors checking                                           */
/***************************************************************************/
#define COND_CHECK(func, cond, retv, errv) \
if ( (cond) ) \
{ \
   fprintf(stderr, "\n[CHECK FAILED at %s:%d]\n| %s(...)=%d (%s)\n\n",\
              __FILE__,__LINE__,func,retv,strerror(errv)); \
   exit(EXIT_FAILURE); \
}
 
#define ErrnoCheck(func,cond,retv)  COND_CHECK(func, cond, retv, errno)
#define PthreadCheck(func,rc) COND_CHECK(func,(rc!=0), rc, rc)
#define FOREVER for(;;) 

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  cv    = PTHREAD_COND_INITIALIZER;
int predicate = 0;
unsigned long nwakeup   = 0; // number of wakeup
unsigned long nspurious = 0; // number of spurious wakeup

/*****************************************************************************/
/* thread - wait on condvar, do stats and reset the condvar                  */
/*****************************************************************************/
void*
thread(void* ignore)
{
   int rc;

   FOREVER {
      // wait that predicate becomes true 
      //
      rc = pthread_mutex_lock(&mutex);
      PthreadCheck("pthread_mutex_lock", rc);
      while (predicate==0) {
         rc = pthread_cond_wait(&cv, &mutex);
         PthreadCheck("pthread_cond_wait", rc);
         nwakeup++;                     // we've been wakeup
         if (predicate==0) nspurious++; // we got a spurious wakeup
      }

      // reset predicate to false 
      //
      predicate=0;
      rc = pthread_mutex_unlock(&mutex);
      PthreadCheck("pthread_mutex_unlock", rc);
   }

   // never reached 
   //
   pthread_exit(NULL);
}


/*****************************************************************************/
/* signal_thread - set predicate to true and signal the condvar              */
/*****************************************************************************/
void*
signal_thread(void* ignore)
{
   int rc;

   FOREVER {
      // set the predicate to true and wakeup one thread
      //
      rc = pthread_mutex_lock(&mutex);
      PthreadCheck("pthread_mutex_lock", rc);
      predicate=1;
      rc = pthread_mutex_unlock(&mutex); // unlock before signal
      PthreadCheck("pthread_mutex_unlock", rc);
      rc = pthread_cond_signal(&cv);
      PthreadCheck("pthread_cond_signal", rc);
   }

   // never reached 
   //
   pthread_exit(NULL);
}


/*****************************************************************************/
/* main- main thread                                                         */
/*****************************************************************************/

const int NTHREADS = 8; // # threads waiting on the condvar 

int
main()
{
   pthread_t tid[NTHREADS];  // threads waiting on  the condvar
   pthread_t tsig;           // thread that signals the condvar
   int       rc;             // return code

   // create our threads
   //
   for (int i=0; i<NTHREADS; i++) {
      rc = pthread_create(tid+i, NULL, thread, NULL);
      PthreadCheck("pthread_create", rc);
   }
   rc = pthread_create(&tsig, NULL, signal_thread, NULL);
   PthreadCheck("pthread_create", rc);

   // wait 3 sec, print statistics and exit
   //
   sleep(3);
   rc = pthread_mutex_lock(&mutex);
   PthreadCheck("pthread_mutex_lock", rc);
   printf("# wakeup   = %8lu\n# spurious = %8lu (%2.2f%%)\n", 
          nwakeup, nspurious, (float)nspurious/nwakeup*100.0
         );
   rc = pthread_mutex_unlock(&mutex);
   PthreadCheck("pthread_mutex_unlock", rc);

   // that's all, folks!
   //
   return EXIT_SUCCESS;
}