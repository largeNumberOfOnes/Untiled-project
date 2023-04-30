 
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




Token* separate(char* str);

//? ###########################################################################
//? ###########################################################################

Token* convertToTokens(char* str) {
    DOT
    // Token* token = token_init(FROM_STATE_START, NULL);
    // state_start(str, token);
    DOT

    Token *token = separate(str);
    detTypes(token);



    // token_printToFile(token, stdout);
DOT
    // detTypes(token);
    DOT
    token_printToFile(token, stdout);

    return token;
}
