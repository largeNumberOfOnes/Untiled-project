

#include "stdio.h"
#include "stdlib.h"
#include "malloc.h"

#include "../globalDefinitions.h"
#include "../parser/interpreter/commands.h" // dev
#include "../parser/interpreter/datatypes.h" // dev
#include <assert.h>
#include <stddef.h>


int command_END_OF_DATA() { return 0; }
int command_PUSHARG() { return 0; }
int command_LENIN() { return 0; }
int command_PUSH() { return 0; }
int command_CALL() { return 0; }
int command_RET() { return 0; }
int command_END() { return 0; }



// int printCommand_CALL(FILE *file, FILE *stream) {
//     fprintf(stream, "call ");
//     for (int q = 0; q < 8; ++q) putc(getc(file), stream);
//     fprintf(stream, "\n");
// }

//? ###########################################################################
//? ###########################################################################

size_t convert_BytesToSizeT(char *str) {

    size_t sum = 0;
    for (size_t q = 0; q < sizeof(size_t); ++q) {
        // if (q == 0) sum += (size_t)(str[q]);
        // else sum += (size_t)(str[q]) >> 16*q;
        sum += (size_t)(unsigned char)(str[q]) << 8*q;
    }

    // return (size_t) (*str);
    // return (size_t)(unsigned char)(str[0]);
    return sum;
}

int convert_BytesToInt(char *str) {

    unsigned sum = 0;
    for (size_t q = 0; q < sizeof(int); ++q) {
        // if (q == 0) sum += (size_t)(str[q]);
        // else sum += (size_t)(str[q]) >> 16*q;
        sum += (unsigned)(unsigned char)(str[q]) << 8*q;
    }

    // return (size_t) (*str);
    // return (size_t)(unsigned char)(str[0]);
    return sum;
}

//? ###########################################################################
//? ###########################################################################

typedef struct pair_t {

    size_t elem;
    size_t type;

} Pair;

typedef struct stack_t {

    Pair *array;

    size_t capacity;
    size_t occupancy;

} Stack;

Stack* stringArray_init();
int stack_push(Stack *stack, Pair elem);
Pair stack_pop(Stack *stack);

Stack* stack_init() {

    Stack *stack = (Stack*) malloc(sizeof(Stack));

    stack->array     = (Pair*) malloc(sizeof(Pair)*100);
    stack->capacity  = 100;
    stack->occupancy = 0;

    stack_push(stack, (Pair){0, 0});

    return stack;
}

int stack_push(Stack *stack, Pair elem) {

    assert(stack != NULL);

    if (stack->capacity == stack->occupancy) {
        stack->array = (Pair*) realloc(stack->array, sizeof(Pair)*(stack->capacity)*2);
        stack->capacity *= 2;
    }

    assert(stack->array != NULL);

    stack->array[(stack->occupancy)++] = elem;

    return 0;
}

Pair stack_pop(Stack *stack) {

    assert(stack != NULL);

    assert(stack->occupancy > 0);

    Pair ret = stack->array[--stack->occupancy];
    stack->array[stack->occupancy] = (Pair){0, 0};

    // if (stack->capacity % 2 == 0 &&
    //     stack->capacity > 100 &&
    //     stack->occupancy < stack->capacity/2-200
    // ) {
    //     stack->array = (size_t*) realloc(stack->array, sizeof(size_t)*(stack->capacity)*2);
    //     stack->capacity *= 2;
    // }

    return ret;
}

int stack_dump(Stack *stack) {

    for (size_t q = 0; q < stack->occupancy; ++q) {
        printf("%08lu - %03lu\n", stack->array[q].elem, stack->array[q].type);
    }

    return 0;
}

size_t stack_getPos(Stack *stack) {

    return stack->occupancy - 1;
}

Pair stack_getByPos(Stack *stack, size_t num) {

    assert(num > 0);
    assert(num < stack->occupancy);

    return stack->array[num];
}

size_t stack_getLastPos(Stack *stack) {

    assert(stack != NULL);

    return stack->occupancy - 1;
}

//? ###########################################################################
//? ###########################################################################

typedef struct prog_t{

    char *arr;
    size_t len;

    size_t dataStart;

} Prog;

enum STACK_ELEM_TYPES {
    STACK_ELEM_TYPES_NULL,
    STACK_ELEM_TYPES_DATA_ADDRESS,
    STACK_ELEM_TYPES_FUNC_ADDRESS,
    STACK_ELEM_TYPES_RET_ADDRESS,
    STACK_ELEM_TYPES_INLINE_INT,
};

size_t getByteCount(FILE *file) {
    
    fseek (file, 0, SEEK_END);
    size_t ret = ftell(file);
    fseek (file, 0, SEEK_SET);
    return ret;
}

Prog readInList(char *filePath) {

    FILE *file = fopen(filePath, "rb");
    if (!file) { perror("Error opening file"); exit(1); }
    size_t len = getByteCount(file);
    char *arr = (char*) malloc(sizeof(char)*len);
    for (size_t q = 0; q < len; ++q) {
        arr[q] = getc(file);
        // printf("-> %d\n", (int)(arr[q]));
    }
    fclose(file);

    Prog prog;
    prog.arr = arr;
    prog.len = len;

    return prog;
}

//? ###########################################################################
//? ###########################################################################

int func_add(Stack *stack, Prog *prog) {

    Pair arg1 = stack_pop(stack);
    Pair arg2 = stack_pop(stack);

    int arg1_int = 0;
    if (arg1.type == STACK_ELEM_TYPES_DATA_ADDRESS) {
        // printf("arg1: %lu\n", prog->dataStart+arg1.elem*5);
        arg1_int = convert_BytesToInt(prog->arr+prog->dataStart+arg1.elem*5+1);
        // printf("arg1: %d\n", arg1_int);
    } else if (arg1.type == STACK_ELEM_TYPES_INLINE_INT) {
        arg1_int = (int)(long)arg1.elem;
    } else {
        CAP
    }
    
    int arg2_int = 0;
    switch (arg2.type) {
    case STACK_ELEM_TYPES_DATA_ADDRESS:
        // printf("arg2: %lu\n", prog->dataStart+arg2.elem*5);
        arg2_int = convert_BytesToInt(prog->arr+prog->dataStart+arg2.elem*5+1);
        // printf("arg2: %d\n", arg2_int);
        break;
    case STACK_ELEM_TYPES_INLINE_INT:
        arg2_int = (int)(long)arg2.elem;
        break;
    default:
        CAP
    }
    // assert(prog.arr[arg2] == DATATYPE_INT);

    // int arg2_int = convert_BytesToInt(prog.arr + arg1 + 2);

    stack_push(stack, (Pair){(size_t)(unsigned)(arg1_int + arg2_int), STACK_ELEM_TYPES_INLINE_INT});

    return 0;
}
int func_sub(Stack *stack, Prog *prog) {

    Pair arg1 = stack_pop(stack);
    Pair arg2 = stack_pop(stack);

    int arg1_int = 0;
    if (arg1.type == STACK_ELEM_TYPES_DATA_ADDRESS) {
        // printf("arg1: %lu\n", prog->dataStart+arg1.elem*5);
        arg1_int = convert_BytesToInt(prog->arr+prog->dataStart+arg1.elem*5+1);
        // printf("arg1: %d\n", arg1_int);
    } else if (arg1.type == STACK_ELEM_TYPES_INLINE_INT) {
        DOT
        arg1_int = (int)(long)arg1.elem;
    } else {
        CAP
    }
    
    int arg2_int = 0;
    switch (arg2.type) {
    case STACK_ELEM_TYPES_DATA_ADDRESS:
        // printf("arg2: %lu\n", prog->dataStart+arg2.elem*5);
        arg2_int = convert_BytesToInt(prog->arr+prog->dataStart+arg2.elem*5+1);
        // printf("arg2: %d\n", arg2_int);
        break;
    case STACK_ELEM_TYPES_INLINE_INT:
        DOT
        arg2_int = (int)(long)arg2.elem;
        break;
    default:
        CAP
    }
    // assert(prog.arr[arg2] == DATATYPE_INT);

    // int arg2_int = convert_BytesToInt(prog.arr + arg1 + 2);


    // printf("arg1: %d\n", arg1_int);
    // printf("arg2: %d\n", arg2_int);

    stack_push(stack, (Pair){(size_t)(unsigned)(arg1_int - arg2_int), STACK_ELEM_TYPES_INLINE_INT});

    return 0;
}
int func_mul(Stack *stack, Prog *prog) {

    Pair arg1 = stack_pop(stack);
    Pair arg2 = stack_pop(stack);

    int arg1_int = 0;
    if (arg1.type == STACK_ELEM_TYPES_DATA_ADDRESS) {
        // printf("arg1: %lu\n", prog->dataStart+arg1.elem*5);
        arg1_int = convert_BytesToInt(prog->arr+prog->dataStart+arg1.elem*5+1);
        // printf("arg1: %d\n", arg1_int);
    } else if (arg1.type == STACK_ELEM_TYPES_INLINE_INT) {
        arg1_int = (int)(long)arg1.elem;
    } else {
        CAP
    }
    
    int arg2_int = 0;
    switch (arg2.type) {
    case STACK_ELEM_TYPES_DATA_ADDRESS:
        // printf("arg2: %lu\n", prog->dataStart+arg2.elem*5);
        arg2_int = convert_BytesToInt(prog->arr+prog->dataStart+arg2.elem*5+1);
        // printf("arg2: %d\n", arg2_int);
        break;
    case STACK_ELEM_TYPES_INLINE_INT:
        arg2_int = (int)(long)arg2.elem;
        break;
    default:
        CAP
    }
    // assert(prog.arr[arg2] == DATATYPE_INT);

    // int arg2_int = convert_BytesToInt(prog.arr + arg1 + 2);

    stack_push(stack, (Pair){(size_t)(unsigned)(arg1_int * arg2_int), STACK_ELEM_TYPES_INLINE_INT});

    return 0;
}
int func_div(Stack *stack, Prog *prog) {

    Pair arg1 = stack_pop(stack);
    Pair arg2 = stack_pop(stack);

    int arg1_int = 0;
    if (arg1.type == STACK_ELEM_TYPES_DATA_ADDRESS) {
        // printf("arg1: %lu\n", prog->dataStart+arg1.elem*5);
        arg1_int = convert_BytesToInt(prog->arr+prog->dataStart+arg1.elem*5+1);
        // printf("arg1: %d\n", arg1_int);
    } else if (arg1.type == STACK_ELEM_TYPES_INLINE_INT) {
        arg1_int = (int)(long)arg1.elem;
    } else {
        CAP
    }
    
    int arg2_int = 0;
    switch (arg2.type) {
    case STACK_ELEM_TYPES_DATA_ADDRESS:
        // printf("arg2: %lu\n", prog->dataStart+arg2.elem*5);
        arg2_int = convert_BytesToInt(prog->arr+prog->dataStart+arg2.elem*5+1);
        // printf("arg2: %d\n", arg2_int);
        break;
    case STACK_ELEM_TYPES_INLINE_INT:
        arg2_int = (int)(long)arg2.elem;
        break;
    default:
        CAP
    }
    // assert(prog.arr[arg2] == DATATYPE_INT);

    // int arg2_int = convert_BytesToInt(prog.arr + arg1 + 2);

    stack_push(stack, (Pair){(size_t)(unsigned)(arg1_int / arg2_int), STACK_ELEM_TYPES_INLINE_INT});

    return 0;
}
int func_print_int(Stack *stack, Prog *prog) {

    Pair arg = stack_getByPos(stack, stack_getLastPos(stack));
    
    int arg_int = 0;
    if (arg.type == STACK_ELEM_TYPES_DATA_ADDRESS) {
        // printf("arg1: %lu\n", prog->dataStart+arg1.elem*5);
        arg_int = convert_BytesToInt(prog->arr+prog->dataStart+arg.elem*5+1);
        // printf("arg1: %d\n", arg1_int);
    } else if (arg.type == STACK_ELEM_TYPES_INLINE_INT) {
        arg_int = (int)(long)arg.elem;
    } else {
        CAP
    }

    printf("%d\n", arg_int);
    // CAP

    return 0;
}

//? ###########################################################################
//? ###########################################################################

Prog decomp(Prog prog, FILE *stream) {

    size_t w = 0;

    int deb = TRUE;
    deb = FALSE;

    prog.dataStart = 2;
    for (size_t q = 0; q < prog.len; ++q, ++w) {
        // for (size_t e = 0; e < 20; ++e) fprintf(stream, "|%u|", (unsigned char)(*(prog.arr+q+e))); fprintf(stream, "\n");


        if (deb) fprintf(stream, "%06lu %06lu: ", w, q);
        // fprintf(stream, "%05lu: ", w);

        if(q >= prog.dataStart && prog.arr[q] != COMMAND_END_OF_DATA) {

            switch (prog.arr[q]) {
            case DATATYPE_INT:
                if (deb) fprintf(stream, "data int: %d\n", convert_BytesToInt(prog.arr+q+1));
                q += sizeof(int);
                break;
            default:
                if (deb) fprintf(stream, "NOTHING: %d\n", (unsigned char)(prog.arr[q]));
                break;
            break;
            }

            // fprintf(stream, "dataByte: %d\n", (unsigned char)(prog.arr[q]));
            continue;
        }

        switch (prog.arr[q]) {
        case COMMAND_LENIN:
            if (deb) fprintf(stream, "Lenin\n");
            break;
        case COMMAND_DATA_ADDRESS:
            prog.dataStart = convert_BytesToSizeT(prog.arr+q+1);
            if (deb) fprintf(stream, "data address is %lu\n", prog.dataStart);
            q += sizeof(size_t);
            break;
        case COMMAND_PUSH:
            if (deb) fprintf(stream, "push %lu\n", convert_BytesToSizeT(prog.arr+q+1));
            q += sizeof(size_t);
            break;
        case COMMAND_END_OF_DATA:
            if (deb) fprintf(stream, "end of data\n");
            break;
        case COMMAND_PUSHARG:
            if (deb) fprintf(stream, "pushArg %lu\n", convert_BytesToSizeT(prog.arr+q+1));
            q += sizeof(size_t);
            break;
        case COMMAND_CALL:
            if (deb) fprintf(stream, "call %lu\n", convert_BytesToSizeT(prog.arr+q+1));
            q += sizeof(size_t);
            break;
        case COMMAND_RET:
            if (deb) fprintf(stream, "ret %lu\n", convert_BytesToSizeT(prog.arr+q+1));
            q += sizeof(size_t);
            break;
        case COMMAND_END:
            if (deb) fprintf(stream, "END\n");
            break;

        case COMMAND_STANDARD_ADD:
            if (deb) fprintf(stream, "add\n");
            break;
        case COMMAND_STANDARD_SUB:
            if (deb) fprintf(stream, "sub\n");
            break;
        case COMMAND_STANDARD_MUL:
            if (deb) fprintf(stream, "mul\n");
            break;
        case COMMAND_STANDARD_DIV:
            if (deb) fprintf(stream, "div\n");
            break;
        case COMMAND_STANDARD_PRINT_INT:
            if (deb) fprintf(stream, "print_int\n");
            break;
        
        default:
            if (deb) fprintf(stream, "NOTHING: %d\n", (unsigned char)(prog.arr[q]));
            break;
        }
    }

    return prog;
}


int execute(Prog prog, FILE *stream) {

    size_t pos = 10;
    size_t func = 1;

    int deb = TRUE;
    deb = FALSE;
    FILE *progout = stdout;

    Stack *stack = stack_init();

    // for (size_t q = 0; q < prog.len; ++q, ++w) {
    while (TRUE) {
        // for (size_t e = 0; e < 20; ++e) fprintf(stream, "|%u|", (unsigned char)(*(prog.arr+q+e))); fprintf(stream, "\n");


        // fprintf(stream, "%06lu %06lu: ", w, q);
        if (deb) fprintf(stream, "%05lu: ", pos);

        switch (prog.arr[pos]) {
        case COMMAND_LENIN:
            if (deb) fprintf(stream, "Lenin\n");
            fprintf(progout, "Lenin has come...\n");
            return 1;
        case COMMAND_DATA_ADDRESS:
            if (deb) fprintf(stream, "data address is %lu\n", convert_BytesToSizeT(prog.arr+pos+1));
            fprintf(progout, "Unexpected command\n");
            fprintf(progout, "Lenin is coming...\n");
            return 1;
        case COMMAND_PUSH:
            if (deb) fprintf(stream, "push %lu\n", convert_BytesToSizeT(prog.arr+pos+1));
            stack_push(stack, (Pair){convert_BytesToSizeT(prog.arr+pos+1), STACK_ELEM_TYPES_DATA_ADDRESS});
            pos += sizeof(size_t) + 1;
            break;
        case COMMAND_END_OF_DATA:
            if (deb) fprintf(stream, "end of data\n");
            fprintf(progout, "Unexpected command\n");
            fprintf(progout, "Lenin is coming...\n");
            return 1;
        case COMMAND_PUSHARG:
            if (deb) fprintf(stream, "pushArg %lu\n", convert_BytesToSizeT(prog.arr+pos+1));
            // printf("----\n");
            // printf("pos: %lu\n", pos);
            // printf("func: %lu\n", func);
            // printf("val: %lu\n", convert_BytesToSizeT(prog.arr+pos+1));

            // print
            Pair addr = stack_getByPos(stack, func - convert_BytesToSizeT(prog.arr+pos+1) );
            stack_push(stack, addr);
            pos += sizeof(size_t) + 1;
            break;
        case COMMAND_CALL:
            if (deb) fprintf(stream, "call %lu\n", convert_BytesToSizeT(prog.arr+pos+1));
            stack_push(stack, (Pair){func, STACK_ELEM_TYPES_FUNC_ADDRESS});
            stack_push(stack, (Pair){pos + sizeof(size_t) + 1, STACK_ELEM_TYPES_RET_ADDRESS});
            pos = convert_BytesToSizeT(prog.arr+pos+1);
            func = stack_getLastPos(stack)-2;
            break;
        case COMMAND_RET:
            // if (deb) fprintf(stream, "ret\n");
            if (deb) fprintf(stream, "ret %lu\n", convert_BytesToSizeT(prog.arr+pos+1));
            size_t args = convert_BytesToSizeT(prog.arr+pos+1);
            Pair temp_ret = stack_pop(stack);
            Pair ret = stack_pop(stack);
            assert(ret.type == STACK_ELEM_TYPES_RET_ADDRESS);
            pos  = ret.elem;
            ret = stack_pop(stack);
            assert(ret.type == STACK_ELEM_TYPES_FUNC_ADDRESS);
            func = ret.elem;
            while (args > 0) { stack_pop(stack); --args; }
            stack_push(stack, temp_ret);
            // pos += sizeof(size_t) + 1;
            break;
        case COMMAND_END:
            if (deb) fprintf(stream, "END\n");
            assert(stack_getPos(stack) != 2);
            pos +=1;
            return 0;

        case COMMAND_STANDARD_ADD:
            if (deb) fprintf(stream, "add\n");
            func_add(stack, &prog);
            pos+=1;
            break;
        case COMMAND_STANDARD_SUB:
            if (deb) fprintf(stream, "sub\n");
            func_sub(stack, &prog);
            pos+=1;
            break;
        case COMMAND_STANDARD_MUL:
            if (deb) fprintf(stream, "mul\n");
            func_mul(stack, &prog);
            pos+=1;
            break;
        case COMMAND_STANDARD_DIV:
            if (deb) fprintf(stream, "div\n");
            func_div(stack, &prog);
            pos+=1;
            break;
        case COMMAND_STANDARD_PRINT_INT:
            if (deb) fprintf(stream, "print_int\n");
            func_print_int(stack, &prog);
            pos+=1;
            break;
        
        default:
            if (deb) fprintf(stream, "Undefined command: %d\n", (unsigned char)(prog.arr[pos]));
            CAP
            break;
        }
        if (deb) printf("----\n");
        // if (deb) fprintf(stream, "pos : %lu\n", pos);
        // if (deb) fprintf(stream, "func: %lu\n", func);
        if (deb) stack_dump(stack);
        if (deb) printf("----\n");
    }

}

int virtMachine() {
    
    Prog prog = readInList("prog.bc");
// DOT
//     printf("%s", prog);
    // CAP

    prog = decomp(prog, stdout);
    // DOT
    execute(prog, stdout);
    // DOT
    // CAP



    return 0;
}



int main() {
    return virtMachine();
}
