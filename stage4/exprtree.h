typedef enum Nodetype
{
    VARIABLE,
    ARRAY,
    CONSTANT,
    READ,
    WRITE,
    CONNECTOR,
    OPERATOR,
    WHILE,
    IF,
    BREAK,
    CONTINUE,
    REPEATUNTIL,
    DOWHILE,
} Nodetype;

typedef enum Type
{
    INTEGER,
    BOOLEAN,
    STRING,
    VOID,
} Type;

typedef struct AST_Node
{
    int val;                             // value of a number for NUM nodes.
    Type type;                           // type of variable
    char *varname;                       // name of a variable for ID nodes
    Nodetype nodetype;                   // type of node
    struct GST_Node *GSTentry;           // Pointer to Global Symbol Table entry for variables
    char *s;                             // string representation of the node
    struct AST_Node *left, *mid, *right; // left, middle and right branches
};

//-------------------AST-------------------
struct AST_Node *makeConstantLeafNode(Type, int, char *);
struct AST_Node *makeVariableLeafNode(char *, char *);
struct AST_Node *makeNode(Nodetype, Type, struct AST_Node *, struct AST_Node *, struct AST_Node *, char *);
struct AST_Node *makeArrayLeafNode(char *, struct AST_Node *, char *);
void ASTPrint(struct AST_Node *);

struct GST_Node
{
    char *name;            // name of the variable
    Type type;             // type of the variable
    int size;              // size of the type
    int isArray;           // 1 if array, 0 if not
    int binding;           // stores the static memory address allocated to the variable
    struct GST_Node *next; // pointer to next symbol table entry
};

//-------------------Symbol Table-------------------
struct GST_Node *GSTLookup(char *);
struct GST_Node *GSTInstall(char *, Type, int, int);
void GSTPrint();
int getSP();

//-------------------Code Generation-------------------
int codeGen(struct AST_Node *, FILE *);
