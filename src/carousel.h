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

/// @brief Initalizes carousel.
/// @note Every carousel must be initalized first with this function.
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

/// @brief Moves node between carousels
/// @param from Carousel which have node to move.
/// @param to Destination carousel.
/// @param node Node to move between carousels.
void carousel_move_to_another_carousel(threadcarousel_t* from, threadcarousel_t* to, threadcarousel_node_t* node);

/// @brief Search for thread with given id in carousel.
/// @param carousel Carousel to find in.
/// @param tid Id of the thread to find.
/// @return Pointer to found thread
///         NULL if thread of given id is not found.
threadcarousel_node_t* carousel_find_by_id(threadcarousel_t* carousel, int tid);

/// @brief Switches current node to next node in carousel.
/// @param carousel Carousel to switch.
void carousel_switch_to_next(threadcarousel_t* carousel);

#endif
