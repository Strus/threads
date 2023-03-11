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

/// @brief Prints formatted data to stdout using printf and flushes stdout buffer after that.
/// @note Every log message is ended with EOL character.
#define INFO(f_, ...)                                                                                                  \
    printf((f_ "\n"), ##__VA_ARGS__);                                                                                  \
    fflush(stdout)

/// @brief Prints formated data with filename and line number at the beginning to stdout using printf and flushes stdout
/// buffer after that.
/// @note Every log message is ended with EOL character.
#define ERROR(f_, ...)                                                                                                 \
    printf(("%s:%d " f_ "\n"), __FILE__, __LINE__, ##__VA_ARGS__);                                                     \
    fflush(stdout)

#endif
