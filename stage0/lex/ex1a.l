%{
    #include<stdio.h>
    int lc, wc, cc;
%}

%%

\n { lc++; cc++; }
[^ \n\t]+ { wc++; cc += yyleng; }
. { cc++;}

%%

int yywrap()
{
    return 1;
}

int main()
{
    yylex();
    printf("Lines: %d\nWords: %d\nCharacters: %d", lc, wc, cc);
}

