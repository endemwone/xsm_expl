int free_reg = -1;

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

int getReg()
{
    free_reg++;
    return free_reg;
}

void freeReg()
{
    free_reg--;
}

void callwrite(FILE *fptr, int res_reg)
{
    int reg = getReg();

    fprintf(fptr, "MOV SP, 4095\n");
    fprintf(fptr, "MOV R%d, \"Write\"\n", reg);
    fprintf(fptr, "PUSH R%d\n", reg);
    fprintf(fptr, "MOV R%d, -2\n", reg);
    fprintf(fptr, "PUSH R%d\n", reg);
    fprintf(fptr, "PUSH R%d\n", res_reg);
    fprintf(fptr, "PUSH R%d\n", reg);
    fprintf(fptr, "PUSH R%d\n", reg);

    freeReg();

    fprintf(fptr, "CALL 0\n");

    fprintf(fptr, "POP R0\n");
    fprintf(fptr, "POP R1\n");
    fprintf(fptr, "POP R1\n");
    fprintf(fptr, "POP R1\n");
    fprintf(fptr, "POP R1\n");
}

int codeGen(struct tnode *t, FILE *fptr)
{
    if (t->op == NULL)
    {
        int reg = getReg();
        fprintf(fptr, "MOV R%d, %d\n", reg, t->val);
        return reg;
    }

    else
    {
        int left = codeGen(t->left, fptr);
        int right = codeGen(t->right, fptr);

        switch (*(t->op))
        {
        case '+':
            fprintf(fptr, "ADD R%d, R%d\n", left, right);
            break;

        case '-':
            fprintf(fptr, "SUB R%d, R%d\n", left, right);
            break;

        case '*':
            fprintf(fptr, "MUL R%d, R%d\n", left, right);
            break;

        case '/':
            fprintf(fptr, "DIV R%d, R%d\n", left, right);
            break;
        }

        freeReg();

        return left;
    }
}