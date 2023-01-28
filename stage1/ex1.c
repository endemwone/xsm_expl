struct tnode *makeLeafNode(int n)
{
    struct tnode *temp;
    temp = (struct tnode *)malloc(sizeof(struct tnode));
    temp->op = NULL;
    temp->val = n;
    temp->left = temp->right = NULL;
    return temp;
}

struct tnode *makeOperatorNode(char c, struct tnode *l, struct tnode *r)
{
    struct tnode *temp;
    temp = (struct tnode *)malloc(sizeof(struct tnode));
    temp->op = (char *)malloc(sizeof(char));
    *(temp->op) = c;
    temp->val = -1;
    temp->left = l;
    temp->right = r;
    return temp;
}

void preorder(struct tnode *t)
{
    if (t != NULL)
    {
        if (t->op == NULL)
            printf("%d ", t->val);
        else
            printf("%c ", *(t->op));
        preorder(t->left);
        preorder(t->right);
    }
}

void postorder(struct tnode *t)
{
    if (t != NULL)
    {
        postorder(t->left);
        postorder(t->right);
        if (t->op == NULL)
            printf("%d ", t->val);
        else
            printf("%c ", *(t->op));
    }
}