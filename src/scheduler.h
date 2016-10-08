///////////////////////////////////////////////////////////////////////////////
///
///    File: scheduler.h
///    Date: 04 Oct 2016
///
///    Copyright (C) 2016 Adrian Kepka <kepka.adrian@gmail.com>
///
///////////////////////////////////////////////////////////////////////////////

#ifndef THREADS_SCHEDULER_H
#define THREADS_SCHEDULER_H

#include "myscheduler.h"
#include "thread.h"
#include "utils/carousel.h"

#include <stdbool.h>
#include <ucontext.h>
#include <signal.h>

#define SCHEDULER_PREEMPTION_INTERVAL_USECONDS 1000

/// @brief Represents simple round-robin scheduler.
typedef struct
{
    threadcarousel_t* carousel;
    mythread_t* current_thread;
    mythread_t* dead_thread;
    unsigned int number_of_threads;
    bool started;
    ucontext_t main_context;
} scheduler_t;

/// @brief Registers thread in scheduler.
/// @param thread Thread to register.
void scheduler_register_thread(mythread_t* thread);

/// @brief Returns current thread selected in scheduler.
/// @return Current thread selected in scheduler.
mythread_t* scheduler_get_current_thread();

/// @brief Disables preemption.
void scheduler_disable_preemption();

/// @brief Enables and schedules next preemption.
void scheduler_enable_preemption();

/// @brief Switches scheduler to next thread.
void scheduler_switch_to_next_thread();

/// @brief Removes dead thread node.
void scheduler_remove_dead_thread();

/// @brief Removes thread that ended on it's own.
void scheduler_remove_returned_thread();

/// @brief Handler for alarm signal. Switches scheduler to next thread.
/// @param signal Signal number.
void scheduler_alarm_signal_handler(int signal);

/// @brief Returns main scheduler context. Every thread context should link to this context.
/// @return Pointer to main scheduler context.
ucontext_t* scheduler_get_main_context();

/// @brief Kills thread of given id.
/// @param tid Id of thread to kill
/// @return Nothing when successful
///         -1 on error.
int scheduler_kill_thread(int tid);

#endif
