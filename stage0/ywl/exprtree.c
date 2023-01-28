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

int evaluate(struct node *t)
{
    if (t->op == NULL)
        return t->val;

    switch (*(t->op))
    {
    case '+':
        return evaluate(t->left) + evaluate(t->right);
    case '-':
        return evaluate(t->left) - evaluate(t->right);
    case '*':
        return evaluate(t->left) * evaluate(t->right);
    case '/':
        return evaluate(t->left) / evaluate(t->right);
    }
}