
// TypeTree* treeSeparator_(char* str) {

//     TypeTree* tree = typeTree_init(0, 0);



//     int state = START;
//     int err = 0;

//     char* p = str;

//     while (1) {

//         if (state == START) {
//             if ('A' <= *p && *p <= 'Z') {
//                 state = TYPE;
//                 ++p;
//             } else if (*p == '\0') {
//                 free(tree);
//                 tree = NULL;
//                 state = END;
//                 continue;
//             } else { // ERROR
//                 fprintf(errout, "Type should start with capital symbol [%.*s]", (int)(p-str), str);
//                 err = UNCORRECT_TYPE;
//                 state = ERROR;
//                 continue;
//             } 
//         }
//         if (state == TYPE) {
//             if (('a' <= *p && *p <= 'z') || 
//                 ('0' <= *p && *p <= '9') ||
//                 (*p == '_')
//             ) {
//                 ++p;
//                 state = TYPE;
//                 continue;
//             } else if (*p == '-') {
//                 state = SEPARATOR;
//                 continue;
//             } else if (*p == '(') {
//                 if (!strcmp(str, "Struct")) {
//                     state = DEF_STRUCT;
//                     continue;
//                 } else if (!strcmp(str, "Array" )) {
//                     state = DEF_ARRAY;
//                     continue;
//                 } else if (!strcmp(str, "Cane"  )) {
//                     state = DEF_CANE;
//                     continue;
//                 } else {
//                     fprintf(errout, "No such type constructor [%.*s]", (int)(p-str), str);
//                     state = ERROR;
//                     continue;
//                     // pass
//                 }
//             } else if (*p == ')') {
//                 // pass
//             } else if (*p == '\0') {
//                 // TypeTree* tree = typeTree_init();

//                 state = END;
//                 // return ;
//             } else { // ERROR
//                 // prevState = START;
//                 state = ERROR;
//                 continue;
//             } 
//         }
//         if (state == SEPARATOR) {
//             if (*(p+1) != '>') {
//                 fprintf(errout, "Separator expected");
//                 err = SEPARATOR_EXPECTED;
//                 state = ERROR;
//                 continue;
//             } else {
//                 tree->entity = typeTree_detEntity(str, p-str);
//                 if (tree->entity == UNDEF_ENTITY) {
//                     fprintf(errout, "No such type constructor [%.*s]", (int)(p-str), str);
//                     err = UNCORRECT_TYPE;
//                     state = err;
//                     continue;
//                 }
//                 if (tree->entity == SIMPLE_ENTITY) {
//                     tree->type = treeType_detType(str, p-str);
//                     if (tree->type == UNDEF_TYPE) {
//                         fprintf(errout, "No such type [%.*s]", (int)(p-str), str);
//                         err = UNCORRECT_TYPE;
//                         state = ERROR;
//                         continue;
//                     }
//                 }
//                 tree->child = treeSeparator_(p);
//                 state = END;
//                 continue;
//             }
//         }
//         if (state == DEF_ARRAY) {
//             tree->entity = ARRAY_ENTITY;
//             ++p;
//             tree->type = (size_t) treeSeparator_(p);
//             ++p;
//             tree->child = treeSeparator_(p);
//             state = END;
//             continue;
//         }
//         if (state == END) {
//             return tree;
//         }
//         if (state == ERROR) {

//         }

//     }
    


//     return tree;
// }
