///////////////////////////////////////////////////////////////////////////////
///
///    File: carousel.h
///    Date: 04 Oct 2016
///
///    Copyright (C) 2016 Adrian Kepka <kepka.adrian@gmail.com>
///
///////////////////////////////////////////////////////////////////////////////

#ifndef THREADS_CAROUSEL_H
#define THREADS_CAROUSEL_H

#include "thread.h"

/// @brief Represents node in carousel (circular linked list)
typedef struct carousel_node
{
    mythread_t* thread;
    struct carousel_node* next;
} threadcarousel_node_t;

/// @brief Represents carousel (circular linked list)
typedef struct carousel
{
    threadcarousel_node_t* tail;
    threadcarousel_node_t* current;
} threadcarousel_t;

/// @brief Initalizes carousel. Every carousel must be initalized first.
/// @param carousel Carousel to initalize.
void carousel_init(threadcarousel_t* carousel);

/// @brief Inserts node at the end of the carousel.
/// @param carousel Carousel to insert element.
/// @param node Node to be inserted
void carousel_insert(threadcarousel_t* carousel, threadcarousel_node_t* node);

/// @brief Removes node from carousel.
/// @param carousel Carousel to remove element.
/// @param node Node to be removed.
void carousel_remove(threadcarousel_t* carousel, threadcarousel_node_t* node);

threadcarousel_node_t* carousel_find_by_id(threadcarousel_t* carousel, int tid);

void carousel_switch_to_next(threadcarousel_t* carousel);

/// @brief Debug function that prints carousel into console.
/// @param carousel Carosel to print.
void carousel_dump(threadcarousel_t* carousel);

#endif
