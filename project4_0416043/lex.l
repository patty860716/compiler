A [aA]
B [bB]
C [cC]
D [dD]
E [eE]
F [fF]
G [gG]
H [hH]
I [iI]
J [jJ]
K [kK]
L [lL]
M [mM]
N [nN]
O [oO]
P [pP]
Q [qQ]
R [rR]
S [sS]
T [tT]
U [uU]
V [vV]
W [wW]
X [xX]
Y [yY]
Z [zZ]
NQUOTE [^'&^\n]
%{
#include "y.tab.h"
#include "node.h"
struct nodeType* newTokenNode(int tokenType);
  int lineCount = 1;
%}

%%

{D}{O}                       {yylval.node = newTokenNode(DO);return(DO);};
{A}{R}{R}{A}{Y}              {yylval.node = newTokenNode(ARRAY);return(ARRAY);};
{E}{L}{S}{E}                 {yylval.node = newTokenNode(ELSE); return(ELSE);};
{E}{N}{D}                    {yylval.node = newTokenNode(END); return(END);};
{T}{H}{E}{N}                 {yylval.node = newTokenNode(THEN); return(THEN);};
{F}{U}{N}{C}{T}{I}{O}{N}     {yylval.node = newTokenNode(FUNCTION); return(FUNCTION);};
{I}{F}                       {yylval.node = newTokenNode(IF);return(IF);};
{N}{O}{T}                    {yylval.node = newTokenNode(NOT); yylval.node->op = OP_NOT;return(NOT);};
{O}{F}                       {yylval.node = newTokenNode(OF); return(OF); };
{B}{E}{G}{I}{N}              {yylval.node = newTokenNode(PBEGIN);return(PBEGIN);};
{P}{R}{O}{C}{E}{D}{U}{R}{E}  {yylval.node = newTokenNode(PROCEDURE);return(PROCEDURE);};
{P}{R}{O}{G}{R}{A}{M}        {yylval.node = newTokenNode(PROGRAM); return(PROGRAM);};
{V}{A}{R}                    {yylval.node = newTokenNode(VAR); return(VAR);};
{W}{H}{I}{L}{E}              {yylval.node = newTokenNode(WHILE); return(WHILE);};
{I}{N}{T}{E}{G}{E}{R}        {yylval.node = newTokenNode(INTEGER); return(INTEGER);};
{S}{T}{R}{I}{N}{G}           {yylval.node = newTokenNode(STRING); return(STRING);};
{R}{E}{A}{L}                 {yylval.node = newTokenNode(REAL); return(REAL);};
'({NQUOTE}|'')+'|\"[^\"&^\n]*\"  {yylval.node = newTokenNode(NUM_REAL);
    yylval.node->string = (char*)malloc(yyleng+1);
    strcpy(yylval.node->string, strdup(yytext));return(NUM_STRING);};
[0-9]+			{yylval.node = newTokenNode(NUM_INT);yylval.node->string = (char*)malloc(yyleng+1);strcpy(yylval.node->string, strdup(yytext));yylval.node->iValue = atoi(strdup(yytext));return(NUM_INT);}
[0-9]+[E]"+"[0-9]+|[0-9]+"."[0-9]+[E]"+"[0-9]+|[0-9]+"."[0-9]+[E]"+"[0-9]+"."[0-9]+|[0-9]+[E]"+"[0-9]+"."[0-9]+ {yylval.node = newTokenNode(NUM_REAL);yylval.node->string = (char*)malloc(yyleng+1);strcpy(yylval.node->string, strdup(yytext)); return(NUM_REAL);};
[0-9]+"."[0-9]+             {yylval.node = newTokenNode(NUM_REAL);yylval.node->rValue = atof(strdup(yytext));yylval.node->string = (char*)malloc(yyleng+1);strcpy(yylval.node->string, strdup(yytext)); return(NUM_REAL);};
([a-zA-Z])([a-zA-Z0-9])* {yylval.node = newTokenNode(IDENTIFIER);yylval.node->string = (char*)malloc(yyleng+1);
    strcpy(yylval.node->string, strdup(yytext)); return(IDENTIFIER);};
":="                         {yylval.node = newTokenNode(ASSIGNMENT); return(ASSIGNMENT);};
":"                          {yylval.node = newTokenNode(COLON); return(COLON);};
","                          {yylval.node = newTokenNode(COMMA); return(COMMA);};
"//"[^\n]+|"/*"[^*/]+"*/"			{};//return(COMMENTS);

"."                         {yylval.node = newTokenNode(DOT); return(DOT);};
".."                         {yylval.node = newTokenNode(DOTDOT);return(DOTDOT);};
"="                          {yylval.node = newTokenNode(EQUAL);yylval.node->op = OP_EQ;return(EQUAL);};
"!="                         {yylval.node = newTokenNode(notEQUAL);yylval.node->op = OP_NE;return(notEQUAL);};
">="                         {yylval.node = newTokenNode(GE);yylval.node->op = OP_GE;return(GE);};
">"                          {yylval.node = newTokenNode(GT);yylval.node->op = OP_GT;return(GT);};
"["                          {yylval.node = newTokenNode(LBRAC);return(LBRAC);};
"<="                         {yylval.node = newTokenNode(LE);yylval.node->op = OP_LE;return(LE);};
"("                          {yylval.node = newTokenNode(LPAREN);return(LPAREN);};
"<"                          {yylval.node = newTokenNode(LT);yylval.node->op = OP_LT;return(LT);};
"-"                          {yylval.node = newTokenNode(MINUS);yylval.node->op = OP_SUB;return(MINUS);};
"<>"                         {yylval.node = newTokenNode(NOTEQUAL);yylval.node->op = OP_NE;return(NOTEQUAL);};
"+"                          {yylval.node = newTokenNode(PLUS);yylval.node->op = OP_ADD;return(PLUS);};
"]"                          {yylval.node = newTokenNode(RBRAC);return(RBRAC);};
")"                          {yylval.node = newTokenNode(RPAREN);return(RPAREN);};
";"                          {yylval.node = newTokenNode(SEMICOLON);return(SEMICOLON);};
"/"                         {yylval.node = newTokenNode(SLASH); yylval.node->op = OP_DIV;return(SLASH);};
"*"                          {yylval.node = newTokenNode(STAR);yylval.node->op = OP_MUL;return(STAR);};

"(*"   |
"{"    { register int c;
     while ((c = input()))
     {
      if (c == '}')
       break;
      else if (c == '*')
      {
       if ((c = input()) == ')')
        break;
       else
        unput (c);
      }
      else if (c == '\n')
       lineCount++;
      else if (c == 0)
        fprintf (stderr, "Unexpected EOF inside comment at line %d\n",lineCount);
     }
    }
[ \t\f]    ;

\r         ;
\n    lineCount++;
.    {fprintf(stderr,"Lexical analyzer error at line %d : %s\n",lineCount,yytext);}
%%

struct nodeType* newTokenNode(int tokenType) {
    struct nodeType *node = newNode(NODE_TOKEN);
    node->tokenType = tokenType;
	//node->line = lineCount;
    return node;
}
