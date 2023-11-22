///////////////////////////////////////////////////////////////////////////////
///
///    File: main.c
///    Date: 4 Sep 2016
///
///    Copyright (C) 2016 Adrian Kepka <kepka.adrian@gmail.com>
///
///////////////////////////////////////////////////////////////////////////////

#include "logging.h"
#include "mymutex.h"
#include "myscheduler.h"
#include "mythreads.h"

#define LOOP_ITERATIONS 100000
#define LOG_ITERATIONS 0
#define USE_MUTEX 1

mymutex_t mutex;

void firstThread(void *data);
void secondThread(void *data);
void thirdThread(void *data);
void fourthThread(void *data);

int main() {
    if (myscheduler_init() == -1) {
        INFO("Failed to init scheduler. Exiting.");
        return -1;
    }

#if USE_MUTEX
    mymutext_init(&mutex);
#endif

    mythreads_start(firstThread, NULL);
    mythreads_start(secondThread, NULL);
    mythreads_start(thirdThread, NULL);

    return myscheduler_start();
}

void firstThread(void *data) {
    INFO("First thread start");

#if USE_MUTEX
    mymutex_lock(&mutex);
#endif

    mythreads_start(fourthThread, NULL);
    for (int i = 0; i <= LOOP_ITERATIONS; i++) {
#if LOG_ITERATIONS
        INFO("T1 - %d", i);
#endif
    }

#if USE_MUTEX
    mymutex_unlock(&mutex);
#endif

    INFO("First thread end");
}

void secondThread(void *data) {
    INFO("Second thread start");

#if USE_MUTEX
    mymutex_lock(&mutex);
#endif

    for (int i = 0; i <= LOOP_ITERATIONS; i++) {
#if LOG_ITERATIONS
        INFO("T2 - %d", i);
#endif
        if (i == 100) {
            INFO("Second thread will end prematurely");
#if USE_MUTEX
            mymutex_unlock(&mutex);
#endif
            if (mythread_exit() == -1) {
                INFO("Failed to exit from second thread.");
            }
        }
    }

#if USE_MUTEX
    mymutex_unlock(&mutex);
#endif

    INFO("Second thread end");
}

void thirdThread(void *data) {
    INFO("Third thread start");

#if USE_MUTEX
    mymutex_lock(&mutex);
#endif

    for (int i = 0; i <= LOOP_ITERATIONS - 2; i++) {
#if LOG_ITERATIONS
        INFO("T3 - %d", i);
#endif
        if (i == 500) {
            INFO("Third thread tries to kill first thread...");
            if (mythread_kill(1) == -1) {
                INFO("Failed to kill first thread.");
            } else {
                INFO("First thread killed");
            }
        }
    }

#if USE_MUTEX
    mymutex_unlock(&mutex);
#endif

    INFO("Third thread end");
}

void fourthThread(void *data) {
    INFO("Fourth thread start");

#if USE_MUTEX
    mymutex_lock(&mutex);
#endif

    for (int i = 0; i <= LOOP_ITERATIONS / 2; i++) {
#if LOG_ITERATIONS
        INFO("T4 - %d", i);
#endif
    }

#if USE_MUTEX
    mymutex_unlock(&mutex);
#endif

    INFO("Fourth thread end");
}
