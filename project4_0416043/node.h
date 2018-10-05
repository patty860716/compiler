#ifndef __NODE_H__
#define __NODE_H__

struct nodeType;

#define VALUE_INVALID   0
#define NODE_PROGRAM          1
#define NODE_OPTIONAL_STATEMENT             2
#define NODE_STATEMENT_LIST            3
#define NODE_STATEMENT           4
#define NODE_FACTOR        5
#define NODE_IDLIST       6
#define NODE_DECLARATION       7
#define NODE_TYPE      8
#define NODE_STANDARDTYPE    9
#define NODE_SUBPROGRAM_DECLARTIONS        10
#define NODE_SUBPROGRAM_DECLARTION  11
#define NODE_SUBPROGRAM_HEAD        12
#define NODE_ARGUMENT             13
#define NODE_PARAMETERLIST           14
#define NODE_OPTIONALVAR           15
#define NODE_COMPOUNDSTATEMENT        16
#define NODE_VARIABLE       17
#define NODE_TAIL       18
#define NODE_PROCEDURESTATEMENT      19
#define NODE_EXPRESSIONLIST    20
#define NODE_EXPRESSION        21
#define NODE_SIMPLEEXPRESSION           22
#define NODE_TERM       23
#define NODE_ADDOP       24
#define NODE_MULOP      25
#define NODE_RELOP   26
#define NODE_comment 68
#define NODE_lambda 69
#define NODE_TYPE_INT 70
#define NODE_TYPE_STRING 71
#define NODE_TYPE_REAL 72
#define NODE_TYPE_ARRAY 73
#define NODE_SYM_REF 95
#define OP_ADD  74
#define OP_SUB  75
#define OP_MUL  76
#define OP_DIV  77
#define OP_GT   78
#define OP_LT   79
#define OP_EQ   80
#define OP_GE   81
#define OP_LE   82
#define OP_NE   83
#define OP_NOT  84
#define NODE_VAR_PROC 85
#define NODE_SUBPROGRAM_FUNC 86
#define NODE_SUBPROGRAM_PROCEDURE 87
#define NODE_INT 88
#define NODE_REAL 89
#define NODE_OP 90
#define NODE_ASSIGN_STATEMENT 91
#define NODE_TOKEN 93
#define NODE_END 94
#define NODE_IF_STATEMENT 96
#define NODE_WHILE_STATEMENT 97
#define NODE_STRING 98
#include"symtab.h"
struct nodeType {
    int nodeType;
    struct nodeType *parent;
    struct nodeType *child;
    struct nodeType *lsibling;
    struct nodeType *rsibling;

    /* Attribute for NODE_TOKEN */
    int tokenType;
	int line;
    int iValue;
    double rValue;
    char valueValid;
	//int init;
    char *string;
	int arrdimension;
    char op;
	enum StdType valueType;
    struct SymTableEntry *entry;

};

struct nodeType* newNode(int type);
void deleteNode(struct nodeType* node);
void addChild(struct nodeType *node, struct nodeType *child);
void printTree(struct nodeType *node, int ident);

#endif

