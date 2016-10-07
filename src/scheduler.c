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
    scheduler.number_of_threads = 0;
    scheduler.started = false;
    scheduler.carousel = (threadcarousel_t*) malloc(sizeof(threadcarousel_t));
    scheduler.current_thread = NULL;
    carousel_init(scheduler.carousel);
}
void myscheduler_start()
{
    scheduler.started = true;

    getcontext(&scheduler.main_context);

    if(scheduler.number_of_threads == 0)
    {
        return;
    }

    scheduler_remove_dead_thread();
    scheduler_switch_to_next_thread();
}

void scheduler_register_thread(mythread_t* thread)
{
    scheduler_disable_preemption();

    scheduler.number_of_threads++;
    threadcarousel_node_t* new_thread_node = (threadcarousel_node_t*) malloc(sizeof(threadcarousel_node_t));
    new_thread_node->thread = thread;
    new_thread_node->thread->id = scheduler.number_of_threads;
    carousel_insert(scheduler.carousel, new_thread_node);

    scheduler_enable_preemption();
}

void scheduler_disable_preemption()
{
    alarm(0);
}

void scheduler_enable_preemption()
{
    if(scheduler.started)
    {
        signal(SIGALRM, scheduler_alarm_signal_handler);
//        ualarm(SCHEDULER_PREEMPTION_INTERVAL_USECONDS, 0);
        alarm(SCHEDULER_PREEMPTION_INTERVAL_USECONDS);
    }
}

void scheduler_switch_to_next_thread()
{
    scheduler_disable_preemption();
    if(!scheduler.started)
    {
        return;
    }

    if(scheduler.number_of_threads == 0)
    {
        setcontext(&scheduler.main_context);
    }

    if(scheduler.current_thread)
    {
        scheduler.current_thread->state = MYTHREAD_STATE_PREEMPTED;
        getcontext(&scheduler.current_thread->context);
        if(scheduler.current_thread->state == MYTHREAD_STATE_PREEMPTED)
        {
            carousel_switch_to_next(scheduler.carousel);
        }
        else
        {
            scheduler_enable_preemption();
            return;
        }
    }
    scheduler.current_thread = scheduler.carousel->current->thread;
    scheduler.current_thread->state = MYTHREAD_STATE_ACTIVE;

    scheduler_enable_preemption();

    setcontext(&scheduler.current_thread->context);
}

void scheduler_remove_dead_thread()
{
    scheduler_disable_preemption();

    if(scheduler.current_thread && scheduler.current_thread->state == MYTHREAD_STATE_ACTIVE)
    {
        scheduler_current_thread_has_ended();
    }

    scheduler_enable_preemption();
}

void scheduler_alarm_signal_handler(int signal)
{
    if(signal != SIGALRM)
    {
        return;
    }

    scheduler_switch_to_next_thread();
}

ucontext_t* scheduler_get_end_context()
{
    return &scheduler.main_context;
}

void scheduler_current_thread_has_ended()
{
    scheduler_disable_preemption();

    scheduler.number_of_threads--;
    carousel_remove(scheduler.carousel, scheduler.carousel->current);
    scheduler.current_thread = NULL;
    scheduler_switch_to_next_thread();

    scheduler_enable_preemption();
}

int scheduler_kill_thread(int tid)
{
    scheduler_disable_preemption();

    if(scheduler.current_thread->id == tid)
    {
        scheduler_current_thread_has_ended();
        return 0;
    }

    scheduler.number_of_threads--;
    threadcarousel_node_t* thread_to_kill = carousel_find_by_id(scheduler.carousel, tid);
    if(thread_to_kill == NULL)
    {
        LOG("Invalid thread id. Nothing to kill.");
        return -1;
    }
    if(scheduler.current_thread == thread_to_kill->thread)
    {
        scheduler.current_thread = NULL;
    }
    carousel_remove(scheduler.carousel, thread_to_kill);
    scheduler_switch_to_next_thread();

    scheduler_enable_preemption();

    return 0;
}

