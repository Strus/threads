///////////////////////////////////////////////////////////////////////////////
///
///    File: myscheduler.h
///    Date: 04 Oct 2016
///
///    Copyright (C) 2016 Adrian Kepka <kepka.adrian@gmail.com>
///
///////////////////////////////////////////////////////////////////////////////

#ifndef THREADS_MYSCHEDULER_H
#define THREADS_MYSCHEDULER_H

/// @brief Initializes simple scheduler based on round-robin policy.
/// @note This function should be called before starting any mythread and before starting scheduler.
void myscheduler_init();

/// @brief Starts simple scheduler based on round-robin policy.
void myscheduler_start();

#endif
