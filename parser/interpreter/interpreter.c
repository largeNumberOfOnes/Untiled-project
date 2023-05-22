
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

size_t convert_BytesToSizeT(char *str) {

    size_t sum = 0;
    for (size_t q = 0; q < sizeof(size_t); ++q) {
        sum += (size_t)(unsigned char)(str[q]) << 8*q;
    }
    return sum;
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
    printf("stradd -> |%s| %lu\n", str, len);
    assert(arr != NULL);
    assert(str != NULL);

    if (arr->capacity == arr->occupancy) {
        arr->array = (char** ) realloc(arr->array, sizeof(char* )*(arr->capacity)*2);
        arr->lensa = (size_t*) realloc(arr->lensa, sizeof(size_t)*(arr->capacity)*2);
        (arr->capacity) *= 2;
    }

    assert(arr->array != NULL);

    arr->array[(arr->occupancy)++] = str;
    arr->lensa[(arr->occupancy)-1] = len;
    // printf("-2-- %lu\n", arr->lensa[q]);

    return arr->occupancy - 1;
}

int stringArray_printToStream(StringArray *arr, FILE *file) {

    for (size_t q = 0; q < arr->occupancy; ++q) {

        for (size_t w = 0; w < arr->lensa[q]; ++w) {
            putc(arr->array[q][w], file);
        }

    }

    return 0;
}

int stringArray_printToFile(StringArray *arr, char *filePath) {

    FILE *file = fopen(filePath, "w+");
    stringArray_printToStream(arr, file);
    fclose(file); file = NULL;

    return 0;
}

char* stringArray_getString(StringArray *arr, size_t num) {

    if (num >= arr->occupancy) return NULL;
    else return arr->array[(int)num];
}

int stringArray_getLength(StringArray *arr, size_t num) {

    if (num >= arr->occupancy) return 0;
    else return arr->lensa[(int)num];
}

int stringArray_changeString(StringArray *arr, size_t num, char *str, size_t len) {

    if (num >= arr->occupancy) { CAP } // dev
    else {
        free(arr->array[(int)num]);
        arr->array[num] = str;
        arr->lensa[num] = len;
    }

    return 0;
}

size_t stringArray_getByteCount(StringArray *arr) {

    size_t sum = 0;
    for (size_t q = 0; q < arr->occupancy; ++q) {
        sum += arr->lensa[q];
    }

    return sum;
}

size_t stringArray_symbolsUnderLine(StringArray *arr, size_t line) {

    size_t sum = 0;
    for (size_t q = 0; q < line; ++q) {
        sum += arr->lensa[q];
    }

    return sum; 
}

//? ###########################################################################
//? ###########################################################################

typedef struct prog_t {

    StringArray *code;
    StringArray *data;
    StringArray *main;

} Prog;

Prog* prog_init() {

    Prog *prog = (Prog*) malloc(sizeof(Prog));

    prog->code = stringArray_init();
    prog->data = stringArray_init();
    prog->main = stringArray_init();

    return prog;
}

Prog* prog_destr(Prog *prog) {

    CAP
    DOT
    return NULL;
}

int prog_combine(Prog *prog, char *filePath) {
    
    // StringArray *arr = stringArray_init();

    FILE *progFile = fopen(filePath, "w+");

    // for (size_t q = 0; q < prog->code->occupancy; ++q) {
    //     stringArray_add(
    //         arr,
    //         stringArray_getString(prog->code, q),
    //         stringArray_getLength(prog->code, q)
    //     );
    // }

    // for (size_t q = 0; q < prog->data->occupancy; ++q) {
    //     stringArray_add(
    //         arr,
    //         stringArray_getString(prog->code, q),
    //         stringArray_getLength(prog->code, q)
    //     );
    // }

    stringArray_printToStream(prog->code, progFile);
    stringArray_printToStream(prog->data, progFile);

    putc(COMMAND_END_OF_DATA, progFile);

    fclose(progFile); progFile = NULL;

    return 0;
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
    prog_codeAdd(prog, s, (strlen(((Function*)(tree->content))->name))+2);

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
        
        prog_dataAdd(prog, str, sizeof(int)+sizeof(char));
    } else {

        CAP
    }

    char *str = (char*) malloc(sizeof(char)*(sizeof(size_t)+1));
    str[0] = COMMAND_PUSH;
    size_t line = prog_getDataLineCount(prog)-1;
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
    size_t num = (size_t) num_;
    // printf("pushing arg: %s which is %d - %lu\n", (char*)(tree->content), num_, num);

    char *str = (char*) malloc(sizeof(char)*2 + sizeof(size_t));
    str[0] = COMMAND_PUSHARG;
    convert_ToBytes(str+1, &num, sizeof(size_t));
    prog_codeAdd(prog, str, sizeof(size_t) + 1);
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

int prog_workWithStandartFunctions(Prog *prog, size_t line) {

    char *str = stringArray_getString(prog->code, line);

    char com = COMMAND_LENIN;

    // printf("add found in line %lu\n", line);
    if (!strcmp(str+2, "add")) { com = COMMAND_STANDARD_ADD; }
    if (!strcmp(str+2, "sub")) { com = COMMAND_STANDARD_SUB; }
    if (!strcmp(str+2, "mul")) { com = COMMAND_STANDARD_MUL; }
    if (!strcmp(str+2, "div")) { com = COMMAND_STANDARD_DIV; }

    if (com != COMMAND_LENIN) {

        char *newstr = malloc(2*sizeof(char));
        newstr[0] = com;
        newstr[1] = '\0';
        stringArray_changeString(prog->code, line, newstr, sizeof(char));

        return TRUE;
    }

    return FALSE;
}

//? ###########################################################################
//? ###########################################################################

int prog_functionAddressesSubstitution(Prog *prog, Deflist *deflist) {
    
    for (size_t q = 0; q < prog->code->occupancy; ++q) {

        char *str = stringArray_getString(prog->code, q);

        if (str[0] == COMMAND_NEED_FOR_ADDRESS) {

            if (stringArray_getLength(prog->code, q) > 2 &&
                str[1] == COMMAND_CALL
            ) {

                printf("changing address %s\n", str+2);
                if (prog_workWithStandartFunctions(prog, q)) continue;

                char *newstr = malloc(2*sizeof(char)+sizeof(size_t));
                newstr[0] = COMMAND_CALL;
                size_t line = deflist_search(deflist, str+2)->line;
                convert_ToBytes(newstr+1, &line, sizeof(size_t));
                newstr[sizeof(char)+sizeof(size_t)] = '\0';
                stringArray_changeString(prog->code, q, newstr, sizeof(size_t)+1);
                str = NULL;

            } else {
                CAP // dev
            }
            
        }

    }

    for (size_t q = 0; q < prog->code->occupancy; ++q) {

        char *str = stringArray_getString(prog->code, q);

        if (str[0] == COMMAND_CALL) {

            char *newstr = malloc(2*sizeof(char)+sizeof(size_t));
            newstr[0] = COMMAND_CALL;
            size_t line = convert_BytesToSizeT(str+1);
            line = stringArray_symbolsUnderLine(prog->code, line) + sizeof(size_t);
            convert_ToBytes(newstr+1, &line, sizeof(size_t));
            newstr[sizeof(char)+sizeof(size_t)] = '\0';
            stringArray_changeString(prog->code, q, newstr, sizeof(size_t)+1);
            str = NULL;
            
        }

    }

    return 0;
}

int prog_putDataAddress(Prog *prog) {

    size_t len = stringArray_getByteCount(prog->code) + sizeof(size_t);
    char *str = (char*) malloc(sizeof(size_t)+2);
    str[0] = COMMAND_DATA_ADDRESS;
    convert_ToBytes(str+1, &len, sizeof(size_t));
    str[sizeof(size_t)+1] = '\0';

    stringArray_changeString(prog->code, 1, str, sizeof(size_t)+1);
    
    return 0;
}

//? ###########################################################################
//? ###########################################################################

int interpreter(Root *root) {

    glob_file = fopen("log.txt", "w+");

    MES("--------------------------------------------------------------------");

    Prog *prog = prog_init();

    prog_codeAddCommand(prog, COMMAND_LENIN);
    prog_codeAddCommand(prog, COMMAND_DATA_ADDRESS);

    Deflist *deflist_main = deflist_search(root->deflist, "main");
    prog_addTree(prog, deflist_main->func->tree, deflist_main->func);
    deflist_main = NULL;
    prog_codeAddCommand(prog, COMMAND_END);


    Deflist *iter = deflist_iteratorInit(root->deflist);
    while (iter) {

        if (iter->func->tree == NULL) {
            printf("--> %s\n", iter->func->name);
            iter = deflist_iteratorNext(iter);
            continue;
        }

        iter->line = prog_getCodeLineCount(prog);

        printf("name -> %s\n", iter->func->name);
        prog_addTree(prog, iter->func->tree, iter->func);
        
        prog_codeAddCommand(prog, COMMAND_RET);

        iter = deflist_iteratorNext(iter);
    }
    iter = NULL;

    // iter = deflist_iteratorInit(root->deflist);
    // while (iter) {
    //     printf("line: %lu\n", iter->line);
    //     iter = deflist_iteratorNext(iter);
    // }
    // CAP

    prog_printCodeToFile(prog, "code.txt");
    prog_functionAddressesSubstitution(prog, root->deflist);

    prog_putDataAddress(prog);

    prog_combine(prog, "prog.bc");


// DOT
//     prog_printDataToFile(prog, "data.txt");
// DOT
    MES("--------------------------------------------------------------------");
    fclose(glob_file);
    prog = prog_destr(prog);

    

    return 0;
}
