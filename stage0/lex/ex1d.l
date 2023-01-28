%{
    #include<stdio.h>
%}

%%

"//".* {printf("Single Line Comment: %s\n", yytext);}
/\*[^*]*\*+(?:[^/*][^*]*\*+)*/ {printf("Multi Line Comment: %s\n", yytext);}
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
