#include <stddef.h>
#include <malloc.h>

#ifndef SIMPLELIST_H
#define SIMPLELIST_H

//? ###########################################################################
//? ###########################################################################

typedef struct simpleList_t {

    struct simpleList_t* prev;
    struct simpleList_t* next;

    size_t pointer;
    
} SimpleList;

//? ###########################################################################
//? ###########################################################################

SimpleList* simpleList_init(size_t pointer);
SimpleList* simpleList_append(SimpleList* list, size_t pointer);

//? ###########################################################################
//? ###########################################################################

#endif
