///////////////////////////////////////////////////////////////////////////////
///
///    File: mymutex.h
///    Date: 09 Oct 2016
///
///    Copyright (C) 2016 Adrian Kepka <kepka.adrian@gmail.com>
///
///////////////////////////////////////////////////////////////////////////////

#ifndef THREADS_MYMUTEX_H
#define THREADS_MYMUTEX_H

#include <stdbool.h>

struct thread;

typedef struct
{
    bool locked;
    struct thread* owner;
    int owner_basic_priority;
} mymutex_t;

/// @brief Initalizes given mutex.
/// @param mutex Pointer to mutex that needs to be initalized
/// @note Every mutex must be initalized first.
void mymutext_init(mymutex_t* mutex);

/// @brief Locks given mutex. Mutex can only be locked if it is not locked already.
///        If thread tries to lock already locked mutex, then:
///        - thread that tried lock will wait until mutex will be unlocked
///        - if thread priority is higher than owner priority, then owner priority will be increased to thread priority
///        - scheduler will perform preemption and switch to next thread
/// @param mutex Pointer to mutex that need to be locked.
/// @return 0 on success, -1 on error.
int mymutex_lock(mymutex_t* mutex);

/// @brief Unlocks given mutex. Mutex can be unlocked only by it's owner (thread that locked it).
///        If thread tries to unlock another thread's mutex, then:
///        - thread that tried unlock will wait until mutex will be unlocked by it's owner
///        - scheduler will perform preemption and switch to next thread
///        If unlock can be performed then priority of owner's thread will be set back to it's original priority.
/// @param mutex Pointer to mutex that need to be unlocked.
/// @return 0 on success, -1 on error.
int mymutex_unlock(mymutex_t* mutex);

#endif
