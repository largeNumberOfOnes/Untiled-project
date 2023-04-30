#include <stdio.h>
#include <malloc.h>
#include <string.h>

//? ###########################################################################
//? ###########################################################################

char* preprocessor(char* filePath) {

    FILE* file = fopen(filePath, "r");

    if (!file) {
        perror("Error while opening file");
        fclose(file); file = NULL;
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long int len = ftell(file) + 1;
    if (len == -1) {
        perror("Error while counting symbols");
        fclose(file); file = NULL;
        return NULL;
    }

    fseek(file, 0, SEEK_SET);

    char* buf = (char*) malloc(sizeof(char)*len);
    char* str = (char*) malloc(sizeof(char)*len);
    char* curbufpos = buf;

    while (1) {
        if (!fgets(str, len, file)) break;
        size_t l = strlen(str);
        if (!(l >= 2 && *str == '/' && *(str+1) == '/')) {
            strcpy(curbufpos, str);
            curbufpos += l;
        }
    }

    fclose(file); file = NULL;
    free(str);

    return buf;
}
