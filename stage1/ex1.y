%{
 #include <stdlib.h>
 #include <stdio.h>
 #include "ex1.h"
 #include "ex1.c"
 int yylex(void);
%}

%union{
    struct tnode *no;
}

%type <no> expr NUM program END
%token NUM PLUS MINUS MUL DIV END
%left PLUS MINUS
%left MUL DIV

%%

program : expr END {
    $$ = $2;
    printf("Prefix expression is: ");
    preorder($1);
    printf("\n");

    printf("Postfix expression is: ");
    postorder($1);
    printf("\n");

    exit(1);
};

expr : expr PLUS expr  {$$ = makeOperatorNode('+',$1,$3);}
    | '(' expr ')'  {$$ = $2;}
    | NUM   {$$ = $1;}
    ;

%%

yyerror(char const *s)
{
    printf("yyerror %s",s);
}


int main(void) {
    yyparse();

    return 0;
}
