#include "simpleList.h"

//? ###########################################################################
//? ###########################################################################

SimpleList* simpleList_init(size_t pointer) {

    SimpleList* list = (SimpleList*) malloc(sizeof(SimpleList));

    list->prev = NULL;
    list->next = NULL;

    list->pointer = pointer;

    return list;
}

SimpleList* simpleList_append(SimpleList* list, size_t pointer) {

    while (list->next) list = list->next;

    SimpleList* newList = simpleList_init(pointer);

    list->next = newList;
    newList->prev = list;

    return newList;
}

//? ###########################################################################
//? ###########################################################################
