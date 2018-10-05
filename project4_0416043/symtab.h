#ifndef __SYMTAB_H___
#define __SYMTAB_H___
#define maxsize 100
enum StdType {TypeInt,TypeString, TypeReal,Array,func,var,proc,typeno};
struct paramlist {
    enum StdType para[maxsize];
	int dimension[maxsize];
	int paramsize;
};
struct arrayrange {
   int rr[10][3];
	int d;
};
#include "node.h"
struct SymTableEntry {
    char name[100];
    enum StdType type,dtype,addtype;  //change
	struct paramlist* parameter;  //changed
	int scopenum;
	int init,val;
	struct arrayrange *range;
	
};

struct SymTable {
    int size;
    struct SymTableEntry entries[100];
};

extern struct SymTable SymbolTable;

//void aaa(struct nodeType* node);
void semanticCheck(struct nodeType* node);

void codegenerate(struct nodeType* node);
#endif

