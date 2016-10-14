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
/// @return 0 on success, -1 on error
int myscheduler_init();

/// @brief Starts simple scheduler based on round-robin policy.
/// @return 0 on succces, -1 on error
int myscheduler_start();

#endif
