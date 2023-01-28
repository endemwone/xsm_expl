typedef struct tnode
{
    int val;                    // value of the expression tree
    char *op;                   // indicates the opertor
    struct tnode *left, *right; // left and right branches
} tnode;

/*Make a leaf tnode and set the value of val field*/
struct tnode *makeLeafNode(int n);

/*Make a tnode with opertor, left and right branches set*/
struct tnode *makeOperatorNode(char c, struct tnode *l, struct tnode *r);

/*To display the prefix notation from an expression tree*/
void preorder(struct tnode *t);

/* To display the postfix notation from an expression tree*/
void postorder(struct tnode *t);
