
#include "../../globalDefinitions.h"

#include "../treeGenerator/tree_structs.h"

#include "commands.h"
#include "datatypes.h"

#include "../treeGenerator/tree_structs.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "string.h"
#include "malloc.h"
#include "ctype.h"


typedef char BYTE;

FILE* glob_file;

//? ###########################################################################
//? ###########################################################################

int convert_ToBytes(char *dest, void *a, size_t size) {
    for (int q = 0; q < size; ++q) {
        dest[q] = ((char*)a)[q];
    }
    return 0;
}

//? ###########################################################################
//? ###########################################################################

typedef struct stringArray_t {

    size_t capacity;
    size_t occupancy;

    char  **array;
    size_t *lensa;

} StringArray;

StringArray* stringArray_init() {

    StringArray *arr = (StringArray*) malloc(sizeof(StringArray));

    arr->array     = (char** ) malloc(sizeof(char* )*10);
    arr->lensa     = (size_t*) malloc(sizeof(size_t)*10);
    arr->capacity  = 10;
    arr->occupancy = 0;

    return arr;
}

size_t stringArray_add(StringArray *arr, char *str, size_t len) {

    assert(arr != NULL);
    assert(str != NULL);

    if (arr->capacity == arr->occupancy) {
        arr->array = (char** ) realloc(arr->array, sizeof(char* )*(arr->capacity)*2);
        arr->lensa = (size_t*) realloc(arr->lensa, sizeof(size_t)*(arr->capacity)*2);
    }

    assert(arr->array != NULL);

    (arr->array)[(arr->occupancy)++] = str;
    (arr->lensa)[(arr->occupancy)-1] = len;
    // printf("-2-- %lu\n", arr->lensa[q]);

    return arr->occupancy - 1;
}

int stringArray_printToFile(StringArray *arr, char *filePath) {

    FILE *file = fopen(filePath, "w+");

    for (size_t q = 0; q < arr->occupancy; ++q) {

        int l = (int) (arr->lensa[q]);
        // printf("-2-- %lu\n", arr->lensa[q]);
        // printf("-1-- %d\n", l);
        // fprintf(file, "%*s", l, arr->array[q]);
        for (size_t w = 0; w < arr->lensa[q]; ++w) {
            putc(arr->array[q][w], file);
        }
        // fprintf(file, "%s", arr->array[q]);
    }

    fclose(file); file = NULL;

    return 0;
}

//? ###########################################################################
//? ###########################################################################

typedef struct prog_t {

    StringArray *code;
    StringArray *data;

} Prog;

Prog* prog_init() {

    Prog *prog = (Prog*) malloc(sizeof(Prog));

    prog->code = stringArray_init();
    prog->data = stringArray_init();

    return prog;
}

Prog* prog_destr(Prog *prog) {

    CAP
    DOT
    return NULL;
}

size_t prog_dataAdd(Prog *prog, char *str, size_t len) {

    return stringArray_add(prog->data, str, len);
}

size_t prog_codeAdd(Prog *prog, char *str, size_t len) {

    return stringArray_add(prog->code, str, len);
}

size_t prog_codeAddCommand(Prog *prog, char command) {

    char *s = (char*) malloc(sizeof(char)*2);
    s[0] = command;
    s[1] = '\0';

    return stringArray_add(prog->code, s, 1);
}

size_t prog_getDataLineCount(Prog *prog) {

    return prog->data->occupancy;
}

size_t prog_getCodeLineCount(Prog *prog) {

    return prog->code->occupancy;
}

int prog_printDataToFile(Prog *prog, char *filePath) {

    return stringArray_printToFile(prog->data, filePath);
}

int prog_printCodeToFile(Prog *prog, char *filePath) {

    return stringArray_printToFile(prog->code, filePath);
}

//? ###########################################################################
//? ###########################################################################

int prog_addTree(Prog *prog, Exptree *tree, Function *func);
int prog_addApply(Prog *prog, Exptree *tree, Function *func);
int prog_addFunction(Prog *prog, Exptree *tree, Function *func);
int prog_addElem(Prog *prog, Exptree *tree, Function *func);

int prog_addApply(Prog *prog, Exptree *tree, Function *func) {

    prog_addTree(prog, ((Pair*)(tree->content))->arg, func);
    prog_addTree(prog, ((Pair*)(tree->content))->func, func);

    return 0;
}

int prog_addFunction(Prog *prog, Exptree *tree, Function *func) {

    char* s = (char*) malloc(
        sizeof(char) * (strlen(((Function*)(tree->content))->name) + 2)
    );
    s[0] = COMMAND_NEED_FOR_ADDRESS;
    s[1] = COMMAND_CALL;
    strcpy(s+2, ((Function*)(tree->content))->name);
    prog_codeAdd(prog, s, strlen(((Function*)(tree->content))->name) + 2);

    fprintf(glob_file, "NFA call %s\n", ((Function*)(tree->content))->name); // log

    return 0;
}

int prog_addElem(Prog *prog, Exptree *tree, Function *func) {

    assert(prog != NULL);
    assert(tree != NULL);

    // prog_addString(prog, "call");
    if (
        tree->elemType->type == INT_TYPE &&
        tree->elemType->child == NULL
    ) {

        int val = atoi( (char*) (tree->content) );

        char *str = (char*) malloc(sizeof(char)*6);
        str[0] = DATATYPE_INT;
        str[1] = (char) (val >> 0 );
        str[2] = (char) (val >> 8 );
        str[3] = (char) (val >> 16);
        str[4] = (char) (val >> 24);
        str[5] = '\0';

        printf("----> |");
        for (int q = 0; q < 5; ++q) {
            printf("%hu ", str[q]);
        }
        printf("|\n");
        
        prog_dataAdd(prog, str, 5);
    } else {

        CAP
    }

    char *str = (char*) malloc(sizeof(char)*(sizeof(size_t)+1));
    str[0] = COMMAND_PUSH;
    size_t line = prog_getDataLineCount(prog);
    convert_ToBytes(str+1, &line, sizeof(size_t));
    prog_codeAdd(prog, str, sizeof(size_t) + 1);
    // MES("Where elem must be added to data"); // dev
    fprintf(glob_file, "push %lu\n", line); // log

    return 0;
}

int prog_addVar(Prog *prog, Exptree *tree, Function *func) {

    assert(prog != NULL);
    assert(tree != NULL);

    int num_ = function_getArgNum(func, (char*)(tree->content));
    assert(num_ >= 0); // dev
    size_t num = (size_t) num;

    char *str = (char*) malloc(sizeof(char)*2 + sizeof(size_t));
    str[0] = COMMAND_PUSHARG;
    convert_ToBytes(str+1, &num, sizeof(size_t));
    prog_codeAdd(prog, str, sizeof(size_t) + 1);
    // MES("Where elem must be added to data"); // dev
    fprintf(glob_file, "pushArg %lu\n", num); // log

    return 0;
}

int prog_addTree(Prog *prog, Exptree *tree, Function *func) {

    assert(prog != NULL);
    assert(tree != NULL);

    if (tree->type == EXPTREE_TYPE_ELEM) {

        return prog_addElem(prog, tree, func);
    } else if (tree->type == EXPTREE_TYPE_VAR) {

        return prog_addVar(prog, tree, func);
    } else if (tree->type == EXPTREE_TYPE_APPLY) {

        return prog_addApply(prog, tree, func);
    } else if (
        tree->type == EXPTREE_TYPE_FUNCTION ||
        tree->type == EXPTREE_TYPE_OPERATOR
    ) {

        return prog_addFunction(prog, tree, func);
    } else if (tree->type == EXPTREE_TYPE_IF) {

        CAP // dev
    }

    return 1;
}

//? ###########################################################################
//? ###########################################################################

int functionAddressesSubstitution() {
    DOT

    return 0;
}

//? ###########################################################################
//? ###########################################################################

int interpreter(Root *root) {

    glob_file = fopen("log.txt", "w+");

    MES("-------------------------------------------------------------------------");

    Prog *prog = prog_init();

    Deflist *iter = deflist_iteratorInit(root->deflist);
    while (iter) {

        if (iter->func->tree == NULL) {

            iter = deflist_iteratorNext(iter);
            continue;
        }

        iter->line = prog_getDataLineCount(prog) + 1;

        printf("name -> %s\n", iter->func->name);
        prog_addTree(prog, iter->func->tree, iter->func);
        
        prog_codeAddCommand(prog, COMMAND_RET);

        iter = deflist_iteratorNext(iter);
    }

    functionAddressesSubstitution();

    prog_printDataToFile(prog, "data.txt");
    prog_printCodeToFile(prog, "code.txt");


    MES("-------------------------------------------------------------------------");
    fclose(glob_file);
    prog = prog_destr(prog);

    

    return 0;
}
