#include <stdio.h>

#include "tokenTypesEnum.h"

#ifndef SCANNER_STRUCTS_H
#define SCANNER_STRUCTS_H

typedef struct token_t {

    struct token_t* prev;
    struct token_t* next;

    int type;
    char* string;

} Token;

typedef struct fence_t {

    struct fence_t* prev;
    struct fence_t* next;

    struct token_t* token;

} Fence;


Token* token_init(int type, char* string);
int token_append(Token* token, int type, char* string);
int token_printSingleToFile(Token *token, FILE *file);
void token_printToFile(Token *token, FILE* file);

Fence* fence_init(Token* token);
int fence_append(Fence* fence, Token* token);


int printTokenTypeByInt(int val, FILE* file);

int isSovereign(Token *token);


int printGraphOfFencelListToFile(Fence* fence, char* filePath);
int printGraphOfTokelListToFile(Token* token, char* filePath);
int printGraphOfTokenListToFile_body(Token* token, FILE* file);

Token* token_removeHead(Token *token);




// dev
int fence_printSingle(Fence *fence, FILE *file);
int fence_print(Fence *fence, char *filePath);







#endif // SCANNER_STRUCTS_H
