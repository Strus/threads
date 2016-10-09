///////////////////////////////////////////////////////////////////////////////
///
///    File: thread.h
///    Date: 04 Oct 2016
///
///    Copyright (C) 2016 Adrian Kepka <kepka.adrian@gmail.com>
///
///////////////////////////////////////////////////////////////////////////////

#ifndef THREADS_THREAD_H
#define THREADS_THREAD_H

#include "mythreads.h"

#include <ucontext.h>

#define MYTHREADS_STACK_SIZE 1024*64 //< 64kB stack

/// @brief Defines thread state
///        MYTHREAD_STATE_NOT_STARTED - thread was created but never started yet
///        MYTHREAD_STATE_ACTIVE - thread is processed now
///        MYTHREAD_STATE_PREEMPTED - thread was preemted now
enum mythread_state
{
    MYTHREAD_STATE_NOT_STARTED,
    MYTHREAD_STATE_ACTIVE,
    MYTHREAD_STATE_PREEMPTED
};

/// @brief Represents thread.
typedef struct thread
{
    unsigned int id;
    ucontext_t context;
    enum mythread_state state;
    int priority;
} mythread_t;

#endif