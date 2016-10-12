///////////////////////////////////////////////////////////////////////////////
///
///    File: scheduler.c
///    Date: 04 Oct 2016
///
///    Copyright (C) 2016 Adrian Kepka <kepka.adrian@gmail.com>
///
///////////////////////////////////////////////////////////////////////////////

#include "scheduler.h"
#include "logging.h"
#include "thread.h"

#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

scheduler_t scheduler;

void myscheduler_init()
{
    scheduler.number_of_threads = 0;
    scheduler.started = false;

    scheduler.carousel = (threadcarousel_t*) malloc(sizeof(threadcarousel_t));
    if(scheduler.carousel == NULL)
    {
        ERROR("malloc() failed: %s. Aborting!", strerror(errno));
        abort();
    }

    scheduler.pending_carousel = (threadcarousel_t*) malloc(sizeof(threadcarousel_t));
    if(scheduler.pending_carousel == NULL)
    {
        ERROR("malloc() failed: %s. Aborting!", strerror(errno));
        abort();
    }

    carousel_init(scheduler.carousel);
    carousel_init(scheduler.pending_carousel);

    scheduler.current_thread = NULL;
    scheduler.dead_thread = NULL;
}
void myscheduler_start()
{
    INFO("Starting scheduler with %d threads", scheduler.number_of_threads);
    scheduler.started = true;

    if(getcontext(&scheduler.main_context) == -1)
    {
        ERROR("Unable to obtain scheduler main context: %s\n Aborting!", strerror(errno));
        abort();
    }

    // If any thread will end then it will return here, because every thread context is linked to scheduler main context.
    // In case of such situation we remove this thread from scheduler.
    scheduler_remove_returned_thread();
    scheduler_switch_to_next_thread();

    free(scheduler.carousel);
    free(scheduler.pending_carousel);
}

void scheduler_register_thread(mythread_t* thread)
{
    scheduler_disable_preemption();

    scheduler.number_of_threads++;
    threadcarousel_node_t* new_thread_node = (threadcarousel_node_t*) malloc(sizeof(threadcarousel_node_t));
    if(new_thread_node == NULL)
    {
        ERROR("malloc() failed: %s. Aborting!", strerror(errno));
        abort();
    }
    new_thread_node->thread = thread;
    new_thread_node->thread->id = scheduler.number_of_threads;
    new_thread_node->thread->state = MYTHREAD_STATE_NOT_STARTED;
    new_thread_node->thread->priority = 0;
    carousel_insert(scheduler.carousel, new_thread_node);

    INFO("New thread added, id: %d. Total threads: %d", new_thread_node->thread->id, scheduler.number_of_threads);

    scheduler_enable_preemption();
}

mythread_t* scheduler_get_current_thread()
{
    return scheduler.current_thread;
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
        ualarm(SCHEDULER_PREEMPTION_INTERVAL_USECONDS, 0);
    }
}

void scheduler_switch_to_next_thread()
{
    scheduler_disable_preemption();
    if(!scheduler.started || scheduler.number_of_threads == 0)
    {
        return;
    }

    scheduler_remove_dead_thread();

    if(scheduler.current_thread)
    {
        scheduler.current_thread->state = MYTHREAD_STATE_PREEMPTED;

        // Save current thread context. Thread will start from here next time.
        if(getcontext(&scheduler.current_thread->context) == -1)
        {
            ERROR("Unable to save current thread context: %s\n Aborting!", strerror(errno));
            abort();
        }

        // If thread is active it means that we just switched to it, so we return from function and continue execution.
        if(scheduler.current_thread->state == MYTHREAD_STATE_ACTIVE)
        {
            return;
        }

        carousel_switch_to_next(scheduler.carousel);
    }

    // This check and logic is needed because I allow to exit from thread with mythread_exit()
    // in the middle of thread function.
    // If we kill last thread with mythread_exit(), then no more threads are available and we should get back to
    // scheduler main context (so where thread context would normally switch after returning from thread function).
    if(scheduler.carousel->current == NULL)
    {
        INFO("No more threads in carousel, exiting.");
        if(setcontext(&scheduler.main_context) == -1)
        {
            ERROR("Unable to set thread context: %s\n Aborting!", strerror(errno));
            abort();
        }
        return;
    }

    scheduler.current_thread = scheduler.carousel->current->thread;
    scheduler.current_thread->state = MYTHREAD_STATE_ACTIVE;

    scheduler_enable_preemption();

    INFO("Next thread will be: %d", scheduler.current_thread->id);
    if(setcontext(&scheduler.current_thread->context) == -1)
    {
        ERROR("Unable to set thread context: %s\n Aborting!", strerror(errno));
        abort();
    }
}

void scheduler_remove_dead_thread()
{
    if(scheduler.dead_thread == NULL)
    {
        return;
    }

    threadcarousel_node_t* node_to_remove = carousel_find_by_id(scheduler.carousel, scheduler.dead_thread->id);
    if(node_to_remove == NULL)
    {
        ERROR("Cannot remove dead thread because it does not exists in carousel. Aborting because something went horribly wrong.");
        abort();
    }

    carousel_remove(scheduler.carousel, node_to_remove);
    if(scheduler.dead_thread == scheduler.current_thread)
    {
        scheduler.current_thread = NULL;
    }
    scheduler.dead_thread = NULL;
    scheduler.number_of_threads--;
}

void scheduler_remove_returned_thread()
{
    scheduler_disable_preemption();

    if(scheduler.current_thread && scheduler.current_thread->state == MYTHREAD_STATE_ACTIVE)
    {
        INFO("Thread with id = %d has ended. Removing from carousel.", scheduler.current_thread->id);
        scheduler.dead_thread = scheduler.current_thread;
        scheduler_remove_dead_thread();
    }

    scheduler_enable_preemption();
}

void scheduler_make_current_thread_pending()
{
    scheduler.current_thread->state = MYTHREAD_STATE_PENDING;

    INFO("Thread with id: %d will be moved from active list to pending list in scheduler.", scheduler.current_thread->id);
    carousel_move_to_another_carousel(scheduler.carousel, scheduler.pending_carousel, scheduler.carousel->current);

    scheduler.current_thread = NULL;
}

void scheduler_remove_one_thread_from_pending_list()
{
    if(scheduler.pending_carousel->tail == NULL)
    {
        return;
    }

    INFO("Thread with id: %d will be moved from active list to pending list in scheduler.", scheduler.pending_carousel->tail->thread->id);
    carousel_move_to_another_carousel(scheduler.pending_carousel, scheduler.carousel, scheduler.pending_carousel->tail);
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
    return &scheduler.main_context;
}

int scheduler_kill_thread(int tid)
{
    scheduler_disable_preemption();

    threadcarousel_node_t* node_to_kill = carousel_find_by_id(scheduler.carousel, tid);
    if(node_to_kill == NULL)
    {
        INFO("Invalid thread id. Nothing to kill.");
        return -1;
    }

    scheduler.dead_thread = node_to_kill->thread;
    // if we kill current thread then we switch to next one
    if(scheduler_get_current_thread()->id == scheduler.dead_thread->id)
    {
        scheduler_switch_to_next_thread();
    }

    scheduler_enable_preemption();

    return 0;
}

