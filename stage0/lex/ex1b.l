%{
    #include<stdio.h>
    int valid_int = 0, valid_float = 0;
%}

%%

[0-9]+(\.[0-9]+) { valid_float += 1; }
[0-9]+ { valid_int += 1; }

%%

int yywrap()
{
    return 1;
}

int main()
{
    yylex();
    printf("Valid Integers: %d, Valid Floats: %d", valid_int, valid_float);
}