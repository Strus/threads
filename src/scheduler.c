///////////////////////////////////////////////////////////////////////////////
///
///    File: scheduler.c
///    Date: 04 Oct 2016
///
///    Copyright (C) 2016 Adrian Kepka <kepka.adrian@gmail.com>
///
///////////////////////////////////////////////////////////////////////////////

#include "scheduler.h"
#include "debug/logging.h"
#include "thread.h"

#include <malloc.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

scheduler_t scheduler;

void myscheduler_init()
{
    scheduler.numberOfThreads = 0;
    scheduler.started = false;
    scheduler.carousel = (threadcarousel_t*) malloc(sizeof(threadcarousel_t));
    scheduler.currentThread = NULL;
    carousel_init(scheduler.carousel);
}
void myscheduler_start()
{
    signal(SIGALRM, scheduler_alarm_signal_handler);

    scheduler.started = true;

    getcontext(&scheduler.context);

    if(scheduler.numberOfThreads > 0)
    {
        scheduler_switch_to_next_thread();
    }
}

void scheduler_register_thread(mythread_t* thread)
{
    scheduler.numberOfThreads++;
    threadcarousel_node_t* newThreadNode = (threadcarousel_node_t*) malloc(sizeof(threadcarousel_node_t));
    newThreadNode->thread = thread;
    newThreadNode->thread->id = scheduler.numberOfThreads;
    carousel_insert(scheduler.carousel, newThreadNode);
}

void scheduler_switch_to_next_thread()
{
    if(!scheduler.started)
    {
        return;
    }

    if(scheduler.currentThread)
    {
        scheduler.currentThread->state = MYTHREAD_STATE_PREEMPTED;
        getcontext(&scheduler.currentThread->context);
        if(scheduler.currentThread->state == MYTHREAD_STATE_PREEMPTED)
        {
            carousel_switch_to_next(scheduler.carousel);
        }
        else
        {
            return;
        }
    }
    scheduler.currentThread = scheduler.carousel->current->thread;
    scheduler.currentThread->state = MYTHREAD_STATE_ACTIVE;

    ualarm(SCHEDULER_PREEMPTION_INTERVAL_USECONDS, 0);

    setcontext(&scheduler.currentThread->context);
}

void scheduler_alarm_signal_handler(int signal)
{
    if(signal != SIGALRM)
    {
        return;
    }

    scheduler_switch_to_next_thread();
}

ucontext_t* scheduler_get_main_context()
{
    return &scheduler.context;
}
