///////////////////////////////////////////////////////////////////////////////
///
///    File: logging.h
///    Date: 07 Oct 2016
///
///    Copyright (C) 2016 Adrian Kepka <kepka.adrian@gmail.com>
///
///////////////////////////////////////////////////////////////////////////////

#ifndef THREADS_LOGGING_H
#define THREADS_LOGGING_H

#include <stdio.h>

#define LOG(f_, ...) printf((f_), ##__VA_ARGS__); printf("\n"); fflush(stdout)

#endif
