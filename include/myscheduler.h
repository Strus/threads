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
void myscheduler_init();

/// @brief Starts simple scheduler.
void myscheduler_start();

#endif
