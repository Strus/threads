///////////////////////////////////////////////////////////////////////////////
///
///    File: mutex.c
///    Date: 09 Oct 2016
///
///    Copyright (C) 2016 Adrian Kepka <kepka.adrian@gmail.com>
///
///////////////////////////////////////////////////////////////////////////////

#include "mymutex.h"
#include "scheduler.h"

#include <stdlib.h>
#include <mymutex.h>
#include <debug/logging.h>

void mymutext_init(mymutex_t* mutex)
{
    mutex->locked = false;
    mutex->owner = NULL;
}

int mymutex_lock(mymutex_t* mutex)
{
    scheduler_disable_preemption();

    if(mutex->locked)
    {
        LOG("Mutex already locked. Waiting.");
        if(mutex->owner == scheduler_get_current_thread())
        {
            LOG("Mutex cannot be locked twice by the same thread. Thread id: %d", scheduler_get_current_thread()->id);
            return -1;
        }

        // Increase owner priority to prevent priority inversion.
        if(mutex->owner->priority < scheduler_get_current_thread()->priority)
        {
            mutex->owner->priority = scheduler_get_current_thread()->priority;
        }

        // Switch to next thread. Current thread context will be saved here.
        scheduler_switch_to_next_thread();

        // Scheduler switched context to this thread again, so we will try to lock mutex again.
        mymutex_lock(mutex);
    }

    mutex->locked = true;
    mutex->owner = scheduler_get_current_thread();
    mutex->owner_basic_priority = scheduler_get_current_thread()->priority;

    scheduler_enable_preemption();

    return 0;
}

int mymutex_unlock(mymutex_t* mutex)
{
    scheduler_disable_preemption();

    if(mutex->owner != scheduler_get_current_thread())
    {
        LOG("Mutex can only be unlocked by it's owner!\n"
                    "    Current thread id: %d"
                    "    Owner id: %d"
                    , scheduler_get_current_thread()->id
                    , mutex->owner->id);

        // Switch to next thread. Current thread context will be saved here.
        scheduler_switch_to_next_thread();

        // Scheduler switched context to this thread again, so we will try to unlock mutex again.
        mymutex_unlock(mutex);
    }

    if(!mutex->locked)
    {
        LOG("Cannot unlock not locked mutex.");
        return -1;
    }

    mutex->locked = false;
    mutex->owner->priority = mutex->owner_basic_priority;
    mutex->owner = NULL;

    scheduler_enable_preemption();

    return 0;
}
