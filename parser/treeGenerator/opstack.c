
#include <assert.h>
#include <malloc.h>
#include <string.h>

#include "expressionAnalizer.h"

//? ###########################################################################
//? ###########################################################################

Opstack* opstack_init() {

    Opstack *stack = (Opstack*) malloc(sizeof(Opstack));

    memset(stack, 0, sizeof(Opstack));

    return stack;
}

Opstack* opstack_push(Opstack *stack, Exptree *tree) {

    assert(stack != NULL);
    assert(tree  != NULL);

    while (stack->next) stack = stack->next;

    if (tree->type == EXPTREE_TYPE_ELEM ||
        tree->type == EXPTREE_TYPE_VAR
    ) {
        printf("pushing to [%p]: %s\n", stack, (char*)(tree->content));
    } else {
        printf("pushing to [%p]: %s\n", stack, ((Function*)(tree->content))->name);
    }

    stack->next = opstack_init();
    stack->next->prev = stack;
    stack->next->tree = tree;

    return stack->next;
}

Exptree* opstack_pop(Opstack *stack) {

    while (stack->next) stack = stack->next;

    Exptree *ret = stack->tree;

    if (ret->type == EXPTREE_TYPE_ELEM ||
        ret->type == EXPTREE_TYPE_VAR
    ) {
        printf("poping from [%p]: %s\n", stack, (char*)(ret->content));
    } else {
        printf("poping from [%p]: %s\n", stack, ((Function*)(ret->content))->name);
    }

    if (stack->prev) stack->prev->next = NULL;
    free(stack);

    return ret;
}

int opstack_getPriority(Opstack *stack) {

    assert(stack != NULL);

    while (stack->next) stack = stack->next;
    if (!stack->tree) return -1;

    assert(stack != NULL);
    assert(stack->tree != NULL);
    // printf("stack--> %p\n", stack->tree);
// DOT
//     if (stack->tree->type == EXPTREE_TYPE_OPERATOR ||
//         stack->tree->type == EXPTREE_TYPE_FUNCTION
//     ) {

//     } else if (stack->tree->type == EXPTREE_TYPE_APPLY) {
    DOT
    return exptree_getPriority(stack->tree);

    // printf("--|||> %d\n", ((Function*)(stack->tree->content))->priority);
    // return ((Function*)(stack->tree->content))->priority;

}

int opstack_isEmpty(Opstack *stack) {

    assert(stack != NULL); // dev

    while (stack->next) stack = stack->next;

    return (stack->tree == NULL);
}

int opstack_dump(Opstack *stack, FILE *file) {

    assert(stack != NULL);

    while (stack->prev) stack = stack->prev;
    
    while (stack) {

        if (stack->tree != NULL) {
            exptree_dump(stack->tree, file);
            fprintf(file, "\n\n");
        }

        stack = stack->next;
    }
    DOT
    return 0;
}

//? ###########################################################################
//? ###########################################################################
