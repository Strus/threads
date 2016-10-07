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

enum mythread_state
{
    MYTHREAD_STATE_ACTIVE,
    MYTHREAD_STATE_PREEMPTED,
    MYTHREAD_STATE_DEAD
};

typedef struct
{
    unsigned int id;
    ucontext_t context;
    mythread_func func;
    enum mythread_state state;
} mythread_t;

#endif