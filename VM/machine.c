

#include "stdio.h"
#include "stdlib.h"
#include "malloc.h"

#include "../globalDefinitions.h"
#include "../parser/interpreter/commands.h" // dev
#include "../parser/interpreter/datatypes.h" // dev
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
    return (int)sum;
}

//? ###########################################################################
//? ###########################################################################

typedef struct pair_t {

    //!SECTION

} Pair;

typedef struct stack_t {

    size_t *array;

    size_t capacity;
    size_t occupancy;

} Stack;

Stack* stringArray_init();
int stack_push(Stack *stack, size_t elem);
size_t stack_pop(Stack *stack);

Stack* stack_init() {

    Stack *stack = (Stack*) malloc(sizeof(Stack));

    stack->array     = (size_t*) malloc(sizeof(size_t)*100);
    stack->capacity  = 100;
    stack->occupancy = 0;

    stack_push(stack, 0);

    return stack;
}

int stack_push(Stack *stack, size_t elem) {

    assert(stack != NULL);

    if (stack->capacity == stack->occupancy) {
        stack->array = (size_t*) realloc(stack->array, sizeof(size_t)*(stack->capacity)*2);
        stack->capacity *= 2;
    }

    assert(stack->array != NULL);

    stack->array[(stack->occupancy)++] = elem;

    return 0;
}

size_t stack_pop(Stack *stack) {

    assert(stack != NULL);

    assert(stack->occupancy > 0);

    size_t ret = stack->array[stack->occupancy--];

    // if (stack->capacity % 2 == 0 &&
    //     stack->capacity > 100 &&
    //     stack->occupancy < stack->capacity/2-200
    // ) {
    //     stack->array = (size_t*) realloc(stack->array, sizeof(size_t)*(stack->capacity)*2);
    //     stack->capacity *= 2;
    // }

    return ret;
}

size_t stack_getPos(Stack *stack) {

    return stack->occupancy - 1;
}

size_t stack_getByPos(Stack *stack, size_t num) {

    assert(num > 0);
    assert(num < stack->occupancy);

    return stack->array[num];
}

//? ###########################################################################
//? ###########################################################################

typedef struct prog_t{

    char *arr;
    size_t len;


} Prog;

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

    size_t arg1 = stack_pop(stack);
    size_t arg2 = stack_pop(stack);

    assert(prog.arr[arg1] == DATATYPE_INT);
    assert(prog.arr[arg2] == DATATYPE_INT);

    int arg1_int = convert_BytesToInt(prog.arr + arg1 + 1);
    int arg2_int = convert_BytesToInt(prog.arr + arg1 + 2);

    stack_push(stack, arg1_int + arg2_int);

    return 0;
}

//? ###########################################################################
//? ###########################################################################

int decomp(Prog prog, FILE *stream) {

    size_t w = 0;

    size_t dataStart = 2;
    for (size_t q = 0; q < prog.len; ++q, ++w) {
        // for (size_t e = 0; e < 20; ++e) fprintf(stream, "|%u|", (unsigned char)(*(prog.arr+q+e))); fprintf(stream, "\n");


        fprintf(stream, "%06lu %06lu: ", w, q);
        // fprintf(stream, "%05lu: ", w);

        if(q >= dataStart && prog.arr[q] != COMMAND_END_OF_DATA) {

            switch (prog.arr[q]) {
            case DATATYPE_INT:
                fprintf(stream, "data int: %d\n", convert_BytesToInt(prog.arr+q+1));
                q += sizeof(int);
                break;
            default:
                fprintf(stream, "NOTHING: %d\n", (unsigned char)(prog.arr[q]));
                break;
            break;
            }

            // fprintf(stream, "dataByte: %d\n", (unsigned char)(prog.arr[q]));
            continue;
        }

        switch (prog.arr[q]) {
        case COMMAND_LENIN:
            fprintf(stream, "Lenin\n");
            break;
        case COMMAND_DATA_ADDRESS:
            dataStart = convert_BytesToSizeT(prog.arr+q+1);
            fprintf(stream, "data address is %lu\n", dataStart);
            q += sizeof(size_t);
            break;
        case COMMAND_PUSH:
            fprintf(stream, "push %lu\n", convert_BytesToSizeT(prog.arr+q+1));
            q += sizeof(size_t);
            break;
        case COMMAND_END_OF_DATA:
            fprintf(stream, "end of data\n");
            break;
        case COMMAND_PUSHARG:
            fprintf(stream, "pushArg %lu\n", convert_BytesToSizeT(prog.arr+q+1));
            q += sizeof(size_t);
            break;
        case COMMAND_CALL:
            fprintf(stream, "call %lu\n", convert_BytesToSizeT(prog.arr+q+1));
            q += sizeof(size_t);
            break;
        case COMMAND_RET:
            fprintf(stream, "ret\n");
            break;
        case COMMAND_END:
            fprintf(stream, "END\n");
            break;

        case COMMAND_STANDARD_ADD:
            fprintf(stream, "add\n");
            break;
        case COMMAND_STANDARD_SUB:
            fprintf(stream, "sub\n");
            break;
        case COMMAND_STANDARD_MUL:
            fprintf(stream, "mul\n");
            break;
        case COMMAND_STANDARD_DIV:
            fprintf(stream, "div\n");
            break;
        
        default:
            fprintf(stream, "NOTHING: %d\n", (unsigned char)(prog.arr[q]));
            break;
        }
    }

    return 0;
}


int execute(Prog prog, FILE *stream) {

    size_t pos = 2;
    size_t func = pos;

    int deb = TRUE;
    FILE *progout = stdout;

    Stack *stack = stack_init();

    // for (size_t q = 0; q < prog.len; ++q, ++w) {
    while (TRUE) {
        // for (size_t e = 0; e < 20; ++e) fprintf(stream, "|%u|", (unsigned char)(*(prog.arr+q+e))); fprintf(stream, "\n");


        // fprintf(stream, "%06lu %06lu: ", w, q);
        fprintf(stream, "%05lu: ", pos);

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
            stack_push(stack, convert_BytesToSizeT(prog.arr+pos+1));
            pos += sizeof(size_t) + 1;
            break;
        case COMMAND_END_OF_DATA:
            if (deb) fprintf(stream, "end of data\n");
            fprintf(progout, "Unexpected command\n");
            fprintf(progout, "Lenin is coming...\n");
            return 1;
        case COMMAND_PUSHARG:
            if (deb) fprintf(stream, "pushArg %lu\n", convert_BytesToSizeT(prog.arr+pos+1));
            size_t addr = stack_getByPos(stack ,func - convert_BytesToSizeT(prog.arr+pos+1) - sizeof(char));
            stack_push(stack, addr);
            pos += sizeof(size_t) + 1;
            break;
        case COMMAND_CALL:
            if (deb) fprintf(stream, "call %lu\n", convert_BytesToSizeT(prog.arr+pos+1));
            stack_push(stack, pos + sizeof(size_t) + 1);
            stack_push(stack, func);
            pos = convert_BytesToSizeT(prog.arr+pos+1);
            func = pos;
            break;
        case COMMAND_RET:
            if (deb) fprintf(stream, "ret\n");
            pos  = stack_pop(stack);
            func = stack_pop(stack);
            break;
        case COMMAND_END:
            if (deb) fprintf(stream, "END\n");
            assert(stack_getPos(stack) != 1);
            return 0;

        case COMMAND_STANDARD_ADD:
            if (deb) fprintf(stream, "add\n");
            func_add(stack);
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
        
        default:
            // fprintf(stream, "NOTHING: %d\n", (unsigned char)(prog.arr[q]));
            CAP
            break;
        }
    }

}

int virtMachine() {
DOT
    
    Prog prog = readInList("prog.bc");
// DOT
//     printf("%s", prog);
    // CAP

    decomp(prog, stdout);



    return 0;
}



int main() {
    return virtMachine();
}
