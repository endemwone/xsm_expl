//---------------------------GST-----------------------------
struct GST_Node *Ghead = NULL;
int binding = 4096;
int SP;

struct GST_Node *GSTLookup(char *name)
{
    struct GST_Node *temp = Ghead;
    while (temp != NULL)
    {
        if (strcmp(temp->name, name) == 0)
            return temp;
        temp = temp->next;
    }
    return NULL;
}

struct GST_Node *GSTInstall(char *name, Type type, int size, int isArray)
{
    struct GST_Node *new_node = GSTLookup(name);
    if (new_node != NULL)
    {
        printf("Variable \"%s\" already declared\n", name);
        exit(1);
    }
    new_node = (struct GST_Node *)malloc(sizeof(struct GST_Node));
    new_node->name = name;
    new_node->type = type;
    new_node->size = size;
    new_node->isArray = isArray;
    new_node->binding = binding;
    SP = binding + size;
    binding += size;
    if (Ghead == NULL)
    {
        Ghead = new_node;
    }
    else
    {
        struct GST_Node *temp = Ghead;
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = new_node;
    }
    return Ghead;
}

void GSTChangeType(struct AST_Node *root, Type type)
{
    struct GST_Node *temp = GSTLookup(root->varname);
    if (temp == NULL)
    {
        printf("Variable \"%s\" not declared\n", root->varname);
        exit(1);
    }
    temp->type = type;
}

void GSTPrint()
{
    char *type;
    char *array;
    struct GST_Node *temp = Ghead;
    printf("Name\tType\tSize\tArray\tBinding\n");
    while (temp != NULL)
    {
        if (temp->type == 0)
        {
            type = (char *)malloc(sizeof(char) * 4);
            strcpy(type, "int");
        }
        else if (temp->type == 2)
        {
            type = (char *)malloc(sizeof(char) * 4);
            strcpy(type, "str");
        }

        if (temp->isArray == 0)
        {
            array = (char *)malloc(sizeof(char) * 3);
            strcpy(array, "no");
        }
        else if (temp->isArray == 1)
        {
            array = (char *)malloc(sizeof(char) * 4);
            strcpy(array, "yes");
        }

        printf("%s\t%s\t%d\t%s\t%d\n", temp->name, type, temp->size, array, temp->binding);
        temp = temp->next;
    }
}

int getSP()
{
    return SP;
}
//---------------------------GST-----------------------------

//---------------------------AST-----------------------------
struct AST_Node *makeVariableLeafNode(char *varname, char *s)
{
    struct AST_Node *new_node = (struct AST_Node *)malloc(sizeof(struct AST_Node));
    new_node->s = (char *)malloc(sizeof(char) * strlen(s));
    new_node->s = strdup(s);
    new_node->nodetype = VARIABLE;
    new_node->varname = (char *)malloc(sizeof(char) * strlen(varname));
    new_node->varname = strdup(varname);
    new_node->GSTentry = GSTLookup(varname);
    if (new_node->GSTentry)
        new_node->type = new_node->GSTentry->type;
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
    new_node->GSTentry = (struct GST_Node *)NULL;
    new_node->left = (struct AST_Node *)NULL;
    new_node->right = (struct AST_Node *)NULL;
    new_node->mid = (struct AST_Node *)NULL;
    new_node->varname = (char *)NULL;
    return new_node;
}

struct AST_Node *makeArrayLeafNode(char *varname, struct AST_Node *l, char *s)
{
    struct AST_Node *new_node = (struct AST_Node *)malloc(sizeof(struct AST_Node));
    new_node->s = (char *)malloc(sizeof(char) * strlen(s));
    new_node->s = strdup(s);
    new_node->nodetype = ARRAY;
    new_node->varname = (char *)malloc(sizeof(char) * strlen(varname));
    new_node->varname = strdup(varname);
    new_node->GSTentry = GSTLookup(varname);
    new_node->type = new_node->GSTentry->type;
    new_node->left = l;
    new_node->right = (struct AST_Node *)NULL;
    new_node->mid = (struct AST_Node *)NULL;
    return new_node;
}

struct AST_Node *makeNode(Nodetype node_type, Type type, struct AST_Node *l, struct AST_Node *m, struct AST_Node *r, char *s)
{
    struct AST_Node *new_node = (struct AST_Node *)malloc(sizeof(struct AST_Node));

    // Type checking
    if (node_type == OPERATOR && type == INTEGER)
    {
        if (l->GSTentry->type != INTEGER && l->GSTentry->type != INTEGER)
        {
            printf("Error: Type mismatch. \"Operator is not integer\".\n");
            exit(1);
        }
    }

    if (node_type == WHILE || node_type == IF)
    {
        if (l->type != BOOLEAN)
        {
            printf("Error: Type mismatch. \"Guard is not boolean\".\n");
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
    new_node->GSTentry = (struct GST_Node *)NULL;
    new_node->varname = (char *)NULL;
    return new_node;
}

struct AST_Node *ASTChangeType(struct AST_Node *root, Type type)
{
    if (root != NULL)
    {
        root->left = ASTChangeType(root->left, type);
        root->right = ASTChangeType(root->right, type);
        if (root->nodetype == VARIABLE)
        {
            GSTChangeType(root, type);
        }
        root = NULL;
        free(root);
    }
}

void ASTPrint(struct AST_Node *root)
{
    if (root->nodetype == VARIABLE)
    {
        printf("%s", root->varname);
    }

    else if (root->nodetype == CONSTANT)
    {
        if (root->type == INTEGER)
            printf("%d", root->val);
        else if (root->type == STRING)
            printf("%s", root->s);
    }

    else if (root->nodetype == READ)
    {
        printf("read(");
        ASTPrint(root->left);
        printf(")");
    }

    else if (root->nodetype == WRITE)
    {
        printf("write(");
        ASTPrint(root->left);
        printf(")");
    }

    else if (root->nodetype == CONNECTOR)
    {
        ASTPrint(root->left);
        printf(";\n");
        ASTPrint(root->right);
    }

    else if (root->nodetype == OPERATOR)
    {
        if (root->type == BOOLEAN)
        {
            ASTPrint(root->left);
            printf("%s", root->s);
            ASTPrint(root->right);
        }
        else
        {
            ASTPrint(root->left);
            printf("%s", root->s);
            ASTPrint(root->right);
        }
    }

    else if (root->nodetype == WHILE)
    {
        printf("while(");
        ASTPrint(root->left);
        printf(")\n");
        ASTPrint(root->right);
    }

    else if (root->nodetype == IF)
    {
        printf("if(");
        ASTPrint(root->left);
        printf(")\n");
        ASTPrint(root->mid);
        if (root->right != NULL)
        {
            printf("else\n");
            ASTPrint(root->right);
        }
    }

    else if (root->nodetype == ARRAY)
    {
        printf("%s[", root->varname);
        ASTPrint(root->left);
        printf("]");
    }
}
//---------------------------AST-----------------------------

//---------------------------Register Allocation-----------------------------
int free_reg = -1;

int getReg()
{
    free_reg++;
    return free_reg;
}

void freeReg()
{
    free_reg--;
}
//---------------------------Register Allocation-----------------------------

//---------------------------Code Generation-----------------------------
int numWhile = 0;
int codeGen(struct AST_Node *t, FILE *target_file) // returns the register number
{
    int p, q, r, s, u, v, addr;

    if (t->nodetype == CONSTANT)
    {
        if (t->type == INTEGER)
        {
            p = getReg();
            fprintf(target_file, "MOV R%d, %d\n", p, t->val);
            return p;
        }
        else if (t->type == STRING)
        {
            p = getReg();
            fprintf(target_file, "MOV R%d, %s\n", p, t->s);
            return p;
        }
    }

    if (t->nodetype == VARIABLE)
    {
        p = getReg();
        addr = getAddr(t);
        fprintf(target_file, "MOV R%d, [%d]\n", p, addr);
        return p;
    }

    if (t->nodetype == READ)
    {
        p = getReg();
        q = getReg();
        addr = getAddr(t->left);
        if (t->left->nodetype == ARRAY)
        {
            freeReg();
            q = codeGen(t->left->left, target_file);
            fprintf(target_file, "MOV R%d, %d\n", p, addr);
            fprintf(target_file, "ADD R%d, R%d\n", p, q);
        }
        else
        {
            fprintf(target_file, "MOV R%d, %d\n", p, addr);
        }
        fprintf(target_file, "MOV R%d, \"Read\"\n", q);
        fprintf(target_file, "PUSH R%d\n", q);
        fprintf(target_file, "MOV R%d, -1\n", q);
        fprintf(target_file, "PUSH R%d\n", q);
        fprintf(target_file, "PUSH R%d\n", p);
        fprintf(target_file, "PUSH R%d\n", q);
        fprintf(target_file, "PUSH R%d\n", q);
        fprintf(target_file, "CALL 0\n");
        fprintf(target_file, "POP R%d\n", q);
        fprintf(target_file, "POP R%d\n", q);
        fprintf(target_file, "POP R%d\n", q);
        fprintf(target_file, "POP R%d\n", q);
        fprintf(target_file, "POP R%d\n", q);
        freeReg();
        freeReg();
        return -1;
    }

    if (t->nodetype == WRITE)
    {
        p = getReg();
        if (t->left->nodetype == ARRAY)
        {
            addr = getAddr(t->left);
            p = getReg();
            q = codeGen(t->left->left, target_file);
            fprintf(target_file, "MOV R%d, %d\n", p, addr);
            fprintf(target_file, "ADD R%d, R%d\n", p, q);
            fprintf(target_file, "MOV R%d, [R%d]\n", p, p);
            freeReg();
        }
        else
        {
            p = codeGen(t->left, target_file);
        }
        q = getReg();
        fprintf(target_file, "MOV R%d, \"Write\"\n", q);
        fprintf(target_file, "PUSH R%d\n", q);
        fprintf(target_file, "MOV R%d, -2\n", q);
        fprintf(target_file, "PUSH R%d\n", q);
        fprintf(target_file, "PUSH R%d\n", p);
        fprintf(target_file, "PUSH R%d\n", q);
        fprintf(target_file, "PUSH R%d\n", q);
        fprintf(target_file, "CALL 0\n");
        fprintf(target_file, "POP R%d\n", q);
        fprintf(target_file, "POP R%d\n", q);
        fprintf(target_file, "POP R%d\n", q);
        fprintf(target_file, "POP R%d\n", q);
        fprintf(target_file, "POP R%d\n", q);
        freeReg();
        freeReg();
        return -1;
    }

    if (t->nodetype == CONNECTOR)
    {
        codeGen(t->left, target_file);
        codeGen(t->right, target_file);
        return -1;
    }

    if (t->nodetype == OPERATOR)
    {
        if (t->type == BOOLEAN)
        {
            p = codeGen(t->left, target_file);
            q = codeGen(t->right, target_file);
            if (strcmp(t->s, ">") == 0)
            {
                fprintf(target_file, "GT R%d, R%d\n", p, q);
            }
            else if (strcmp(t->s, "<") == 0)
            {
                fprintf(target_file, "LT R%d, R%d\n", p, q);
            }
            else if (strcmp(t->s, ">=") == 0)
            {
                fprintf(target_file, "GE R%d, R%d\n", p, q);
            }
            else if (strcmp(t->s, "<=") == 0)
            {
                fprintf(target_file, "LE R%d, R%d\n", p, q);
            }
            else if (strcmp(t->s, "==") == 0)
            {
                fprintf(target_file, "EQ R%d, R%d\n", p, q);
            }
            else if (strcmp(t->s, "!=") == 0)
            {
                fprintf(target_file, "NE R%d, R%d\n", p, q);
            }
            freeReg();
            return p;
        }

        if (t->s[0] == '=')
        {
            p = codeGen(t->right, target_file);
            addr = getAddr(t->left);

            if (t->left->nodetype == ARRAY)
            {
                int p2 = getReg();
                q = codeGen(t->left->left, target_file);
                fprintf(target_file, "MOV R%d, %d\n", p2, addr);
                fprintf(target_file, "ADD R%d, R%d\n", p2, q);
                fprintf(target_file, "MOV [R%d], R%d\n", p2, p);
                freeReg();
                freeReg();
            }
            else
            {
                fprintf(target_file, "MOV [%d], R%d\n", addr, p);
            }
            freeReg();
            return -1;
        }

        else
        {
            p = codeGen(t->left, target_file);
            q = codeGen(t->right, target_file);
            switch (t->s[0])
            {
            case '+':
                fprintf(target_file, "ADD R%d, R%d\n", p, q);
                freeReg();
                return p;
            case '-':
                fprintf(target_file, "SUB R%d, R%d\n", p, q);
                freeReg();
                return p;
            case '*':
                fprintf(target_file, "MUL R%d, R%d\n", p, q);
                freeReg();
                return p;
            case '/':
                fprintf(target_file, "DIV R%d, R%d\n", p, q);
                freeReg();
                return p;
            case '%':
                fprintf(target_file, "MOD R%d, R%d\n", p, q);
                freeReg();
                return p;
            }
        }
    }

    else if (t->nodetype == WHILE)
    {
        u = getLabel();
        v = getLabel();
        fprintf(target_file, "L%d:\n", u);
        p = codeGen(t->left, target_file);
        fprintf(target_file, "JZ R%d, L%d\n", p, v);
        freeReg();
        numWhile++;
        p = codeGen(t->right, target_file);
        numWhile--;
        fprintf(target_file, "JMP L%d\n", u);
        fprintf(target_file, "L%d:\n", v);
        return p;
    }

    else if (t->nodetype == IF)
    {
        s = getLabel();
        p = codeGen(t->left, target_file);
        fprintf(target_file, "JZ R%d, L%d\n", p, s);
        freeReg();
        p = codeGen(t->mid, target_file);
        if (t->right != NULL)
        {
            r = getLabel();
            fprintf(target_file, "JMP L%d\n", r);
            fprintf(target_file, "L%d:\n", s);
            p = codeGen(t->right, target_file);
            fprintf(target_file, "L%d:\n", r);
        }
        else
        {
            fprintf(target_file, "L%d:\n", s);
        }
        return p;
    }

    else if (t->nodetype == BREAK)
    {
        if (numWhile)
            fprintf(target_file, "JMP L%d\n", v);
    }

    else if (t->nodetype == CONTINUE)
    {
        if (numWhile)
            fprintf(target_file, "JMP L%d\n", u);
    }

    else if (t->nodetype == REPEATUNTIL)
    {
        u = getLabel();
        v = getLabel();
        fprintf(target_file, "L%d:\n", u);
        numWhile = 1;
        p = codeGen(t->left, target_file);
        numWhile = 0;
        p = codeGen(t->right, target_file);
        fprintf(target_file, "JZ R%d, L%d\n", p, u);
        fprintf(target_file, "L%d:\n", v);
        freeReg();
        return p;
    }

    else if (t->nodetype == DOWHILE)
    {
        u = getLabel();
        v = getLabel();
        fprintf(target_file, "L%d:\n", u);
        numWhile++;
        p = codeGen(t->left, target_file);
        numWhile--;
        p = codeGen(t->right, target_file);
        fprintf(target_file, "JNZ R%d, L%d\n", p, u);
        fprintf(target_file, "L%d:\n", v);
        freeReg();
        return p;
    }

    else if (t->nodetype == ARRAY)
    {
        addr = getAddr(t);
        p = codeGen(t->left, target_file);
        q = getReg();
        fprintf(target_file, "MOV R%d, %d\n", q, addr);
        fprintf(target_file, "ADD R%d, R%d\n", q, p);
        fprintf(target_file, "MOV R%d, [R%d]\n", p, q);
        freeReg();
        return p;
    }
}
//---------------------------Code Generation-----------------------------

//---------------------------Utility Functions-----------------------------
int getAddr(struct AST_Node *t)
{
    return t->GSTentry->binding;
}

int label = 0;

int getLabel()
{
    return label++;
}
//---------------------------Utility Functions-----------------------------
