%{
/*
 * grammar.y
 *
 * Pascal grammar in Yacc format, based originally on BNF given
 * in "Standard Pascal -- User Reference Manual", by Doug Cooper.
 * This in turn is the BNF given by the ANSI and ISO Pascal standards,
 * and so, is PUBLIC DOMAIN. The grammar is for ISO Level 0 Pascal.
 * The grammar has been massaged somewhat to make it LALR, and added
 * the following extensions.
 *
 * constant expressions
 * otherwise statement in a case
 * productions to correctly match else's with if's
 * beginnings of a separate compilation facility
 */

 #include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include "node.h"
     /* Called by yyparse on error.  */
extern int lineCount;
     void yyerror (char const *s)
     {
        extern char *yytext;
        extern int lineCount;
        fprintf (stderr, "%s: at line %d symbol'%s'\n", s,lineCount,yytext);
     }
struct nodeType *root;
FILE* f_assembly;
char *classname;
 struct SymTable SymbolTable;
int noerror =0 ;
%}
%union 
{
    struct nodeType * node;
    char *string;
}

%token <node> ARRAY ASSIGNMENT COLON COMMA
%token <node> DOT DOTDOT ELSE END EQUAL FUNCTION DO
%token <node>  GE GT  IF LBRAC LE LPAREN LT MINUS NOT
%token <node>  notEQUAL NOTEQUAL OF PBEGIN PLUS PROCEDURE PROGRAM RBRAC
%token <node>  INTEGER REAL STRING RPAREN SEMICOLON SLASH STAR THEN COMMENTS VAR WHILE 
%token <node>  NUM_INT NUM_REAL NUM_STRING
%token <node> IDENTIFIER
%type <node> program optional_statements statement_list statement factor identifier_list declarations type standard_type subprogram_declarations subprogram_declaration subprogram_head arguments parameter_list optional_var compound_statement variable tail procedure_statement expression_list expression simple_expression term addop mulop relop

%left PLUS MINUS
%left MULTIPLY DIVIDE

%%
program : PROGRAM IDENTIFIER LPAREN identifier_list RPAREN SEMICOLON declarations subprogram_declarations compound_statement DOT
{ 
$$ = newNode(NODE_PROGRAM);
root = $$;
deleteNode($1);
classname = (char*)malloc(strlen($2->string));
strcpy(classname,$2->string);
 addChild($$,$2);
deleteNode($3);
 addChild($$,$4);
deleteNode($5);
  deleteNode($6);
 addChild($$,$7);
 addChild($$,$8);
 addChild($$,$9);
 deleteNode($10);
addChild($$,newNode(NODE_END));	
}
 ;
identifier_list : identifier_list COMMA IDENTIFIER
 {
 $$ = $1;
 deleteNode($2);
 addChild($$,$3);
 }
| IDENTIFIER
{
$$ = newNode(NODE_IDLIST);
 addChild($$,$1);
 }
 ;

declarations : declarations VAR identifier_list COLON type SEMICOLON
{
 $$ = $1;                  //$$ = newNode(NODE_DECLARATION);
$3->line = lineCount;
 deleteNode($2);
 addChild($$,$3);
 deleteNode($4);
 addChild($$,$5);
 deleteNode($6);
}	| 
{
 $$ = newNode(NODE_DECLARATION);
$$->line = lineCount;
 }
;

type : standard_type
	{
$$ = $1;				//$$ = newNode(NODE_TYPE);addChild($$,$1);
	}| ARRAY LBRAC NUM_INT DOTDOT NUM_INT RBRAC OF type
{
 $$ = newNode(NODE_TYPE_ARRAY);                //有改
 deleteNode($1);
 deleteNode($2);
 $3->nodeType = NODE_INT;
 addChild($$,$3);
 deleteNode($4);
 $5->nodeType = NODE_INT;
 addChild($$,$5);
 deleteNode($6);
 deleteNode($7);
 addChild($$,$8);
}
;

standard_type : INTEGER { 
$$ = $1;
 $$->nodeType = NODE_TYPE_INT; 
}| REAL {
$$ = $1;
 $$->nodeType = NODE_TYPE_REAL; }| STRING
{
$$ = newNode(NODE_STANDARDTYPE);
 $$ = $1;
 $$->nodeType = NODE_TYPE_STRING; };

 subprogram_declarations :
	subprogram_declarations subprogram_declaration SEMICOLON
{
$$ = $1;//$$ = newNode(NODE_SUBPROGRAM_DECLARTIONS); 
addChild($$,$2);
 deleteNode($3);}	| 
{ 
$$ = newNode(NODE_SUBPROGRAM_DECLARTIONS);
 }
;
subprogram_declaration :
	subprogram_head 
	declarations			
        compound_statement
{
$$ = newNode(NODE_SUBPROGRAM_DECLARTION);
addChild($$,$1);
addChild($$,$2);
addChild($$,$3);
}
;
subprogram_head : FUNCTION IDENTIFIER arguments COLON standard_type SEMICOLON
	{
	$$ = newNode(NODE_SUBPROGRAM_FUNC);     //original :HEAD
	deleteNode($1);
	addChild($$,$2);	
addChild($$,$3);
deleteNode($4);
addChild($$,$5);
 deleteNode($6); 
$$->line = lineCount;
	}| PROCEDURE IDENTIFIER arguments SEMICOLON
{
$$ = newNode(NODE_SUBPROGRAM_PROCEDURE);     //original :HEAD
	deleteNode($1);
	addChild($$,$2);	
addChild($$,$3);
deleteNode($4);
$$->line = lineCount;
}
;	
arguments : LPAREN parameter_list RPAREN
	{
$$ = $2; 						//$$ = newNode(NODE_ARGUMENT);addChild($$,$2);
	deleteNode($1);	
deleteNode($3);	
	}|
{
 $$ = newNode(NODE_lambda);   	//$$ = newNode(NODE_ARGUMENT);
	}
;
parameter_list : optional_var identifier_list COLON type
	{
	$$ = newNode(NODE_PARAMETERLIST);
	deleteNode($1);
	addChild($$,$2);	
	deleteNode($3);
addChild($$,$4);
$$->line = lineCount;	
	}| optional_var identifier_list COLON type SEMICOLON parameter_list 
{
 $$ = newNode(NODE_PARAMETERLIST);
	deleteNode($1);
	addChild($$,$2);	
	deleteNode($3);
addChild($$,$4);	
deleteNode($5);
addChild($$,$6);
$$->line = lineCount;
}
;

optional_var   : VAR
     {
$$ = newNode(NODE_OPTIONALVAR);	
	deleteNode($1);		 
	}   |
{
 $$ = newNode(NODE_OPTIONALVAR);	
	}
;

compound_statement : PBEGIN
		       optional_statements
		       END
{ 
$$ = newNode(NODE_COMPOUNDSTATEMENT);	addChild($$,$2);//$$ = $2;
deleteNode($1);
$$->line = lineCount;	
}
;


optional_statements : statement_list
{
$$ = newNode(NODE_OPTIONAL_STATEMENT);
addChild($$,$1);}	|
{
 $$ = newNode(NODE_OPTIONAL_STATEMENT);
 }
;
statement_list : statement
	{ $$ = newNode(NODE_STATEMENT_LIST);
 addChild($$,$1);
 }| statement_list SEMICOLON statement
{
 $$ = $1;
 deleteNode($2);
 addChild($$,$3);
}
;

statement : variable ASSIGNMENT expression
{ $$ = newNode(NODE_ASSIGN_STATEMENT);
 addChild($$,$1);
 deleteNode($2);
 addChild($$,$3);
	$$->line = lineCount;
  }	| procedure_statement
{
 $$ = newNode(NODE_STATEMENT);
 addChild($$,$1);
}	| compound_statement
{
 $$ = newNode(NODE_STATEMENT);
 addChild($$,$1); 
}	| IF expression THEN statement ELSE statement
{
 $$ = newNode(NODE_IF_STATEMENT);
 deleteNode($1);
 addChild($$,$2);
 deleteNode($3);
 addChild($$,$4);
 deleteNode($5);
 addChild($$,$6); 
}	| WHILE expression DO statement
{
 $$ = newNode(NODE_WHILE_STATEMENT);
 deleteNode($1);
 addChild($$,$2);
 deleteNode($3);
 addChild($$,$4); 
}	|
{
 $$ = newNode(NODE_STATEMENT);
 }
;
variable : IDENTIFIER tail{
//$$ = newNode(NODE_VARIABLE);
$$ = $1;
	//addChild($$,$1);
addChild($$,$2); 
 $1->nodeType = NODE_SYM_REF;        //改
$1->line = lineCount;
}
;
tail     : LBRAC expression RBRAC tail
{
$$ = newNode(NODE_TAIL);	
	deleteNode($1);
addChild($$,$2); 	
	deleteNode($3);
addChild($$,$4);
}	|
{
$$ = newNode(NODE_lambda);	
	}
;
procedure_statement : IDENTIFIER
{ 
$$ = $1;                               //change
//$$ = newNode(NODE_PROCEDURESTATEMENT);
	//addChild($$,$1);
	//$1->nodeType = NODE_SYM_REF;       //改
	$$->nodeType = NODE_SYM_REF;
	//$1->line = lineCount;
	$$->line = lineCount;
}	| IDENTIFIER LPAREN expression_list RPAREN
{
//$$ = newNode(NODE_PROCEDURESTATEMENT);
//	addChild($$,$1);
$$ = $1;
	deleteNode($2);
addChild($$,$3);
deleteNode($4);
$1->nodeType = NODE_SYM_REF;     //改
$1->line = lineCount;
 }
;
expression_list : expression
{
$$ = newNode(NODE_EXPRESSIONLIST);	
	addChild($$,$1);
}	| expression_list COMMA expression
{
$$ = $1;	
	deleteNode($2);
	addChild($$,$3); 
	}
;
expression : simple_expression
{
$$ = $1;					//newNode(NODE_EXPRESSION);	addChild($$,$1);	
 }	| simple_expression relop simple_expression
{
 $$ = newNode(NODE_OP);	
 $$->op =$2->op;
	addChild($$,$1);
	addChild($$,$3);
$$->line = lineCount;
}
;
simple_expression : term
{
$$ = $1;	
}	| simple_expression addop term
{
 $$ = newNode(NODE_OP);
	$$->op = $2->op;
	addChild($$,$1);
	addChild($$,$3);$$->line = lineCount;
}
;
term : factor
{ 
$$ = $1;
}	| term mulop factor
{
$$ = newNode(NODE_OP);
$$->op = $2->op;	
	addChild($$,$1);
	addChild($$,$3);$$->line = lineCount;	
}
;
factor : IDENTIFIER LPAREN expression_list RPAREN
{
$$ = newNode(NODE_VAR_PROC);
$$->line = lineCount;
 addChild($$,$1);
 deleteNode($2);
// addChild($$,$3);
addChild($1,$3);
 deleteNode($4);
 $1->nodeType = NODE_SYM_REF;  			//change
$1->line = lineCount;
}	| IDENTIFIER tail                 //array
{
 $$ = newNode(NODE_VAR_PROC);   //$$ = newNode(NODE_FACTOR);
$$->line = lineCount;
 addChild($$,$1);
 //addChild($$,$2);
addChild($1,$2); 
 $1->nodeType = NODE_SYM_REF;   		//change
$1->line = lineCount;
}	|NUM_INT
{
$$=$1;
$$->nodeType = NODE_INT; 
}	| NUM_REAL
{
$$ = $1;
 $$->nodeType = NODE_REAL;  
}	
	| NUM_STRING
{
$$ = $1;
 $$->nodeType = NODE_STRING;  
}
|LPAREN expression RPAREN
{
$$ = $2;
 deleteNode($1);  
 deleteNode($3);
}	| NOT factor
{
 deleteNode($1);
 $$ = $2; 
}   | addop NUM_REAL
{

$$ = $2;
 $$->nodeType = NODE_REAL; double a = $$->rValue;
	a-=2*a;
	$$->rValue = a;    
}
	| addop NUM_INT
{
$$ = $2;
 $$->nodeType = NODE_INT;
	int a = $$->iValue;
	a-=2*a;
	$$->iValue = a;    
}
;

addop : PLUS { 
$$ = $1;
}| MINUS
{
$$ = $1;}
;

mulop : STAR { 
$$ = $1;}| SLASH
{
$$ =$1;}
;

relop : EQUAL
{
$$ = $1;} | notEQUAL
{
$$ = $1;} | LT
{
$$ = $1;} | GT
{
$$ = $1;} | LE
{ 
$$ = $1;
	} | GE
{
$$ = $1;
	} |NOTEQUAL
{
$$ = $1;	
}
 ;

%%
int main(int argc, char** argv) {
    int res;
	
	
    fprintf(stderr, "open file.\n");
    if(argc>1 && freopen(argv[1],"r",stdin)==NULL){
        exit(1);
    }
	
     fprintf(stderr, "call yyparse\n");
    res = yyparse();
    fprintf(stderr, "after call yyparse, res = %d.\n", res);
    if (res==0)
        fprintf(stderr, "SUCCESS\n");
    else
        fprintf(stderr, "ERROR\n");
	//printf("Tree:\n");
	//printTree(root,0);
	char* filename;
	filename = (char*)malloc(strlen(classname));
	strncpy(filename,argv[1],strlen(argv[1])-2);
	strcat(filename,".j");
	
	f_assembly=fopen(filename,"w");
	semanticCheck(root);
	
	if(noerror==0){
	codegenerate(root);
	printf("code generate done!\n");
	}
	free(classname);
	return 0;
}

int yywrap() {
  fprintf(stderr, "end.\n"); 
  //fclose(yyin);
}
