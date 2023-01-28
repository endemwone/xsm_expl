int freereg_no=-1;

struct tnode* makeLeafNode(int n)
{
    struct tnode *temp;
    temp = (struct tnode*)malloc(sizeof(struct tnode));
    temp->op = NULL;
    temp->val = n;
    temp->left = NULL;
    temp->right = NULL;
    return temp;
}

struct tnode* makeOperatorNode(char c,struct tnode *l,struct tnode *r){
    struct tnode *temp;
    temp = (struct tnode*)malloc(sizeof(struct tnode));
    temp->op = malloc(sizeof(char));
    *(temp->op) = c;
    temp->left = l;
    temp->right = r;
    return temp;
}

int getReg()
{
    freereg_no++;
    return freereg_no;
} 

void freeReg()
{
    freereg_no--;
}

void callwrite(FILE *fptr,int resreg_no)
{
    int reg=getReg();

    fprintf(fptr,"MOV SP, 4095\n");
    fprintf(fptr,"MOV R%d,\"Write\"\n",reg);
    fprintf(fptr,"PUSH R%d\n",reg);
    fprintf(fptr,"MOV R%d, -2\n",reg);
    fprintf(fptr,"PUSH R%d\n",reg);
    fprintf(fptr,"PUSH R%d\n",resreg_no);
    fprintf(fptr,"PUSH R%d\n",reg);
    fprintf(fptr,"PUSH R%d\n",reg);
    
    freeReg();
    
    fprintf(fptr,"CALL 0\n");

    fprintf(fptr,"POP R0\n");
    fprintf(fptr,"POP R1\n");
    fprintf(fptr,"POP R1\n");
    fprintf(fptr,"POP R1\n");
    fprintf(fptr,"POP R1\n");
}

int evaluate(struct tnode *t){
    if(t->op == NULL)
    {
        return t->val;
    }
    else{
        switch(*(t->op)){
            case '+' : return evaluate(t->left) + evaluate(t->right);
                       break;
            case '-' : return evaluate(t->left) - evaluate(t->right);
                       break;
            case '*' : return evaluate(t->left) * evaluate(t->right);
                       break;
            case '/' : return evaluate(t->left) / evaluate(t->right);
                       break;
        }
    }
}

int codeGen(struct tnode *t,FILE *fptr)
{
    if(t->op == NULL)           //Leaf Node
    {
          int p=getReg();
          fprintf(fptr,"MOV R%d,%d\n",p,t->val);
          return p;
    }

    else{

        int left=codeGen(t->left,fptr);
        int right=codeGen(t->right,fptr);

        switch(*(t->op))
        {
            case '+' : fprintf(fptr,"ADD R%d,R%d\n",left,right);
                       break;
            case '-' : fprintf(fptr,"SUB R%d,R%d\n",left,right);
                       break;
            case '*' : fprintf(fptr,"MUL R%d,R%d\n",left,right);
                       break;
            case '/' : fprintf(fptr,"DIV R%d,R%d\n",left,right);
                       break;
        }

        freeReg();

        if(left<right)
        {return left;}

        else
        {return right;}
    }
}