

#include "../treeGenerator/tree_structs.h"

typedef struct stack_t {

    struct stack_t *next;
    struct stack_t *prev;

    Exptree *tree;

} Stack;

Stack* stack_push(Stack *stack, Exptree *tree);
Stack* stack_pop(Stack *stack);








