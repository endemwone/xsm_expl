struct AST_Node *makeVariableLeafNode(Type type, char varname, char *s)
{
    struct AST_Node *new_node = (struct AST_Node *)malloc(sizeof(struct AST_Node));
    new_node->s = (char *)malloc(sizeof(char) * strlen(s));
    new_node->s = strdup(s);
    new_node->nodetype = VARIABLE;
    new_node->type = type;
    new_node->varname = (char *)malloc(sizeof(char));
    new_node->varname[0] = varname;
    new_node->left = (struct AST_Node *)NULL;
    new_node->right = (struct AST_Node *)NULL;
    new_node->mid = (struct AST_Node *)NULL;
    return new_node;
}

struct AST_Node *makeConstantLeafNode(Type type, int val, char *s)
{
    struct AST_Node *new_node = (struct AST_Node *)malloc(sizeof(struct AST_Node));
    new_node->s = (char *)malloc(sizeof(char) * strlen(s));
    new_node->s = strdup(s);
    new_node->nodetype = CONSTANT;
    new_node->type = type;
    new_node->val = val;
    new_node->left = (struct AST_Node *)NULL;
    new_node->right = (struct AST_Node *)NULL;
    new_node->mid = (struct AST_Node *)NULL;
    new_node->varname = (char *)NULL;
    return new_node;
}

struct AST_Node *makeNode(Nodetype node_type, Type type, struct AST_Node *l, struct AST_Node *m, struct AST_Node *r, char *s)
{
    struct AST_Node *new_node = (struct AST_Node *)malloc(sizeof(struct AST_Node));

    // Type checking
    if (node_type == OPERATOR && type == INTEGER)
    {
        if (l->type == BOOLEAN || r->type == BOOLEAN)
        {
            printf("Error: int mismatch 1\n");
            exit(1);
        }
    }
    if (node_type == WHILE || node_type == IF)
    {
        if (l->type == INTEGER)
        {
            printf("Error: int mismatch 2\n");
            exit(1);
        }
    }

    new_node->s = (char *)malloc(sizeof(char) * strlen(s));
    new_node->s = strdup(s);
    new_node->nodetype = node_type;
    new_node->type = type;
    new_node->left = l;
    new_node->mid = m;
    new_node->right = r;
    new_node->varname = (char *)NULL;
    return new_node;
}

void print_tree(struct AST_Node *root)
{
    if (root->nodetype == VARIABLE)
    {
        printf("%c", *(root->varname));
    }

    else if (root->nodetype == CONSTANT)
    {
        printf("%d", root->val);
    }

    else if (root->nodetype == READ)
    {
        printf("read(");
        print_tree(root->left);
        printf(")");
    }

    else if (root->nodetype == WRITE)
    {
        printf("write(");
        print_tree(root->left);
        printf(")");
    }

    else if (root->nodetype == STATEMENT)
    {
        print_tree(root->left);
        printf(";");
        print_tree(root->right);
    }

    else if (root->nodetype == OPERATOR)
    {
        if (root->type == BOOLEAN)
        {
            print_tree(root->left);
            printf("%s", root->s);
            print_tree(root->right);
        }
        else
        {
            print_tree(root->left);
            printf("%s", root->s);
            print_tree(root->right);
        }
    }

    else if (root->nodetype == WHILE)
    {
        printf("while(");
        print_tree(root->left);
        printf(")");
        print_tree(root->right);
    }

    else if (root->nodetype == IF)
    {
        printf("if(");
        print_tree(root->left);
        printf(")");
        print_tree(root->mid);
        printf("else");
        print_tree(root->right);
    }
}

int storage[26];

int evaluate(struct AST_Node *t)
{
    int p, q, addr;

    if (t->nodetype == VARIABLE)
    {
        addr = t->varname[0] - 'a';
        return (int)storage[addr];
    }

    else if (t->nodetype == CONSTANT)
    {
        return t->val;
    }

    else if (t->nodetype == READ)
    {
        addr = t->left->varname[0] - 'a';
        scanf("%d", &storage[addr]);

        return 0;
    }

    else if (t->nodetype == WRITE)
    {
        p = evaluate(t->left);
        printf("%d\n", p);
        return 0;
    }

    else if (t->nodetype == STATEMENT)
    {
        p = evaluate(t->left);
        q = evaluate(t->right);
        return 0;
    }

    else if (t->nodetype == OPERATOR)
    {
        p = evaluate(t->left);
        q = evaluate(t->right);

        if (t->type == BOOLEAN)
        {
            if (strcmp(t->s, ">") == 0)
            {
                if (p > q)
                    return 1;
                else
                    return 0;
            }
            else if (strcmp(t->s, "<") == 0)
            {
                if (p < q)
                    return 1;
                else
                    return 0;
            }
            else if (strcmp(t->s, ">=") == 0)
            {
                if (p >= q)
                    return 1;
                else
                    return 0;
            }
            else if (strcmp(t->s, "<=") == 0)
            {
                if (p <= q)
                    return 1;
                else
                    return 0;
            }
            else if (strcmp(t->s, "==") == 0)
            {
                if (p == q)
                    return 1;
                else
                    return 0;
            }
            else if (strcmp(t->s, "!=") == 0)
            {
                if (p != q)
                    return 1;
                else
                    return 0;
            }
        }

        if (strcmp(t->s, "=") == 0)
        {
            p = evaluate(t->right);
            addr = t->left->varname[0] - 'a';
            storage[addr] = p;
            return 0;
        }

        else
        {
            if (strcmp(t->s, "+") == 0)
                return p + q;
            else if (strcmp(t->s, "-") == 0)
                return p - q;
            else if (strcmp(t->s, "*") == 0)
                return p * q;
            else if (strcmp(t->s, "/") == 0)
                return p / q;
        }
    }

    else if (t->nodetype == WHILE)
    {
        while (evaluate(t->left))
            evaluate(t->right);
        return 0;
    }

    else if (t->nodetype == IF)
    {
        if (evaluate(t->left))
            evaluate(t->mid);
        else
            evaluate(t->right);
        return 0;
    }

    return 0;
}
