

#include "../scanner/tokenTypesEnum.h"
#include "../scanner/scanner_structs.h"

#include "tree_structs.h"

#ifndef EXPRESSION_ANALIZER_H
#define EXPRESSION_ANALIZER_H


typedef struct operatorStack_t {



} OperatorStack;

typedef struct outputSequence_t {

    

} OutputSequence;

// typedef struct exptree_t {

//     int type;

//     char *content;
//     TypeTree *elemType;

// } Exptree;


//? ###########################################################################
//? ###########################################################################

typedef struct opstack_t {

    struct opstack_t *next;
    struct opstack_t *prev;

    Exptree *tree;

} Opstack;

Opstack* opstack_init();
Opstack* opstack_push(Opstack *stack, Exptree *tree);
Exptree* opstack_pop(Opstack *stack);
int opstack_getPriority(Opstack *stack);
int opstack_isEmpty(Opstack *stack);

int opstack_dump(Opstack *stack, FILE *file);

// typedef struct valstack_t {

//     struct valstack_t *next;
//     struct valstack_t *prev;

//     Exptree *tree;

// } Valstack;

// Valstack* valstack_init();
// Valstack* valstack_push(Valstack *stack, Exptree *tree);
// Exptree*  valstack_pop(Valstack *stack);

//? ###########################################################################
//? ###########################################################################

int expressionPermutator(Token *token, Root *root);
Exptree* expression(Token **token_ptr, Scope *scope);

//? ###########################################################################
//? ###########################################################################

#endif // EXPRESSION_ANALIZER_H
