///////////////////////////////////////////////////////////////////////////////
///
///    File: main.c
///    Date: 4 Sep 2016
///
///    Copyright (C) 2016 Adrian Kepka <kepka.adrian@gmail.com>
///
///////////////////////////////////////////////////////////////////////////////

#include "mythreads.h"
#include "myscheduler.h"
#include "debug/logging.h"

#include <stdio.h>
#include <unistd.h>

#define LOOP_ITERATIONS 5

void firstThread(void* data)
{
    LOG("First thread start");
    for(int i = 0; i <= LOOP_ITERATIONS; i++)
    {
        LOG("T1 - %d", i);
        sleep(1);
    }
    LOG("First thread end");
}

void secondThread(void* data)
{
    LOG("Second thread start");
    for(int i = 0; i <= LOOP_ITERATIONS; i++)
    {
        LOG("T2 - %d", i);
        sleep(1);
    }
    LOG("Second thread end");
}

void thirdThread(void* data)
{
    LOG("Third thread start");
    for(int i = 0; i <= LOOP_ITERATIONS; i++)
    {
        LOG("T3 - %d", i);
        sleep(1);
    }
    LOG("Third thread end");
}

int main()
{
    myscheduler_init();

    mythreads_start(firstThread, NULL);
    mythreads_start(secondThread, NULL);
    mythreads_start(thirdThread, NULL);

    myscheduler_start();
//    threadcarousel_t carousel;
//    carousel_init(&carousel);
//
//    threadcarousel_node_t* new1 = (threadcarousel_node_t*) malloc(sizeof(threadcarousel_node_t));
//    new1->thread = 1;
//    carousel_insert(&carousel, new1);
//
//    threadcarousel_node_t* new2 = (threadcarousel_node_t*) malloc(sizeof(threadcarousel_node_t));
//    new2->thread = 2;
//    carousel_insert(&carousel, new2);
//
//    threadcarousel_node_t* new3 = (threadcarousel_node_t*) malloc(sizeof(threadcarousel_node_t));
//    new3->thread = 3;
//    carousel_insert(&carousel, new3);
//
//    carousel_remove(&carousel, new3);
    return 0;
}