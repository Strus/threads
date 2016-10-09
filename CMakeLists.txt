cmake_minimum_required(VERSION 3.6)
project(threads)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99 -Wall -Werror")
set(EXECUTABLE_OUTPUT_PATH ../bin)
set(APP_NAME threadsTestApp)

include_directories(include)
include_directories(src)

set(sources
        include/mythreads.h
        include/myscheduler.h
        include/mymutex.h
        src/main.c
        src/thread.c
        src/thread.h
        src/scheduler.c
        src/scheduler.h
        src/mutex.c
        src/utils/carousel.c
        src/utils/carousel.h
        src/debug/logging.h
        )

add_executable(${APP_NAME} ${sources})

