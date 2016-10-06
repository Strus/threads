///////////////////////////////////////////////////////////////////////////////
///
///    File: mythreads.h
///    Date: 04 Oct 2016
///
///    Copyright (C) 2016 Adrian Kepka <kepka.adrian@gmail.com>
///
///////////////////////////////////////////////////////////////////////////////

#ifndef THREADS_MYTHREADS_H
#define THREADS_MYTHREADS_H

typedef void (*mythread_func)(void*);

/// @brief Registers new thread in scheduler.
/// @param func Pointer to thread function.
/// @param args Arguments of thread function.
/// @return Thread id in case of success, -1 otherwise.
int mythreads_start(mythread_func func, void* args);

/// @brief End current thread
/// @return 0 in case of success, other number in case of error.
int mythread_exit(void);

/// @brief Ends thread of given id.
/// @param tid Id of the thread to end.
/// @return 0 in case of success, other number in case of error.
int mythread_kill(int tid);

#endif