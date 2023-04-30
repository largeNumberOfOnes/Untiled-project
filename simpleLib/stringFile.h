#include <string.h>
#include <malloc.h>

#ifndef STRINGFILE_H
#define STRINGFILE_H

//? ###########################################################################
//? ###########################################################################

typedef struct stringFile_t {

    char* buf;

    size_t length;
    size_t position;

} StringFile;

//? ###########################################################################
//? ###########################################################################

StringFile* stringFile_init(char* str);
StringFile* stringFile_dstr(StringFile* stringFile);

char stringFile_getc(StringFile* stringFile);
int stringFile_ungetc(StringFile* stringFile, char c);

int stringFile_dump(StringFile* stringFile);

//? ###########################################################################
//? ###########################################################################

#endif
