#include "stringFile.h"
#include <stdio.h>
#include <stdlib.h>
#define DOT printf("\033[91mDOT: SOMEONE CALL ME FROM LINE: %d\n\033[39m", __LINE__);
//? ###########################################################################
//? ###########################################################################

StringFile* stringFile_init(char* str) {
    StringFile* stringFile = (StringFile*) malloc(sizeof(StringFile));
    stringFile->length = strlen(str);
    // stringFile->buf = (char*) malloc((stringFile->length+1)*sizeof(char));
    stringFile->buf = (char*) malloc(stringFile->length*sizeof(char));
    stringFile->buf = strcpy(stringFile->buf, str);
    
DOT
    stringFile->position = 0;

    return stringFile;
}

StringFile* stringFile_dstr(StringFile* stringFile) {

    free(stringFile->buf);
    free(stringFile);

    return NULL;
}

int stringFile_dump(StringFile* stringFile) {

    printf("StringFile [%p] dump:\n", stringFile);

    printf("\t");
    
    return 0;
}

char stringFile_getc(StringFile* stringFile) {

    return stringFile->buf[stringFile->position++];
}

int stringFile_ungetc(StringFile* stringFile, char c) {

    if (!stringFile->position) {

        stringFile->position--;
        stringFile->buf[stringFile->position] = c;

    }

    return 0;
}

//? ###########################################################################
//? ###########################################################################
