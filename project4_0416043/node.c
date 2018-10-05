#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "node.h"
int state=0;
struct nodeType* newNode(int type) {
    struct nodeType *node = (struct nodeType*)malloc(sizeof(struct nodeType));
    node->nodeType = type;
    node->valueValid = VALUE_INVALID;
    node->string = NULL;
    node->parent = NULL;
    node->child = NULL;
    node->lsibling = node;
    node->rsibling = node;
	node->arrdimension = 0;
    return node;
}

void addChild(struct nodeType *node, struct nodeType *child) {
    child->parent = node;

    if(node->child == NULL) {
        node->child = child;
    }
    else {
        child->lsibling = node->child->lsibling;
        child->rsibling = node->child;
        node->child->lsibling->rsibling = child;
        node->child->lsibling = child;
    }
}

void deleteNode(struct nodeType *node) {
    if(node->string != NULL)
        free(node->string);
    free(node);
}

void printTree(struct nodeType *node, int ident) {
    static char blank[1024];
    for(int i=0; i<ident; i++)
        blank[i] = ' ';
    blank[ident] = 0;

    switch(node->nodeType) {
       
		
        case NODE_PROGRAM:
            printf("%s/PROGRAM\n", blank);
            ident += 8;
            break;
	case NODE_ASSIGN_STATEMENT:
            printf("%s/NODE_ASSIGN_STATEMENT\n", blank);
            ident += 8;
            break;
	case NODE_TYPE_ARRAY:
            printf("%s/NODE_TYPE_ARRAY\n", blank);
            ident += 8;
            break;
        case NODE_IDLIST:
            printf("%s/IDENTIFIER LIST\n", blank);
            ident += 8;
            break;
	case NODE_END:
            printf("%s/PEND\n", blank);
            ident += 8;
            break;
        case NODE_DECLARATION:
            printf("%s/DECLARATION\n", blank);
            ident += 8;
            break;
        case NODE_TYPE:
            printf("%s/TYPE\n", blank);
            ident += 8;
            break;
        case NODE_TYPE_INT:
            printf("%s/TYPE int\n", blank);
            ident += 8;
            break;
	case NODE_TYPE_REAL:
            printf("%s/TYPE REAL\n", blank);
            ident += 8;
            break;
	case NODE_TYPE_STRING:
            printf("%s/TYPE STRING\n", blank);
            ident += 8;
            break;
	case NODE_STANDARDTYPE:
            printf("%s/STANDARD TYPE\n", blank);
            ident += 8;
            break;
        case NODE_SUBPROGRAM_DECLARTIONS:
            printf("%s/SUBPROGRAM DECLARATIONS\n", blank);
            ident += 8;
            break;
        case NODE_SUBPROGRAM_DECLARTION:
            printf("%s/SUBPROGRAM DECLARATION\n", blank);
            ident += 8;
            break;
        case NODE_SUBPROGRAM_FUNC:
            printf("%s/SUBPROGRAM HEAD FUNC\n", blank);
            ident += 8;
            break;
	case NODE_SUBPROGRAM_PROCEDURE:
            printf("%s/SUBPROGRAM HEAD PROCEDURE\n", blank);
            ident += 8;
            break;
        case NODE_ARGUMENT:
            printf("%s/ARGUMENTS\n", blank);
            ident += 8;
            break;
        case NODE_PARAMETERLIST:
            printf("%s/PARAMETERT LIST\n", blank);
            ident += 8;
            break;
        case NODE_OPTIONALVAR:
            printf("%s/OPTIONAL VAR\n", blank);
            ident += 8;
            break;
        case NODE_COMPOUNDSTATEMENT:
            printf("%s/COMPOUND STATEMENT\n", blank);
            ident += 8;
            break;
        case NODE_OPTIONAL_STATEMENT:
            printf("%s/OPTIONAL STATEMENT\n", blank);
            ident += 8;
            break;
	case NODE_VAR_PROC:
            printf("%s/VAR_PROC\n", blank);
            ident += 8;
            break;
        case NODE_STATEMENT_LIST:
            printf("%s/STATEMENT LIST\n", blank);
            ident += 8;
            break;
        case NODE_STATEMENT:
            printf("%s/STATEMENT\n", blank);
            ident += 8;
            break;
        case NODE_VARIABLE:
            printf("%s/VARIABLE\n", blank);
            ident += 8;
            break;
        case NODE_TAIL:
            printf("%s/TAIL\n", blank);
            ident += 8;
            break;
        case NODE_PROCEDURESTATEMENT:
            printf("%s/PROCEDURE STATEMENT\n", blank);
            ident += 8;
            break;
        case NODE_EXPRESSIONLIST:
            printf("%s/EXPRESSION LIST\n", blank);
            ident += 8;
            break;
        case NODE_EXPRESSION:
            printf("%s/EXPRESSION\n", blank);
            ident += 8;
            break;
        case NODE_SIMPLEEXPRESSION:
            printf("%s/SIMPLE EXPRESSION\n", blank);
            ident += 8;
            break;
        case NODE_TERM:
            printf("%s/TERM\n", blank);
            ident += 8;
            break;
        case NODE_FACTOR:
            printf("%s/FACTOR\n", blank);
            ident += 8;
            break;
	 case NODE_SYM_REF:
            printf("%s/NODE_SYM_REF\n", blank);
            ident += 8;
            break;
        case NODE_OP:
            switch(node->op) {
                case OP_ADD:
                    printf("%s+\n", blank);
                    break;
                case OP_SUB:
                    printf("%s-\n", blank);
                    break;
                case OP_MUL:
                    printf("%s*\n", blank);
                    break;
                case OP_DIV:
                    printf("%s/\n", blank);
                    break;
                case OP_GT:
                    printf("%s>\n", blank);
                    break;
                case OP_LT:
                    printf("%s<\n", blank);
                    break;
                case OP_EQ:
                    printf("%s=\n", blank);
                    break;
                case OP_GE:
                    printf("%s>=\n", blank);
                    break;
                case OP_LE:
                    printf("%s<=\n", blank);
                    break;
                case OP_NE:
                    printf("%s!=\n", blank);
                    break;
            }
            ident++;
            break;
        case NODE_TOKEN:
		printf("%s/%s %d",blank,node->string,node->tokenType);
            break;
	case NODE_INT:
		 printf("%s/NODE_INT\n", blank);
            ident += 8;
            break;
	case NODE_REAL:
		 printf("%s/NODE_REAL\n", blank);
            ident += 8;
            break;
	case NODE_lambda:
            printf("%s/lambda\n", blank);
            break;
        default:
            printf("%sdefault:%d\n", blank, node->nodeType);
          break;
    }

    struct nodeType *child = node->child;
    if(child != NULL) {
        do {
            printTree(child, ident);
            child = child->rsibling;
        } while(child != node->child);
    }
}

