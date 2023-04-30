
// Fence -> Block(list of structurs) -> bak(smallest code element)



// bak <=> code element
enum BUK_TYPE {
    DEF       ,
    TYPE      ,
    NUMBER    ,
    CHARACTER ,
    LIST      ,
    OPERATOR  ,
    FUNCTION  ,
};


typedef struct fence_struct {



} Fence;


typedef struct bak_struct {

    int type;
    char* text;

} Bak;


int lexicalAnalyzer();
int structSeparator();
