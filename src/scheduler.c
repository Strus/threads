///////////////////////////////////////////////////////////////////////////////
///
///    File: scheduler.c
///    Date: 04 Oct 2016
///
///    Copyright (C) 2016 Adrian Kepka <kepka.adrian@gmail.com>
///
///////////////////////////////////////////////////////////////////////////////

#include "scheduler.h"
#include "thread.h"

#include <malloc.h>
#include <unistd.h>

scheduler_t scheduler;

void myscheduler_init()
{
    scheduler.numberOfThreads = 0;
    scheduler.started = false;
    scheduler.carousel = (threadcarousel_t*) malloc(sizeof(threadcarousel_t));
    carousel_init(scheduler.carousel);
}
void myscheduler_start()
{
    signal(SIGALRM, scheduler_alarm_signal_handler);

    scheduler.started = true;

    getcontext(&scheduler.context);

    scheduler.context.uc_link = 0;
    scheduler.context.uc_stack.ss_sp = malloc(MYTHREADS_STACK_SIZE);
    scheduler.context.uc_stack.ss_size = MYTHREADS_STACK_SIZE;
    scheduler.context.uc_stack.ss_flags = 0;

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
    carousel_insert(scheduler.carousel, newThreadNode);
}

void scheduler_switch_to_next_thread()
{
    if(!scheduler.started)
    {
        return;
    }

    ucontext_t* previousContext = &scheduler.carousel->current->thread->context;
    carousel_switch_to_next(scheduler.carousel);

    alarm(SCHEDULER_PREEMPTION_INTERVAL_SECONDS);

    swapcontext(previousContext, &scheduler.carousel->current->thread->context);
}

void scheduler_alarm_signal_handler(int signal)
{
    if(signal != SIGALRM)
    {
        return;
    }

    scheduler_switch_to_next_thread();
}