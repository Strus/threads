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

#define SCHEDULER_PREEMPTION_INTERVAL_SECONDS 5

/// @brief Represents scheduler.
typedef struct
{
    threadcarousel_t* carousel;
    size_t numberOfThreads;
    bool started;
    ucontext_t context;
} scheduler_t;

/// @brief Registers thread in scheduler.
/// @param thread Thread to register.
void scheduler_register_thread(mythread_t* thread);

void scheduler_switch_to_next_thread();

void scheduler_alarm_signal_handler(int signal);

#endif
