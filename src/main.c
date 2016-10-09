///////////////////////////////////////////////////////////////////////////////
///
///    File: main.c
///    Date: 4 Sep 2016
///
///    Copyright (C) 2016 Adrian Kepka <kepka.adrian@gmail.com>
///
///////////////////////////////////////////////////////////////////////////////

#include "mythreads.h"
#include "myscheduler.h"
#include "mymutex.h"
#include "debug/logging.h"

#define LOOP_ITERATIONS 1000000
#define LOG_ITERATIONS 0
#define USE_MUTEX 1

mymutex_t mutex;

void firstThread(void* data);
void secondThread(void* data);
void thirdThread(void* data);
void fourthThread(void* data);

int main()
{
    myscheduler_init();

#if USE_MUTEX
    mymutext_init(&mutex);
#endif

    mythreads_start(firstThread, NULL);
    mythreads_start(secondThread, NULL);
    mythreads_start(thirdThread, NULL);

    myscheduler_start();
    return 0;
}

void firstThread(void* data)
{
    LOG("First thread start");

#if USE_MUTEX
    mymutex_lock(&mutex);
#endif

    mythreads_start(fourthThread, NULL);
    for(int i = 0; i <= LOOP_ITERATIONS; i++)
    {
#if LOG_ITERATIONS
        LOG("T1 - %d", i);
#endif
    }

#if USE_MUTEX
    mymutex_unlock(&mutex);
#endif

    LOG("First thread end");
}

void secondThread(void* data)
{
    LOG("Second thread start");

#if USE_MUTEX
    mymutex_lock(&mutex);
#endif

    for(int i = 0; i <= LOOP_ITERATIONS; i++)
    {
#if LOG_ITERATIONS
        LOG("T2 - %d", i);
#endif
        if(i == 100)
        {
            LOG("Second thread end prematurely");
            #if USE_MUTEX
                mymutex_unlock(&mutex);
            #endif
            if(mythread_exit() == -1)
            {
                LOG("Failed to exit from second thread.");
            }
        }
    }

#if USE_MUTEX
    mymutex_unlock(&mutex);
#endif

    LOG("Second thread end");
}

void thirdThread(void* data)
{
    LOG("Third thread start");

#if USE_MUTEX
    mymutex_lock(&mutex);
#endif

    for(int i = 0; i <= LOOP_ITERATIONS - 2; i++)
    {
#if LOG_ITERATIONS
        LOG("T3 - %d", i);
#endif
        if(i == 500)
        {
            LOG("First thread killed");
            if(mythread_kill(1) == -1)
            {
                LOG("Failed to kill first thread.");
            }
        }
    }

#if USE_MUTEX
    mymutex_unlock(&mutex);
#endif

    LOG("Third thread end");
}

void fourthThread(void* data)
{
    LOG("Fourth thread start");

#if USE_MUTEX
    mymutex_lock(&mutex);
#endif

    for(int i = 0; i <= LOOP_ITERATIONS / 2; i++)
    {
#if LOG_ITERATIONS
        LOG("T4 - %d", i);
#endif
    }

#if USE_MUTEX
    mymutex_unlock(&mutex);
#endif

    LOG("Fourth thread end");
}
