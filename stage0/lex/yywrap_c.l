%{
    #include<stdio.h>
    int flag = 0;
%}

%%

[0-9]+  printf("number");

%%

int yywrap()
{
    if(flag == 0)
    {
        flag = 1;
        return 0;
    }
    else
        return 1;
}

int main()
{
    yyin = stdin;
    yylex();
    return 1;
}