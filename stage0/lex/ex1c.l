%{
    #include<stdio.h>
%}

%%

A[A-Za-z0-9][A-Za-z0-9] {printf("%s\n", yytext);}
. {}

%%

int yywrap()
{
    return 1;
}

int main()
{
    yylex();
}
