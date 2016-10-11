///////////////////////////////////////////////////////////////////////////////
///
///    File: thread.c
///    Date: 04 Oct 2016
///
///    Copyright (C) 2016 Adrian Kepka <kepka.adrian@gmail.com>
///
///////////////////////////////////////////////////////////////////////////////

#include "thread.h"
#include "scheduler.h"
#include "logging.h"

#include <malloc.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int mythreads_start(mythread_func func, void* args)
{
    mythread_t* thread = (mythread_t*)malloc(sizeof(mythread_t));
    if(thread == NULL)
    {
        ERROR("malloc() failed: %s. Aborting!", strerror(errno));
        abort();
    }

    if(getcontext(&thread->context) == -1)
    {
        ERROR("Error while obtaining thread contex: %s", strerror(errno));
        free(thread);
        return 0;
    }

    thread->context.uc_stack.ss_sp = thread->stack;
    thread->context.uc_stack.ss_size = MYTHREADS_STACK_SIZE;
    thread->context.uc_link = scheduler_get_main_context();
    if(thread->context.uc_stack.ss_sp == NULL)
    {
        ERROR("malloc() failed: %s. Aborting!", strerror(errno));
        abort();
    }

    makecontext(&thread->context, (void (*)(void))func, 1, args);

    scheduler_register_thread(thread);

    return thread->id;
}

int mythread_exit(void)
{
    int tid = scheduler_get_current_thread()->id;

    INFO("Thread with id: %d exited.", tid);
    return scheduler_kill_thread(tid);
}

int mythread_kill(int tid)
{
    INFO("Killing thread with id: %d", tid);
    return scheduler_kill_thread(tid);
}
