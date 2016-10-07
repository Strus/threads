///////////////////////////////////////////////////////////////////////////////
///
///    File: carousel.c
///    Date: 04 Oct 2016
///
///    Copyright (C) 2016 Adrian Kepka <kepka.adrian@gmail.com>
///
///////////////////////////////////////////////////////////////////////////////

#include "carousel.h"

#include <malloc.h>
#include <stdio.h>
#include <stdbool.h>
#include <thread.h>

void carousel_init(threadcarousel_t* carousel)
{
    carousel->tail = NULL;
    carousel->current = NULL;
}

void carousel_insert(threadcarousel_t* carousel, threadcarousel_node_t* node)
{
    // check if carousel is empty
    if(carousel->tail == NULL)
    {
        carousel->tail = node;
        carousel->tail->next = carousel->tail;
        carousel->current = carousel->tail;
        return;
    }

    node->next = carousel->tail->next;
    carousel->tail->next = node;
    carousel->tail = node;
}

void carousel_remove(threadcarousel_t* carousel, threadcarousel_node_t* node)
{
    threadcarousel_node_t* temp = node->next;
    bool node_is_tail = (node == carousel->tail);
    bool node_is_current = (node == carousel->current);
    node->thread = node->next->thread;
    node->next = temp->next;
    free(temp);

    if(node_is_tail)
    {
        carousel->tail = node;
    }

    if(node_is_current)
    {
        carousel->current = node;
    }
}

threadcarousel_node_t* carousel_find_by_id(threadcarousel_t* carousel, int tid)
{
    threadcarousel_node_t* temp = carousel->tail;
    // in case we have only one item in carousel (then the below loop would end instantly)
    if(temp->thread->id == tid)
    {
        return temp;
    }

    for(threadcarousel_node_t* iter = carousel->tail->next; iter != carousel->tail; iter = iter->next)
    {
        if (temp->thread->id == tid)
        {
            return temp;
        }
    }

    return NULL;
}

void carousel_switch_to_next(threadcarousel_t* carousel)
{
    carousel->current = carousel->current->next;
}

void carousel_dump(threadcarousel_t* carousel)
{
    /// @todo iterator
    threadcarousel_node_t* temp = carousel->tail;
    do
    {
        printf("%d\n", temp->thread->id);
        temp = temp->next;
    }while(temp != carousel->tail);
    printf("\n");
}
