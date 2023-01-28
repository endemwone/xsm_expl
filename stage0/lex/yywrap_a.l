%{
    #include<stdio.h>
%}

%%

[0-9]+  printf("number");

%%

int yywrap()
{
    FILE *file_ptr;
    char *file="input_file .l";
    file_ptr = fopen("input_file.l","r");
    if(yyin == stdin)
    {
        yyin = file_ptr;
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

