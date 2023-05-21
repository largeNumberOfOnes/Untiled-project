
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

    char **array;

} StringArray;

StringArray* stringArray_init() {

    StringArray *arr = (StringArray*) malloc(sizeof(StringArray));

    arr->array     = (char**) malloc(sizeof(char*)*10);
    arr->capacity  = 10;
    arr->occupancy = 0;

    return arr;
}

size_t stringArray_add(StringArray *arr, char *str) {

    assert(arr != NULL);
    assert(str != NULL);

    if (arr->capacity == arr->occupancy) {
        arr->array = (char**) realloc(arr->array, sizeof(char*)*(arr->capacity)*2);
    }

    assert(arr->array != NULL);

    (arr->array)[(arr->occupancy)++] = str;

    return arr->occupancy - 1;
}

int stringArray_printToFile(StringArray *arr, char *filePath) {

    FILE *file = fopen(filePath, "w+");

    for (size_t q = 0; q < arr->occupancy; ++q) {

        fprintf(file, "%s", arr->array[q]);
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

size_t prog_dataAdd(Prog *prog, char *str) {

    return stringArray_add(prog->data, str);
}

size_t prog_codeAdd(Prog *prog, char *str) {

    return stringArray_add(prog->code, str);
}

size_t prog_codeAddCommand(Prog *prog, char command) {

    char *s = (char*) malloc(sizeof(char)*2);
    s[0] = command;
    s[1] = '\0';

    return stringArray_add(prog->code, s);
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

int prog_addTree(Prog *prog, Exptree *tree);
int prog_addApply(Prog *prog, Exptree *tree);
int prog_addFunction(Prog *prog, Exptree *tree);
int prog_addElem(Prog *prog, Exptree *tree);

int prog_addApply(Prog *prog, Exptree *tree) {

    prog_addTree(prog, ((Pair*)(tree->content))->arg);
    prog_addTree(prog, ((Pair*)(tree->content))->func);

    return 0;
}

int prog_addFunction(Prog *prog, Exptree *tree) {

    char* s = (char*) malloc(
        sizeof(char) * (strlen(((Function*)(tree->content))->name) + 2)
    );
    s[0] = COMMAND_NEED_FOR_ADDRESS;
    s[1] = COMMAND_CALL;
    strcpy(s+2, ((Function*)(tree->content))->name);
    prog_codeAdd(prog, s);

    fprintf(glob_file, "NFA call %s\n", ((Function*)(tree->content))->name); // log

    return 0;
}

int prog_addElem(Prog *prog, Exptree *tree) {

    assert(prog != NULL);
    assert(tree != NULL);

    // prog_addString(prog, "call");
    if (FALSE) {

    } else if (
        tree->elemType->type == INT_TYPE &&
        tree->elemType->child == NULL
    ) {

        printf("777-> %s", (char*) (tree->content));
        int val = atoi( (char*) (tree->content) );
        printf("888-> %d", val);
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
        
        prog_dataAdd(prog, str);
    } else {
        
        CAP
    }

    char *str = (char*) malloc(sizeof(char)*3);
    str[0] = COMMAND_PUSH;
    size_t line = prog_getDataLineCount(prog);
    convert_ToBytes(str+1, &line, sizeof(size_t));
    prog_codeAdd(prog, str);
    // MES("Where elem must be added to data"); // dev
    fprintf(glob_file, "push %lu\n", line); // log

    return 0;
}

int prog_addTree(Prog *prog, Exptree *tree) {

    assert(prog != NULL);
    assert(tree != NULL);

    if (tree->type == EXPTREE_TYPE_ELEM) {

        return prog_addElem(prog, tree);
    } else if (tree->type == EXPTREE_TYPE_APPLY) {

        return prog_addApply(prog, tree);
    } else if (
        tree->type == EXPTREE_TYPE_FUNCTION ||
        tree->type == EXPTREE_TYPE_OPERATOR
    ) {

        return prog_addFunction(prog, tree);
    } else if (tree->type == EXPTREE_TYPE_IF) {

        CAP // dev
    }

    return 1;
}

//? ###########################################################################
//? ###########################################################################

int functionAddressesSubstitution() {
    DOT
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
        prog_addTree(prog, iter->func->tree);
        
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
