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

#include <stdio.h>
#include <unistd.h>

#define LOOP_ITERATIONS 10

void firstThread(void* data)
{
    printf("First thread start\n");
    for(int i = 0; i < LOOP_ITERATIONS; i++)
    {
        printf("T1 - %d\n", i);
        fflush(stdout);
        sleep(1);
    }
    printf("First thread end\n");
}

void secondThread(void* data)
{
    printf("Second thread start\n");
    for(int i = 0; i < LOOP_ITERATIONS; i++)
    {
        printf("T2 - %d\n", i);
        fflush(stdout);
        sleep(1);
    }
    printf("Second thread end\n");
}

void thirdThread(void* data)
{
    printf("Third thread start\n");
    for(int i = 0; i < LOOP_ITERATIONS; i++)
    {
        printf("T3 - %d\n", i);
        fflush(stdout);
        sleep(1);
    }
    printf("Third thread end\n");
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