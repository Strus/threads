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

#include "utils/carousel.h"
#include "thread.h"
#include <malloc.h>

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
    for(int i = 0; i <= LOOP_ITERATIONS - 2; i++)
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
        if(i == 2)
        {
            LOG("Third thread end prematurely");
            mythread_exit();
        }
    }
    LOG("Third thread end");
}

void fourthThread(void* data)
{
    LOG("Fourth thread start");
    for(int i = 0; i <= LOOP_ITERATIONS - 2; i++)
    {
        LOG("T4 - %d", i);
        sleep(1);
        if(i == 2)
        {
            mythread_kill(1);
        }
    }
    LOG("Fourth thread end");
}

int main()
{
    myscheduler_init();

    mythreads_start(firstThread, NULL);
    mythreads_start(secondThread, NULL);
    mythreads_start(thirdThread, NULL);
    mythreads_start(fourthThread, NULL);

    myscheduler_start();
//    threadcarousel_t carousel;
//    carousel_init(&carousel);
//
//    threadcarousel_node_t* new1 = (threadcarousel_node_t*) malloc(sizeof(threadcarousel_node_t));
//    new1->thread = (mythread_t*) malloc(sizeof(mythread_t));
//    new1->thread->id = 1;
//    carousel_insert(&carousel, new1);
//
//    threadcarousel_node_t* new2 = (threadcarousel_node_t*) malloc(sizeof(threadcarousel_node_t));
//    new2->thread = (mythread_t*) malloc(sizeof(mythread_t));
//    new2->thread->id = 2;
//    carousel_insert(&carousel, new2);
//
//    threadcarousel_node_t* new3 = (threadcarousel_node_t*) malloc(sizeof(threadcarousel_node_t));
//    new3->thread = (mythread_t*) malloc(sizeof(mythread_t));
//    new3->thread->id = 3;
//    carousel_insert(&carousel, new3);
//
//    carousel_remove(&carousel, new3);
    return 0;
}