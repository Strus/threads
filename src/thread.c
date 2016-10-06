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

#include <malloc.h>
#include <stdlib.h>

int mythreads_start(mythread_func func, void* args)
{
    mythread_t* thread = (mythread_t*)malloc(sizeof(mythread_t));
    getcontext(&thread->context);
    thread->context.uc_stack.ss_sp = malloc(MYTHREADS_STACK_SIZE);
    thread->context.uc_stack.ss_size = MYTHREADS_STACK_SIZE;
    thread->context.uc_stack.ss_flags = 0;
    if(!thread->context.uc_stack.ss_sp)
    {
        printf("Error creating new thread! malloc() failed!");
        return -1;
    }

    makecontext(&thread->context, (void (*)(void))func, 1, args);

    scheduler_register_thread(thread);

    return thread->id;
}

int mythread_exit(void)
{
    return 0;
}

int mythread_kill(int tid)
{
    return 0;
}
