cmake_minimum_required(VERSION 3.6)
project(threads)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99 -Wall -Werror -Wno-deprecated-declarations")
set(EXECUTABLE_OUTPUT_PATH bin)
set(APP_NAME threadsTestApp)

include_directories(include)
include_directories(src)

set(sources
        include/mythreads.h
        include/myscheduler.h
        include/mymutex.h
        src/thread.c
        src/thread.h
        src/scheduler.c
        src/scheduler.h
        src/mutex.c
        src/carousel.c
        src/carousel.h
        src/logging.h
        main.c
        )

add_executable(${APP_NAME} ${sources})

