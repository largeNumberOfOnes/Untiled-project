#include "scanner_structs.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

#include "../../globalDefinitions.h"
#include "tokenTypesEnum.h"

Token* token_init(int type, char* string) {
    
    Token* token = (Token*) malloc(sizeof(Token));

    token->prev = NULL;
    token->next = NULL;

    token->type = type;
    token->string = string;

    return token;
}

Fence* fence_init(Token* token) {

    Fence* fence = (Fence*) malloc(sizeof(Fence));

    fence->prev = NULL;
    fence->next = NULL;

    fence->token = token;

    return fence;
}

int token_append(Token* token, int type, char* string) {

    while (token->next) token = token->next;

    Token* newToken = token_init(type, string);

    token->next = newToken;
    newToken->prev = token;

    return 0;
}

int fence_append(Fence* fence, Token* token) {

    while (fence->next) fence = fence->next;

    Fence* newFence = fence_init(token);

    fence->next = newFence;
    newFence->prev = fence;

    return 0;
}

Token* token_removeHead(Token *token) {

    if (!token) return NULL;

    Token *ret = token->next;

    free(token->string);
    free(token);

    ret->prev = NULL;

    return ret;
}

void token_printToFile(Token *token, FILE* file) {

    while (token) {
        // fprintf(file, "-> type: %d - ", token->type);
        fprintf(file, "─┬🢒 type: %d - ", token->type);
        printTokenTypeByInt(token->type, file);
        // fprintf(file, "   str : |%s|\n", token->string);
        fprintf(file, " └🢒 str : |%s|\n", token->string);
        token = token->next;
    }

}

char* getTokenTypeByInt(int val) {
    FILE* file = fopen("parser/scanner/tokenTypesEnum.h", "r");
    assert(file != NULL);
    char* buf = (char*) malloc(sizeof(char)*100);
    for (int q = 0; q < val + 5; ++q) {
        fgets(buf, 100, file);
    }
    fclose(file);

    return buf;
}

int printTokenTypeByInt(int val, FILE* file) {
    char* typeStr = getTokenTypeByInt(val);
    if (!typeStr) return -1;
    fprintf(file, "%s", typeStr+15);
    free(typeStr);
    return 0;
}

int isSovereign(Token *token) {

    switch (token->type) {
        case TOKEN_TYPE_SOVEREIGN_def:
        case TOKEN_TYPE_SOVEREIGN_type:
        case TOKEN_TYPE_SOVEREIGN_main:
            return 1;
        default:
            return 0;
    }
}


//? ###########################################################################
//? ###########################################################################

int printGraphOfTokenListToFile_body(Token* token, FILE* file) {

    while (token) {

        size_t len = strlen(token->string);
        char* tokenString = (char*) malloc(sizeof(char)*len);
        for (size_t q = 0, w = 0; q < len+1; ++q, ++w) {
            switch (token->string[q]) {
                case '"':
                case '>':
                case '<':
                case '{':
                case '}':
                    tokenString[w++] = '\\';
            }
            tokenString[w] = token->string[q];
        }

        //------ fprintf(file, "\t%lu [shape=record, label=\" type: %d | %s | %p | { %p | %p } \"]\n", (unsigned long) token, token->type, tokenString, token, token->prev, token->next);
        fprintf(file, "\t%lu [shape=record, label=\" { <03> type: %d | %s | %p | {<01> %p |<04>| <02> %p } } \"]\n", (unsigned long) token, token->type, tokenString, token, token->prev, token->next);
        // if (token->prev) {
        //     fprintf(file, "\t%lu:<01> -> %lu:<04> [color=\"red\"]\n",   (unsigned long) token, (unsigned long) token->prev);
        // }
        // if (token->next) {
        //     fprintf(file, "\t%lu:<02> -> %lu:<03> [color=\"green\"]\n", (unsigned long) token, (unsigned long) token->next);
        // }
        if (token->prev) {
            fprintf(file, "\t%lu -> %lu\n",   (unsigned long) token->prev, (unsigned long) token);
        }

        token = token->next;
    }

    return 0;
}

int printGraphOfTokelListToFile(Token* token, char* filePath) {

    FILE* file = fopen(filePath, "w+");

    fprintf(file, "digraph {\n");

    // fprintf(file, "rankdir=LR");

    // fprintf(file, "\t%lu [label=\"type: %d\\n%s\\n%p\"]\n", (unsigned long) token, token->type, token->string, token);

    printGraphOfTokenListToFile_body(token, file);

    fprintf(file, "}\n");

    fclose(file); 

    return 0;
}

int printGraphOfFencelListToFile(Fence* fence, char* filePath) {

    FILE* file = fopen(filePath, "w+");

    fprintf(file, "digraph {\n");

    // fprintf(file, "\trankdir=HR\n");

    int q = 0;
    while (fence) {

        // fprintf(file, "\t%lu [shape=record, label=\" type: %d | %s | %p | { %p | %p } \"]\n", (unsigned long) token, token->type, tokenString, token, token->prev, token->next);
        // fprintf(file, "{rank = same;\n");
        fprintf(file, "subgraph cluster%d{\n", q++);

        fprintf(file, "\t%lu [shape=record, label=\" { type: 000 | <05> %p | %p | { <01> %p |<04>| <02> %p } } \"]\n", (unsigned long) fence, fence->token, fence, fence->prev, fence->next);

        printGraphOfTokenListToFile_body(fence->token, file);

        // fprintf(file, "\t%lu:<05> -> %lu [color=\"blue\"]\n", (unsigned long) fence, (unsigned long) fence->token);
        fprintf(file, "\t%lu -> %lu [color=\"blue\"]\n", (unsigned long) fence, (unsigned long) fence->token);
        if (fence->prev) {
            fprintf(file, "\t%lu -> %lu\n",   (unsigned long) fence->prev, (unsigned long) fence);
        }
        // if (fence->prev) {
        //     fprintf(file, "\t%lu:<01> -> %lu:<04> [color=\"red\"]\n",   (unsigned long) fence, (unsigned long) fence->prev);
        // }
        // if (fence->next) {
        //     fprintf(file, "\t%lu:<02> -> %lu [color=\"green\"]\n", (unsigned long) fence, (unsigned long) fence->next);
        // }

        fprintf(file, "\n}\n");

        fence = fence->next;
    }
    fprintf(file, "}\n");

    fclose(file); 

    return 0;
}


static char* getClearString(char *str) {

    size_t len = strlen(str);
    char* newstr = (char*) malloc(sizeof(char)*len*2);
    int q = 0;
    int w = 0;
    // for (size_t q = 0, w = 0; q < len+1; ++q, ++w) {
    while (1) {
        switch (str[q]) {
            case '"':
            case '>':
            case '<':
            case '{':
            case '}':
                newstr[w++] = '\\';
        }
        newstr[w++] = str[q++];
        if (newstr[w-1] == '\0') break;
    }

    return newstr;
}


int token_printSingle(Token *token, FILE *file) {

    // fprintf(file, "%lu [shape=record, label=\"{Fence|self: %p| cont: %p| next: %p| prev: %p}\"]\n", (size_t) fence, fence, fence->token, fence->next, fence->prev);
    char* tokenClearSting = getClearString(token->string);
    fprintf(file, "%lu [shape=record, label=\"{Token|self: %p| type: ", (size_t) token, token);
    printTokenTypeByInt(token->type, file);
    fprintf(file, "| string: \\|%s\\|}\"]\n", tokenClearSting);
    free(tokenClearSting);

    return 1;
}

int token_printSequence(Token *token, FILE *file) {

    while (token) {

        token_printSingle(token, file);

        if (token->next) {
            fprintf(file, "%ld -> %ld [constraint=false]\n", (size_t) token, (size_t) token->next);
            fprintf(file, "%ld -> %ld [constraint=false]\n", (size_t) token->next, (size_t) token);
        }


        token = token->next;
    }

    return 0;
}

int fence_printSingle(Fence *fence, FILE *file) {

    fprintf(file, "subgraph Fence%lu {\n", (size_t)fence);
    // fprintf(file, "\tnode [style=filled,color=white];\n");
    fprintf(file, "\tstyle=filled;\n");
    fprintf(file, "\tbcolor=\"lightgrey;\"\n");
    // fprintf(file, "\trankdir=LR;\n");

    fprintf(file, "\t%lu [shape=record, fillcolor=\"red\", label=\"{Fence|self: %p| cont: %p| next: %p| prev: %p}\"]\n", (size_t) fence, fence, fence->token, fence->next, fence->prev);

    token_printSequence(fence->token, file);
    fprintf(file, "{rank=same;");
    fprintf(file, " %lu", (size_t) fence);

    Token *tok = fence->token;
    while (tok) {
        fprintf(file, " %lu", (size_t) tok);
        tok = tok->next;
    }
    fprintf(file, "}\n");

    fprintf(file, "%ld -> %ld [constraint=false]\n", (size_t) fence, (size_t) fence->token);


    fprintf(file, "\n");
    fprintf(file, "}\n");

    return 0;
}

int fence_printSequence(Fence *fence, FILE *file) {

    while (fence) {

        fence_printSingle(fence, file);


        if (fence->next) {
            fprintf(file, "%ld -> %ld\n", (size_t) fence, (size_t) fence->next);
            fprintf(file, "%ld -> %ld\n", (size_t) fence->next, (size_t) fence);
        }

        fence = fence->next;
    }

    return 0;
}

int fence_print(Fence *fence, char *filePath) {

    FILE *file = fopen(filePath, "w+");
    
    fprintf(file, "digraph {\n");
    // fprintf(file, "rankdir=\"TB\";\n\n");

    fence_printSequence(fence, file);

    fprintf(file, "\n}");

    fclose(file);

    return 0;
}
