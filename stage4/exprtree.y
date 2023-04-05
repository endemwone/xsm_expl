%{
	#include <stdlib.h>
	#include <stdio.h>
	#include "exprtree.h"
	#include "exprtree.c"

    extern FILE* yyin;
	extern char* yytext;
	extern int yylineno;

    void yyerror(char const *s);
	int yylex(void);
%}

%union{
	struct AST_Node *node;
}

%type <node> program
%type <node> Declarations DeclList Decl VarList
%type <node> stmt_list stmt InputStmt OutputStmt AsgStmt IfStmt WhileStmt RepeatUntilStmt DoWhileStmt
%type <node> BREAK_ CONTINUE_
%type <node> ID_ NUM_ TEXT_ id
%type <node> INT_ STR_
%type <node> expr
%token PLUS_ MINUS_ MUL_ DIV_ MOD_ LT_ GT_ LE_ GE_ NE_ EQ_
%token BEGIN_ END_ READ_ WRITE_ IF_ THEN_ ELSE_ ENDIF_ WHILE_ DO_ ENDWHILE_ BREAK_ CONTINUE_ REPEAT_ UNTIL_
%token INT_ STR_ DECL_ ENDDECL_
%token ID_ NUM_ TEXT_ ID_Decl_
%left LT_ GT_ LE_ GE_ NE_ EQ_
%left PLUS_ MINUS_
%left MUL_ DIV_ MOD_

%%

program : BEGIN_ Declarations stmt_list END_ {
								$$ = $3;
								printf("Parsing Successful\n");

								ASTPrint($3);

								FILE *fp = fopen("output.xsm", "w");
								fprintf(fp, "0\n2056\n0\n0\n0\n0\n0\n0\n");
								fprintf(fp, "BRKP\n");
								fprintf(fp, "MOV SP, %d\n", SP);
								int p = codeGen($3, fp);
								fprintf(fp, "INT 10\n");
								
								exit(0);
							}
		| BEGIN_ Declarations END_ {
			printf("Empty Program\n");
			printf("Parsing Successful\n");
			exit(1);
		}


Declarations: DECL_ DeclList ENDDECL_	{GSTPrint();}
			| DECL_ ENDDECL_ {$$ = NULL;}

DeclList: DeclList Decl
		| Decl
		
Decl: INT_ VarList ';' {ASTChangeType($2, INTEGER);}
	| STR_ VarList ';' {ASTChangeType($2, STRING);}

VarList: VarList ',' ID_ {
			GSTInstall($3->varname, $3->type, 1, 0);
			$$ = makeNode(CONNECTOR, VOID, $1, NULL, $3, "VARLIST");
		}
		| ID_ {
			GSTInstall($1->varname, $1->type, 1, 0);
			$$ = $1;
		}
		| VarList ',' ID_ '[' NUM_ ']' {
			GSTInstall($3->varname, $3->type, $5->val, 1);
			$$ = makeNode(CONNECTOR, VOID, $1, NULL, $3, "VARLIST");
		}
		| ID_ '[' NUM_ ']' {
			GSTInstall($1->varname, $1->type, $3->val, 1);
			$$ = $1;
		}


stmt_list: stmt_list stmt ';' {$$ = makeNode(CONNECTOR, VOID, $1, NULL, $2, "CONNECTOR");}
	| stmt ';' {$$ = $1;}

stmt: InputStmt
    | OutputStmt
    | AsgStmt
	| IfStmt
	| WhileStmt
	| RepeatUntilStmt
	| DoWhileStmt
	| BREAK_
	| CONTINUE_

InputStmt: READ_ '(' id ')' { $$ = makeNode(READ, VOID, $3, NULL, NULL, "READ");}

OutputStmt: WRITE_ '(' expr ')' { $$ = makeNode(WRITE, VOID, $3, NULL, NULL, "WRITE");}

AsgStmt: id '=' expr { $$ = makeNode(OPERATOR, INTEGER, $1, NULL, $3, "="); }

IfStmt: IF_ '(' expr ')' THEN_ stmt_list ELSE_ stmt_list ENDIF_ { $$ = makeNode(IF, VOID, $3, $6, $8, "IF");}
    | IF_ '(' expr ')' THEN_ stmt_list ENDIF_ { $$ = makeNode(IF, VOID, $3, $6, NULL, "IF");}

WhileStmt: WHILE_ '(' expr ')' DO_ stmt_list ENDWHILE_ { $$ = makeNode(WHILE, VOID, $3, NULL, $6, "WHILE");}

RepeatUntilStmt: REPEAT_ stmt_list UNTIL_ '(' expr ')' { $$ = makeNode(REPEATUNTIL, VOID, $2, NULL, $5, "REPEAT");}

DoWhileStmt: DO_ stmt_list WHILE_ '(' expr ')' { $$ = makeNode(DOWHILE, VOID, $2, NULL, $5, "DOWHILE");}


expr : expr PLUS_ expr		{$$ = makeNode(OPERATOR, INTEGER, $1, NULL, $3, "+");}
	| expr MINUS_ expr		{$$ = makeNode(OPERATOR, INTEGER, $1, NULL, $3, "-");}
	| expr MUL_ expr		{$$ = makeNode(OPERATOR, INTEGER, $1, NULL, $3, "*");}
	| expr DIV_ expr		{$$ = makeNode(OPERATOR, INTEGER, $1, NULL, $3, "/");}
	| expr MOD_ expr		{$$ = makeNode(OPERATOR, INTEGER, $1, NULL, $3, "%");}
	| expr LT_ expr			{$$ = makeNode(OPERATOR, BOOLEAN, $1, NULL, $3, "<");}
	| expr GT_ expr			{$$ = makeNode(OPERATOR, BOOLEAN, $1, NULL, $3, ">");}
	| expr LE_ expr			{$$ = makeNode(OPERATOR, BOOLEAN, $1, NULL, $3, "<=");}
	| expr GE_ expr			{$$ = makeNode(OPERATOR, BOOLEAN, $1, NULL, $3, ">=");}
	| expr NE_ expr			{$$ = makeNode(OPERATOR, BOOLEAN, $1, NULL, $3, "!=");}
	| expr EQ_ expr			{$$ = makeNode(OPERATOR, BOOLEAN, $1, NULL, $3, "==");}
	| '(' expr ')' 			{$$ = $2;}
	| id					{$$ = $1;}
	| NUM_					{$$ = $1;}
	| TEXT_					{$$ = $1;}


id:	ID_ {
		$$ = $1;

		// Error if variable not declared
		struct GST_Node *curr = GSTLookup($1->varname);
		if (curr == NULL) {
			printf("Variable \"%s\" not declared\n", $1->varname);
			exit(1);
		}
		$$->type = curr->type;
	}
	| ID_ '[' expr ']' {
		// Error if expr is boolean
		if ($3->type == BOOLEAN) {
			printf("Array index cannot be boolean\n");
			exit(1);
		}

		// Error if array not declared
		struct GST_Node *curr = GSTLookup($1->varname);
		if (curr == NULL) {
			printf("Array \"%s\" not declared\n", $1->varname);
			exit(1);
		}		

		// Error if not array
		if (!curr->isArray) {
			printf("\"%s\" is not an array\n", $1->varname);
			exit(1);
		}

		$1->type = curr->type;
		$$ = makeArrayLeafNode($1->varname, $3, "ARRAY");
	}

%%

void yyerror(char const *s)
{
    printf("yyerror | Line: %d\n%s: %s\n", yylineno, s, yytext);
	exit(1);
}


int main(int argc, char *argv[]) 
{
    yyin=fopen(argv[1],"r");
	yyparse();
	
	return 0;
}