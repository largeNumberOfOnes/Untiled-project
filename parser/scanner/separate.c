 
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "scanner_structs.h"

#include <stdlib.h>

#include "../../globalDefinitions.h"

#include "detTypes.h"
#include "scanner.h"

//? ###########################################################################
//? ###########################################################################

static int isTerminatorSymbol(char c) {
    return (c == '\0') || (c == EOF);
}

static int isOperatorSymbol(char c) {
    switch (c) {
    case ':':
    case '-':
    case '+':
    case '*':
    case '/':
    case '&':
    case '^':
    case '%':
    case '$':
    case '#':
    case '=':
        return TRUE;
    default:
        return FALSE;
    }
}

static char* selectWord(char *p) {
    while (1) {
        if (
            ('A' <= *p && *p <= 'Z') ||
            ('a' <= *p && *p <= 'z') ||
            ('0' <= *p && *p <= '9') ||
            (*p == '_')
        ) {
            ++p;
        } else {
            return p;
            // return state_end(str, token, p, FROM_STATE_WORD);
        }
    }
}

static char* selectOperator(char *p) {
    while (1) {
        if (isOperatorSymbol(*p)) {
            ++p;
        } else {
            return p;
        }
    }
}

//? ###########################################################################
//? ###########################################################################

static int state_end(char *str, Token *token, char *p, int from) {

    if (isTerminatorSymbol(*str)) {
        return 0;
    }

    char *tokenString = (char*) malloc(sizeof(char)*(p-str+1));
    strncpy(tokenString, str, p-str);
    tokenString[p-str] = '\0';

    assert(token != NULL);
    
    token_append(token, from, tokenString);

    return p-str;
}

static int state_type(char *str, Token *token, char *p) {

    while (1) {
        if (
            ('A' <= *p && *p <= 'Z') ||
            ('a' <= *p && *p <= 'z') ||
            ('0' <= *p && *p <= '9') ||
            (*p == '(' || *p == ')') ||
            (*p == '-' || *p == '>')
        ) {
            ++p;
        } else {
            return state_end(str, token, p, FROM_STATE_TYPE);
        }
    }
}

static int state_word(char *str, Token *token, char *p) {

    p = selectWord(p);
    return state_end(str, token, p, FROM_STATE_WORD);
}

static int state_operator(char *str, Token *token, char *p) {

    p = selectOperator(p);
    return state_end(str, token, p, FROM_STATE_OPERATOR);
}

static int state_number(char *str, Token *token, char *p) {
    
    int isPoint = 0;

    while (1) {
        if ('0' <= *p && *p <= '9') {
            ++p;
        } else if (!isPoint && *p == '.') {
            isPoint = 1;
            ++p;
        } else {
            if (isPoint) {
                return state_end(str, token, p, FROM_STATE_FLOAT);
            } else {
                return state_end(str, token, p, FROM_STATE_INT);
            }
            break;
        }
    }
}

static int state_bracket(char *str, Token *token, char *p) {

    if (*p == '(') {
        ++p;
        return state_end(str, token, p, FROM_STATE_PARENTHESES_BRACKET_OPEN);
    } else if (*p == ')') {
        ++p;
        return state_end(str, token, p, FROM_STATE_PARENTHESES_BRACKET_CLOSE);
    }

    return 0;
}

static int state_separator(char *str, Token *token, char *p) {

    while (1) { // dev
        if (isspace(*p)) {
            ++p;
        } else {
            return p-str;
        }
    }
}

static int state_string(char *str, Token *token, char *p) {

    assert(*p == '\'');
    ++p;

    while (*p != '\'') ++p;
    ++p;

    return state_end(str, token, p, FROM_STATE_STRING);
}

static int state_acute(char *str, Token *token, char *p) {

    assert(*p == '`');
    ++p;

    if (isOperatorSymbol(*p)) {
        p = selectOperator(p);
        return state_end(str, token, p, FROM_STATE_ACUTED_OPERATOR);
    } else if ('a' <= *p && *p <= 'z') {
        p = selectWord(p);
        return state_end(str, token, p, FROM_STATE_ACUTED_FUNCTION);
    }

    return 0;
}

//? ###########################################################################
//? ###########################################################################

static int state_start(char *str, Token *token) {

    DOT

    while (1) {
        int retExit = -1;
        DOT
        if ('A' <= *str && *str <= 'Z') {
            DOT
            retExit = state_type(str, token, str);
        } else if ('a' <= *str && *str <= 'z') {
            DOT
            retExit = state_word(str, token, str);
        } else if ('0' <= *str && *str <= '9') {
            retExit = state_number(str, token, str);
        } else if (*str == '(' || *str == ')') {
            retExit = state_bracket(str, token, str);
        } else if (isspace(*str)) {
            retExit = state_separator(str, token, str);
        } else if (isOperatorSymbol(*str)) {
            retExit = state_operator(str, token, str);
        } else if (*str == '\'')  {
            retExit = state_string(str, token, str);
        } else if (*str == '`') {
            retExit = state_acute(str, token, str);
        } else if (isTerminatorSymbol(*str)) {
            retExit = 0;
            // return state_end(str, token, str, FROM_STATE_END);
        } else {
            errout("Undefined symbol: %c[%d]", *str, (int)(*str));
            exit(0); // dev
        }

        if (!retExit) {
            DOT
            return 0;
        } else {
            str += retExit;
        }
    }
}

//? ###########################################################################
//? ###########################################################################

Token* separate(char* str) {

    Token* token = token_init(FROM_STATE_START, NULL);
    state_start(str, token);
    token = token_removeHead(token);

    return token;
}
