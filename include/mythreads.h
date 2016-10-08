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
/// @return Thread id when successful, 0 on error.
int mythreads_start(mythread_func func, void* args);

/// @brief End current thread
/// @return Nothing when successful
///         -1 on error.
int mythread_exit(void);

/// @brief Ends thread of given id.
/// @param tid Id of the thread to end.
/// @return Nothing when successful
///         -1 on error.
int mythread_kill(int tid);

#endif