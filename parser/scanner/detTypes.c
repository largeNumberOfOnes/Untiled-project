#include "scanner.h"
#include "scanner_structs.h"
#include <string.h>

#include "../../globalDefinitions.h"
#include "tokenTypesEnum.h"

static const int CANNOT_DEFINE_TYPE = -1;

static int detTokenTypeFromWord(char* str) {

    if (!strcmp(str, "def"))  { return TOKEN_TYPE_SOVEREIGN_def;  }
    if (!strcmp(str, "type")) { return TOKEN_TYPE_SOVEREIGN_type; }
    if (!strcmp(str, "main")) { return TOKEN_TYPE_SOVEREIGN_main; }

    return TOKEN_TYPE_VARIABLE;
}

static int detTokenTypeFromSimleState(int simpleState) {
//dev // why is it need? For whom?
    switch (simpleState) {
        case FROM_STATE_PARENTHESES_BRACKET_OPEN:  return TOKEN_TYPE_PARENTHESES_BRACKET_OPEN;
        case FROM_STATE_PARENTHESES_BRACKET_CLOSE: return TOKEN_TYPE_PARENTHESES_BRACKET_CLOSE;
        case FROM_STATE_TYPE:                      return TOKEN_TYPE_TYPE;
        case FROM_STATE_INT:                       return TOKEN_TYPE_INT;
        case FROM_STATE_FLOAT:                     return TOKEN_TYPE_FLOAT;
        case FROM_STATE_OPERATOR:                  return TOKEN_TYPE_OPERATOR;
        case FROM_STATE_UNDEF:                     return TOKEN_TYPE_UNDEF;
        case FROM_STATE_START:                     return TOKEN_TYPE_UNDEF;
        case FROM_STATE_END:                       return TOKEN_TYPE_UNDEF;
        case FROM_STATE_ACUTED_OPERATOR:           return TOKEN_TYPE_ACUTED_OPERATOR;
        case FROM_STATE_ACUTED_FUNCTION:           return TOKEN_TYPE_ACUTED_FUNCTION;
        case FROM_STATE_STRING:                    return TOKEN_TYPE_STRING;

        // case FROM_STATE_WORD:  token_type = TOKEN_TY

        default: return CANNOT_DEFINE_TYPE;
    }

}

int detTypes(Token *token) {

    // int isTypeDet = 0;

    // isTypeDet = 1;
    DOT

    while (token) {

        int token_type = detTokenTypeFromSimleState(token->type);        
        if (token_type != CANNOT_DEFINE_TYPE) {
            token->type = token_type;
            goto CONTINUE;
        }

        if (token->type == FROM_STATE_WORD) {
            DOT
            token->type = detTokenTypeFromWord(token->string);
            goto CONTINUE;
        }

        token->type = TOKEN_TYPE_UNDEF;

        CONTINUE:
            token = token->next;
            // printf("%p\n", token);
    }

    return 0;

}



