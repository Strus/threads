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

#define SCHEDULER_PREEMPTION_INTERVAL_USECONDS 1

/// @brief Represents scheduler.
typedef struct
{
    threadcarousel_t* carousel;
    mythread_t* current_thread;
    size_t number_of_threads;
    bool started;
    ucontext_t main_context;
} scheduler_t;

/// @brief Registers thread in scheduler.
/// @param thread Thread to register.
void scheduler_register_thread(mythread_t* thread);

void scheduler_disable_preemption();

void scheduler_enable_preemption();

void scheduler_switch_to_next_thread();

void scheduler_remove_dead_thread();

void scheduler_alarm_signal_handler(int signal);

ucontext_t* scheduler_get_end_context();

void scheduler_current_thread_has_ended();

int scheduler_kill_thread(int tid);

#endif
