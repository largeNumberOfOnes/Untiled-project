
#include "tree_structs.h"
#include "string.h"

int function_getArgNum(Function *func, char *arg) {

    for (int q = 0; q < func->argsCount; ++q) {
        if (!strcmp(func->argNames[q], arg)) {
            return q;
        }
    }

    return -1;
}

int function_isOper(Function * func) {

    return func->nutapp == FUNCTION_NUTAPP_OPERATOR;
}

int function_dump(Function *func, FILE *file) {

    fprintf(file, "name: %s\n", func->name);
    fprintf(file, "arg count: %d\n", func->argsCount);
    for (int q = 0; q < func->argsCount; ++q) {
        fprintf(file, "   -> |%s|\n", func->argNames[q]);
    }
    fprintf(file, "priority: %d\n", func->priority);
    fprintf(file, "nutapp: %d\n", func->nutapp);
    fprintf(file, "type string: %s\n", func->typetreeString);

    return 0;
}
