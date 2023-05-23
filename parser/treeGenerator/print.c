#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "tree_structs.h"
#include "expressionAnalizer.h"



int exptree_dump(Exptree *tree, FILE *file) {

    // fprintf(file, "─┬─🢒 type: %d\n", tree->type);
    // if (tree->type == EXPTREE_TYPE_ELEM) {
    //     fprintf(file, "─┬─🢒 type: %d\n", tree->content);

    // } else if (tree->type == EXPTREE_TYPE_APPLY) {

    // } else if ()

    assert(tree != NULL);

    switch (tree->type) {
        case EXPTREE_TYPE_APPLY:
            fprintf(file, "─┬─🢒 type: APPLY\n");
            fprintf(file, " ╰─🢒 content: --\n");
            break;
        case EXPTREE_TYPE_ELEM:
            fprintf(file, "─┬─🢒 type: ELEM\n");
            fprintf(file, " ╰─🢒 content: %s\n", (char*)(tree->content));
            break;
        case EXPTREE_TYPE_VAR:
            fprintf(file, "─┬─🢒 type: VAR\n");
            fprintf(file, " ╰─🢒 content: %s\n", (char*)(tree->content));
            break;
        case EXPTREE_TYPE_OPERATOR:
            fprintf(file, "─┬─🢒 type: OPERATOR\n");
            fprintf(file, " ╰─🢒 content: %s\n", ((Function*)(tree->content))->name);
            break;
        case EXPTREE_TYPE_FUNCTION:
            fprintf(file, "─┬─🢒 type: FUNCTION\n");
            fprintf(file, " ╰─🢒 content: %s\n", ((Function*)(tree->content))->name);
            break;
        case EXPTREE_TYPE_IF:
            fprintf(file, "─┬─🢒 type: IF\n");
            fprintf(file, " ╰─🢒 content: --\n");
            break;
        default:
            printf("--> %d\n", tree->type);
            CAP
    }

    return 0;
}

// int opstack_printToFile(Opstack *stack, FILE *file) {

//     while (stack) {

//         fprintf(file, "─┬─🢒 type: %p\n", stack);


//         stack = stack->next;
//     }

//     return 0;
// }

int exptree_printToFile_body(Exptree *tree, FILE *file);
int exptree_printToFile_apply(Exptree *tree, FILE *file) {

    assert(tree != NULL);
    // assert(tree != NULL);
    char *typestr = typeTree_convertToString(tree->elemType);
    fprintf(file, "\t%lu [shape=rectangle, style=\"filled\",fillcolor=\"#506266\", label=\"APPLY\n%s\"]\n", (size_t) tree, typestr);
    free(typestr);

    Pair *pair = (Pair*)(tree->content);
    assert(pair != 0);
    assert(pair->func != 0);
    assert(pair->arg != 0);

    exptree_printToFile_body(pair->func, file);
    fprintf(file, "%lu -> %lu\n", (size_t) tree, (size_t)(pair->func));
    exptree_printToFile_body(pair->arg, file);
    fprintf(file, "%lu -> %lu\n", (size_t) tree, (size_t)(pair->arg));

    return 0;
}

int exptree_printToFile_if(Exptree *tree, FILE *file) {

    assert(tree != NULL);

    char *typestr = typeTree_convertToString(tree->elemType);
    fprintf(file, "\t%lu [shape=rectangle, style=\"filled\",fillcolor=\"#A3AB78\", label=\"IF_THEN_ELSE\n%s\"]\n", (size_t) tree, typestr);
    free(typestr);

    Ifexptree *iftree = (Ifexptree*)(tree->content);
    assert(iftree != NULL);
    assert(iftree->cond != 0);
    assert(iftree->arg1 != 0);
    assert(iftree->arg2 != 0);

    exptree_printToFile_body(iftree->cond, file);
    fprintf(file, "%lu -> %lu\n", (size_t) tree, (size_t)(iftree->cond));
    fprintf(file, "%lu -> %lu\n", (size_t) tree, (size_t)(iftree->arg1));
    fprintf(file, "%lu -> %lu\n", (size_t) tree, (size_t)(iftree->arg2));
    exptree_printToFile_body(iftree->arg1, file);
    exptree_printToFile_body(iftree->arg2, file);

    return 0;
}

int exptree_printToFile_body(Exptree *tree, FILE *file) {

    if (tree == NULL) return 0;

    char *typestr = typeTree_convertToString(tree->elemType);
    switch (tree->type) {
        case EXPTREE_TYPE_APPLY:
            exptree_printToFile_apply(tree, file);
            break;
        case EXPTREE_TYPE_IF:
            exptree_printToFile_if(tree, file);
            break;
        case EXPTREE_TYPE_ELEM:
            fprintf(file, "\t%lu [shape=rectangle, style=\"filled\",fillcolor=\"#BDE038\", label=\"ELEM\n%s\n%s\"]\n", (size_t) tree, (char*)(tree->content), typestr);
            break;
        case EXPTREE_TYPE_VAR:
            fprintf(file, "\t%lu [shape=rectangle, style=\"filled\",fillcolor=\"#C26D97\", label=\"VAR\n%s\n%s\"]\n", (size_t) tree, (char*)(tree->content), typestr);
            break;
        case EXPTREE_TYPE_OPERATOR:
            fprintf(file, "\t%lu [shape=rectangle, style=\"filled\",fillcolor=\"#818274\", label=\"OPERATOR\n%s\n%p\n%s\"]\n", (size_t) tree, ((Function*)(tree->content))->name, (void*)tree, typestr);
            break;
        case EXPTREE_TYPE_FUNCTION:
            fprintf(file, "\t%lu [shape=rectangle, style=\"filled\",fillcolor=\"#A3AB78\", label=\"FUNCTION\n%s\n%p\n%s\"]\n", (size_t) tree, ((Function*)(tree->content))->name, (void*)tree, typestr);
            break;
    }
    free(typestr);
    // char *typestr = typeTree_convertToString(tree->)
    // CAP // dev

    return 0;
}

int exptree_printToFile(Exptree *tree, char* filePath) {

    assert(tree != NULL);

    FILE *file = fopen(filePath, "w+");

    fprintf(file, "digraph {\n\n");
    exptree_printToFile_body(tree, file);
    fprintf(file, "\n}\n");

    fclose(file);

    return 0;
}
