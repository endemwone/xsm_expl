%{
    #include<stdio.h>
%}

%%

[0-9]+  printf("number");

%%

int yywrap()
{
    if(yyin != stdin)
    {
        yyin = stdin;
        return 0;
    }
    else
        return 1;
}

int main()
{
    yyin = fopen("input_file.l","r");
    yylex();
    return 1;
}