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

/* Generate code for the expression tree */
int codeGen(struct tnode *t, FILE *fptr);

/* Call the write function */
void callwrite(FILE *fptr, int res_reg);