#include <stddef.h>
#include <assert.h>
#include <stdio.h> 
#include <malloc.h>
#include <ctype.h>
#include <errno.h>
#include <string.h>
#include "errorNumbers.h"

#define DOT printf("\033[91mDOT: SOMEONE CALL ME FROM LINE: %d\n\033[39m", __LINE__);



size_t token_getMaxStringLen() {

    return 100;
}



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


char* goUntilSymbol(FILE* file, char c, char* string, size_t* pos) {

    while (1) {

        if (*pos >= token_getMaxStringLen()) {
            errno = TOKEN_STRING_IS_OVER;
            return string;
        }

        char s = getc(file);

        if (s == EOF) {
            errno = NO_CLOSE_SYMBOL_IN_FILE;
            return string;
        }

        string[(*pos)++] = s;
        
        if (s == c) {

            return string;
        }

    }

    assert(0);
    return NULL;
}

char* getTokenString(FILE* file) {

    char* string = (char*) malloc(sizeof(char)*token_getMaxStringLen());
    size_t pos = 0;

    while (1) {

        if (pos >= token_getMaxStringLen()-1) {
            errno = TOKEN_STRING_IS_OVER;
            goto END_OF_getTokenString_with_string_return; 
        }

        char c = getc(file);
        printf("Symbol %c readed.\n", c); // log

        if (pos == 0 && c == EOF) {
            errno = TRYING_TO_CREATING_TOKEN_FROM_EOF;
            return NULL;
        }

        switch (c) {
            case '"':
                goUntilSymbol(file, '"', string, &pos);
                if (errno) {
                    return NULL;
                }
                break;
            case '(':
            case ')':
            case '{':
            case '}':
            case '[':
            case ']':
            case ':':
                if (pos == 0) {
                    string[pos++] = c;
                    goto END_OF_getTokenString_with_string_return;
                } else {
                    ungetc(c, file);
                    printf("Symbol %c returned.\n", c); // log
                    goto END_OF_getTokenString_with_string_return;
                }
                break; // just for bee
            case EOF:
                goto END_OF_getTokenString_with_string_return;
                break;
        }

        if (isspace(c)) {

            goto END_OF_getTokenString_with_string_return;
        }

        string[pos++] = c;

    }

    END_OF_getTokenString_with_string_return:
        // string[(pos < token_getMaxStringLen()-1) ? (pos) : (token_getMaxStringLen()-1)] = 0;
        string[pos] = 0;
        printf("The final string is |%s|\n", string);
        return string;
}

int throughSpaces(FILE* file) {

    char c = getc(file);
    while (isspace(c)) c = getc(file);
    ungetc(c, file);

    return 0;
}


Token* createGlobalTokenList(FILE* file) {


    Token* head = NULL;
    Token* curToken = head;
    int isHeadEmpty = 1;

    while (1) {

        throughSpaces(file);

        char* tokenString = getTokenString(file);
        if (!tokenString || errno) {
            switch (errno) {
                case TRYING_TO_CREATING_TOKEN_FROM_EOF:
                    printf("EOF was reached.\n"); // log
                    return head;
                case TOKEN_STRING_IS_OVER:
                case NO_CLOSE_SYMBOL_IN_FILE:
                    return NULL;
                default:
                    printf("|%s|, errno = %d\n", tokenString, errno); // log
                    assert(0); // THERE MUST BE ERROR PROCESSING
            }
        }

        if (isHeadEmpty) {
            head = token_init(0, tokenString);
            curToken = head;
            isHeadEmpty = 0;
        } else {
            token_append(curToken, 0, tokenString);
            curToken = curToken->next;
        }
        
    }

    return 0;
}

int printGraphOfTokelListToFile_body(FILE* file, Token* token) {

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

int printGraphOfTokelListToFile(char* filePath, Token* token) {

    FILE* file = fopen(filePath, "w+");

    fprintf(file, "digraph {\n");

    // fprintf(file, "rankdir=LR");

    // fprintf(file, "\t%lu [label=\"type: %d\\n%s\\n%p\"]\n", (unsigned long) token, token->type, token->string, token);

    printGraphOfTokelListToFile_body(file, token);

    fprintf(file, "}\n");

    fclose(file); 

    return 0;
}

int printGraphOfFencelListToFile(char* filePath, Fence* fence) {

    FILE* file = fopen(filePath, "w+");

    fprintf(file, "digraph {\n");

    // fprintf(file, "\trankdir=HR\n");

    int q = 0;
    while (fence) {

        // fprintf(file, "\t%lu [shape=record, label=\" type: %d | %s | %p | { %p | %p } \"]\n", (unsigned long) token, token->type, tokenString, token, token->prev, token->next);
        // fprintf(file, "{rank = same;\n");
        fprintf(file, "subgraph cluster%d{\n", q++);

        fprintf(file, "\t%lu [shape=record, label=\" { type: 000 | <05> %p | %p | { <01> %p |<04>| <02> %p } } \"]\n", (unsigned long) fence, fence->token, fence, fence->prev, fence->next);

        printGraphOfTokelListToFile_body(file, fence->token);

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

int isSovereign(Token* token) {
    
    if (!strcmp(token->string, "import") ||
        !strcmp(token->string, "def"   ) ||
        !strcmp(token->string, "main"  )
    ) {
        return 1;
    }

    return 0;
}

Fence* createFence(Token* token) {

    assert(token != NULL); // It is laziness

    Fence* head     = NULL;
    Fence* curFence = head;
    int isHeadEmpty = 0;

    while (token) {

        if (isSovereign(token)) {
            if (!isHeadEmpty) {
                head = fence_init(token);
                curFence = head;
                isHeadEmpty = 1;
            } else {
                assert(token->prev);
                token->prev->next = NULL;
                token->prev = NULL;
                fence_append(curFence, token);
                curFence = curFence->next;
            }
        }

        token = token->next;
    }

    return head;
}

int lexicalAnalyzer(FILE* file) {

    assert(file != NULL);
    // assert(isTheLastSymbolNewLine());

    printf("START\n");



    Token* token = createGlobalTokenList(file);
    printf("EEE: %d\n", errno);
    Fence* fence = createFence(token);

    printGraphOfFencelListToFile("graph001", fence);




    printf("Something really important.\n");

 
    return 0;
}
