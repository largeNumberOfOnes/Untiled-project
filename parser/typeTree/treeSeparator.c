#include <errno.h>
// #include <stddef.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include <stdlib.h>

#include "treeElements.h"
#include "treeSeparator.h"

#include "../../globalDefinitions.h"

TypeTree* treeSeparator_(char* str);


TypeTree* treeSeparator(char* str) {

    return treeSeparator(str);

}

// enum {
//     SEPARATOR,
//     CREATE,
//     START,
//     ERROR,
//     TYPE,
//     END,
//     DEF_CANE,
//     DEF_ARRAY,
//     DEF_STRUCT,
// };

enum {
    SUCCESS,
    UNCORRECT_TYPE,
    SEPARATOR_EXPECTED,
    NO_SUCH_CONSTRUCTOR,
    NO_CLOSE_BRACKET,
    WRONG_BRACKETS_SEQUENCE,
};

enum {
    START,
    TYPE,
    SEPARATOR,
    CONSTRUCTOR,
    INIT,
    ERROR,
    END,
};


static TypeTree* constructor_struct(char* str) {


    return 0;
}

static TypeTree* constructor_array(char* str) {
    printf("arr: |%s|\n", str);
    TypeTree* tree = treeSeparator_(str);
    if (!tree) {
        errout("Error in type of Array in [%s]", str);
    }
    return tree;
}

static TypeTree* constructor_cane(char* str) {


    return 0;
}

char* separateBrackets(char* str) {

    char* p = str + 1;
    int opencount = 1;
    int closcount = 0;
    while (1) {
        if (*p == ')') {
            if (opencount == closcount + 1) {
                break;
                // ++closcount;
            } else if (opencount <= closcount) {
                errout("Wrong brackets sequence in [%.*s]\n", (int)(p-str), str);
                return NULL;
            } else {
                ++closcount;
            }
        }
        if (*p == '(') {
            ++opencount;
        }
        if (*p == '\0') {
            errout("No close bracket in [%.*s]\n", (int)(p-str), str);
            return NULL;
        }
        ++p;
    }

    ++str;
    --p;
    char* newstr = (char*) malloc(sizeof(char)*(p-str + 1));
    strncpy(newstr, str, p-str+1);
    // printf("from constructor str: |%.*s|\n", (int)(p-str), str);
    newstr[p-str + 1] = '\0';
    // printf("ssss: |%s|", newstr);

    return newstr;
}

int constructor(TypeTree* tree, char** s) {
    DOT
    
    char* str = *s;
    printf("from constructor str: |%.10s|\n", str);
    // exit(0); 
    // return 0;
    char* newstr = separateBrackets(str);  
    DOT
    if (!newstr) {
        return ERROR;
    }
    *s = *s + strlen(newstr) + 2;
    printf("tst: |%.10s|\n", *s);

    if (tree->entity == UNDEF_ENTITY) {
        errout("No constructor for UNDEF ENTITY\n");
        return NO_SUCH_CONSTRUCTOR;
    }
    if (tree->entity == STRUCT_ENTITY) {
        tree->type = (size_t)constructor_struct(newstr);
        return 0;
    }
    if (tree->entity == ARRAY_ENTITY) {
        tree->type = (size_t)constructor_array(newstr);
        return 0;
    }
    if (tree->entity == CANE_ENTITY) {
        tree->type = (size_t)constructor_cane(newstr);
        return 0;
    }

    return -12;
}

static int state_TYPE(TypeTree* tree, char** s) {

    char* str = *s;
    char* p = str;

    if ('A' <= *p && *p <= 'Z') {
        ++p;
    } else if (*p == '\0') {
        errout("No type for constructor [%.5s]\n", p-5);
        return ERROR;
        // typeTree_dstr(tree); 
        // return END;
    } else {
        errout("Type should start with capital symbol in [%.1s]", str);
    }

    while (1) {
        if (('a' <= *p && *p <= 'z') ||
            ('0' <= *p && *p <= '9') ||
            (*p == '_')
        ) {
                ++p;
        } else {
            *s = p;
            tree->entity = typeTree_detEntity(str, p-str);

            if (tree->entity == UNDEF_ENTITY) {
                errout("Undefined type [%.*s]\n", (int)(p-str), str);
                return ERROR;
            }
            if (tree->entity == SIMPLE_ENTITY) {
                tree->type = typeTree_detType(str, p-str);
                return SEPARATOR;
            } else {
                if (*p == '(') {
                    *s = p;
                    DOT
                    constructor(tree, s);
                    DOT
                    return SEPARATOR;
                } else {
                    errout("Expected '(' in type constructor, but given [%.5s]\n", p);
                    return ERROR;
                }
            }

        }
    }

    return 0;
}

static int state_separator(char** s) {

    char* p = *s;
    while (1) {
        if (*p == ' ') {
            ++p;
        } else if (*p == '-' && *(p+1) == '>') {
            p += 2;
            while (*p == ' ') {
                ++p;
            }
            *s = p;
            return INIT;
        } else if (*p == '\0') {
            *s = p;
            DOT
            printf("--- |%9s|\n", p-4);
            return INIT;
        } else {
            errout("Separator expected, but given [%.5s]\n", *s);
            *s = p;
            return ERROR;
        }
    }

    return INIT;
}


TypeTree* treeSeparator_(char* str) {

    TypeTree* tree = typeTree_init(UNDEF_ENTITY, UNDEF_TYPE);



    int state = TYPE;
    int err = 0;

    char* p = str;

    while (1) {

        if (state == TYPE) {
            state = state_TYPE(tree, &str);
            DOT
        }

        if (state == INIT) {
            printf("--- |%9s|\n", str-4);
            // printf("INIT: Type is [%.*s]\n", (int)(p-str), str); // deb
            // exit(0);
            
            if (*str == '\0') {
                return tree;
            } else {
                tree->child = treeSeparator_(str);
                return tree;
            }
        }

        if (state == SEPARATOR) {
            typeTree_print_file(stdout, tree);
            printf("str: |%.10s|\n", str);
            printf("SEP\n");
            state = state_separator(&str);
            // exit(0);/
            // state = state_separator(tree, )
        }
        DOT
        if (state == ERROR) {
            printf("ERROR WHILE TYPE READING\n");
            printf("err: %d\n", err);
            exit(0);
            // while (1);
            return NULL;
        }

    }
    
    return NULL; // ... for beauty
}

TypeTree* typeTree_treeSeparator(char* str) {
    return treeSeparator_(str);
}



// 
//          +--> SEPARATOR -> 
//          |
//          |
//  START --+--> TYPE -+-------------> INIT -+->  -+-> ~NEW
//                 |                ^                   |
//                 |                |                   +-> END
//                 +-> CONSTRUCTOR -+
// 



