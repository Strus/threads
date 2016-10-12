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
#include <logging.h>

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
        INFO("Mutex already locked. Owner id: %d. Waiting.", mutex->owner->id);
        if(mutex->owner == scheduler_get_current_thread())
        {
            INFO("Mutex cannot be locked twice by the same thread. Thread id: %d", scheduler_get_current_thread()->id);
            return -1;
        }

        // Increase owner priority to prevent priority inversion.
        if(mutex->owner->priority < scheduler_get_current_thread()->priority)
        {
            mutex->owner->priority = scheduler_get_current_thread()->priority;
        }

        scheduler_make_current_thread_pending();

        // Switch to next thread. Current thread context will be saved here.
        scheduler_switch_to_next_thread();
    }

    mutex->locked = true;
    mutex->owner = scheduler_get_current_thread();
    mutex->owner_basic_priority = scheduler_get_current_thread()->priority;
    INFO("Mutex locked. Owner id: %d", mutex->owner->id);

    scheduler_enable_preemption();

    return 0;
}

int mymutex_unlock(mymutex_t* mutex)
{
    scheduler_disable_preemption();

    if(mutex->owner != scheduler_get_current_thread())
    {
        INFO("Mutex can only be unlocked by it's owner!\n"
                    "    Current thread id: %d"
                    "    Owner id: %d"
                    , scheduler_get_current_thread()->id
                    , mutex->owner->id);

        // Make current thread pending, so he will be not take into consideration during next preemption
        // until mutex will be unlocked
        scheduler_make_current_thread_pending();

        // Switch to next thread. Current thread context will be saved here.
        scheduler_switch_to_next_thread();
    }

    if(!mutex->locked)
    {
        INFO("Cannot unlock not locked mutex.");
        return -1;
    }

    unsigned int owner_id = mutex->owner->id;
    mutex->locked = false;
    mutex->owner->priority = mutex->owner_basic_priority;
    mutex->owner = NULL;
    INFO("Mutex unlocked. Old owner id: %d", owner_id);

    // Mutex is unlocked, so we remove one thread from pending list and move it to scheduler carousel.
    scheduler_remove_one_thread_from_pending_list();

    scheduler_enable_preemption();

    return 0;
}
