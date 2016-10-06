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
    carousel->head = NULL;
    carousel->tail = NULL;
    carousel->current = NULL;
}

void carousel_insert(threadcarousel_t* carousel, threadcarousel_node_t* node)
{
    // check if carousel is empty
    if(carousel->head == NULL)
    {
        carousel->head = node;
        carousel->head->next = carousel->head;
        carousel->tail = carousel->head;
        carousel->tail->next = carousel->head;
        carousel->current = carousel->head;
        return;
    }

    node->next = carousel->head;
    carousel->tail->next = node;
    carousel->tail = node;
}

void carousel_remove(threadcarousel_t* carousel, threadcarousel_node_t* node)
{
    threadcarousel_node_t* temp = node->next;
    bool tailRemoval = (node == carousel->tail);
    bool twoElementsCarousel = (temp->next == node);
    node->thread = node->next->thread;
    node->next = temp->next;
    free(temp);

    if(twoElementsCarousel && tailRemoval)
    {
        carousel->head = carousel->tail;
    }
    else
    {
        carousel->tail = carousel->head;
    }
}

void carousel_switch_to_next(threadcarousel_t* carousel)
{
    carousel->current = carousel->current->next;
}

void carousel_dump(threadcarousel_t* carousel)
{
    threadcarousel_node_t* temp = carousel->head;
    do
    {
        printf("%d\n", temp->thread->id);
        temp = temp->next;
    }while(temp != carousel->head);
    printf("\n");
}
