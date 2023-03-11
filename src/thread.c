///////////////////////////////////////////////////////////////////////////////
///
///    File: thread.c
///    Date: 04 Oct 2016
///
///    Copyright (C) 2016 Adrian Kepka <kepka.adrian@gmail.com>
///
///////////////////////////////////////////////////////////////////////////////

#include "thread.h"
#include "logging.h"
#include "scheduler.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>

int mythreads_start(mythread_func func, void *args) {
    mythread_t *thread = (mythread_t *)malloc(sizeof(mythread_t));
    if (thread == NULL) {
        ERROR("malloc() failed: %s.", strerror(errno));
        return 0;
    }

    if (getcontext(&thread->context) == -1) {
        ERROR("Error while obtaining thread contex: %s", strerror(errno));
        free(thread);
        return 0;
    }

    thread->context.uc_stack.ss_sp = thread->stack;
    thread->context.uc_stack.ss_size = MYTHREADS_STACK_SIZE;
    thread->context.uc_link = scheduler_get_main_context();

    makecontext(&thread->context, (void (*)(void))func, 1, args);

    if (scheduler_register_thread(thread) == -1) {
        INFO("Failed to register thread in scheduler.");
        free(thread);
        return 0;
    }

    return thread->id;
}

int mythread_exit(void) {
    int tid = scheduler_get_current_thread()->id;

    INFO("Thread with id: %d exiting.", tid);
    return scheduler_kill_thread(tid);
}

int mythread_kill(int tid) {
    INFO("Killing thread with id: %d", tid);
    return scheduler_kill_thread(tid);
}
