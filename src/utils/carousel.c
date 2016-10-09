///////////////////////////////////////////////////////////////////////////////
///
///    File: carousel.c
///    Date: 04 Oct 2016
///
///    Copyright (C) 2016 Adrian Kepka <kepka.adrian@gmail.com>
///
///////////////////////////////////////////////////////////////////////////////

#include "carousel.h"
#include "debug/logging.h"

#include <malloc.h>

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
    // we have only one node and we are removing it
    if(node->next == node)
    {
        free(node);
        carousel->tail = NULL;
        carousel->current = NULL;
        return;
    }

    threadcarousel_node_t* temp = node->next;
    // node to delete is becoming next node, which will be freed at the end.
    node->thread = temp->thread;
    node->next = temp->next;

    // temp will be deleted at the end, so we need to switch tail and current pointers
    // if they are pointing on temp right now.
    if(carousel->tail == temp)
    {
        carousel->tail = node;
    }
    if(carousel->current == temp)
    {
        carousel->current = node;
    }

    free(temp);
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
        if (iter->thread->id == tid)
        {
            return iter;
        }
    }

    return NULL;
}

void carousel_switch_to_next(threadcarousel_t* carousel)
{
    carousel->current = carousel->current->next;
}
