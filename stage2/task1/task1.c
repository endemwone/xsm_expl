struct AST_Node *makeVariableNode(int type, char varname, char *s)
{
    struct AST_Node *new_node = (struct AST_Node *)malloc(sizeof(struct AST_Node));
    new_node->s = (char *)malloc(sizeof(char) * strlen(s));
    new_node->s = strdup(s);
    new_node->nodetype = VARIABLE;
    new_node->type = type;
    new_node->varname = malloc(sizeof(char));
    *(new_node->varname) = varname;
    new_node->left = (struct AST_Node *)NULL;
    new_node->right = (struct AST_Node *)NULL;
    return new_node;
}

struct AST_Node *makeConstantNode(int type, int val, char *s)
{
    struct AST_Node *new_node = (struct AST_Node *)malloc(sizeof(struct AST_Node));
    new_node->s = (char *)malloc(sizeof(char) * strlen(s));
    new_node->s = strdup(s);
    new_node->nodetype = CONSTANT;
    new_node->type = type;
    new_node->val = val;
    new_node->left = (struct AST_Node *)NULL;
    new_node->right = (struct AST_Node *)NULL;
    new_node->varname = NULL;
    return new_node;
}

struct AST_Node *makeStmtNode(int type, struct AST_Node *left, struct AST_Node *right, char *s)
{
    struct AST_Node *new_node = (struct AST_Node *)malloc(sizeof(struct AST_Node));
    new_node->s = (char *)malloc(sizeof(char) * strlen(s));
    new_node->s = strdup(s);
    new_node->nodetype = STATEMENT;
    new_node->type = type;
    new_node->left = left;
    new_node->right = right;
    new_node->varname = NULL;
    return new_node;
}

struct AST_Node *makeExprNode(int type, char op, struct AST_Node *left, struct AST_Node *right, char *s)
{
    struct AST_Node *new_node = (struct AST_Node *)malloc(sizeof(struct AST_Node));
    new_node->s = (char *)malloc(sizeof(char) * strlen(s));
    new_node->s = strdup(s);
    new_node->nodetype = EXPRESSION;
    new_node->type = type;
    new_node->op = op;
    new_node->left = left;
    new_node->right = right;
    new_node->varname = NULL;
    return new_node;
}

void print_tree(struct AST_Node *root)
{
    if (root)
    {
        if (root->left != NULL)
            print_tree(root->left);
        printf("%s ", root->s);
        if (root->right != NULL)
            print_tree(root->right);
    }
}