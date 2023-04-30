#include "divideString.h"
#include "stringFile.h"
#include <malloc.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define DOT printf("\033[91mDOT: SOMEONE CALL ME FROM LINE: %d\n\033[39m", __LINE__);
//? ###########################################################################
//? ###########################################################################

#pragma region
// SimpleList* divideString_divideString(char* str, char* sep) {

//     SimpleList* head = simpleList_init(0);
//     SimpleList* curl = head;
//     DOT
//     StringFile* strFile = stringFile_init(str);
//     stringFile_dump(strFile);
//     DOT
//     // StringFile* sepFile = stringFile_init(sep);

//     char* buf = (char*) malloc(sizeof(char)*strFile->length);
//     size_t cap = 0;

//     size_t pos = 0;

//     while (1) {

//         size_t fill = 0;
//         for (; fill < strFile->length; ++fill) {
//             if (str[pos+fill] != sep[fill]) {
//                 break;
//             }
//         }
//         if (fill == strlen(sep)) {
//             char* newStr = (char*) malloc(sizeof(char)*cap);
//             strncpy(newStr, buf, cap);
//             curl->pointer = (unsigned long) newStr;
//             curl = simpleList_append(curl, 0);
//             pos += cap + fill;
//             cap = 0;
//         }
//         for (; fill > 0; --fill);

//         if (str[pos++] == '\0') {
//             break;
//         }

//         buf[cap] = str[pos++];

//     }

//     return head;

// }
#pragma endregion

#pragma region
// SimpleList* divideString_divideString(char* str, char* sep) {

//     printf("str: %s\n", str);
//     printf("sep: %s\n", sep);

//     SimpleList* head = simpleList_init(0);
//     SimpleList* curl = head;

//     for(int q = 0; q < 10; ++q) {
//         simpleList_append(head, q);
//     }

//     size_t cap    = 0;
//     size_t len    = strlen(str);
//     size_t sepLen = strlen(sep);

//     char* buf = (char*) malloc(sizeof(char)*len);

//     for (size_t q = 0; q < len; ++q) {
//         // printf("%d\n", q);

//         printf("q: %lu, str[%c]\n", q, str[q]);
//         printf("buf: |%.*s|\n", (int)cap, buf);

//         size_t w = 0;
//         while (str[q+w] == sep[w]) {
//             printf("str[q+w] = %c\n", str[q+w]);
//             printf("sep[w]   = %c\n", sep[w]);
//             // printf("q: %lu, str[%c]\n", q, str[q]);
//             printf("%d", (int)w);
//             ++w;
//         }
//         printf("\n");
//         if (w == sepLen || str[q] == '(') {

//             char* newStr = (char*) malloc(sizeof(char)*(cap+1));
//             printf("throw buf: %.*s\n", (int) cap, buf);
//             strncpy(newStr, buf, cap);
//             newStr[cap] = '\0';
//             printf("newStr->|%s|\n", newStr);
//             // DOT
//             simpleList_append(head, (size_t) newStr);
//             cap = 0;
//             q += w;

//         }

//         buf[cap++] = str[q];
        

//         printf("------------------------------------------------------------------------\n");
//     }

//     // ---



//     return head;
// }
#pragma endregion





SimpleList* divideString_divideString(char* str, char* sep) {

    // int state = START





    return 0;
}
