#include "symtab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern struct SymTable SymbolTable;
struct paramlist* param;
int cnt = 0;
int scope = 0; 
enum StdType dcvalueType;
int arrdimension = 0;
struct arrayrange *range;
extern FILE* f_assembly;
int init = 0;
extern char* classname;
int localnum=0;
int leftright = 0;
int ifwhile = 0;
int labelnum = 0;
extern int noerror;
struct paramlist* new_para()
{
	struct paramlist* tmp = (struct paramlist*)malloc(sizeof(struct paramlist));
	memset(tmp->para,0,sizeof(enum StdType)*maxsize);
	memset(tmp->dimension,0,sizeof(int)*maxsize);
	tmp->paramsize = 0;
	return tmp;

}
struct arrayrange* newrange()
{
	struct arrayrange* tmp = (struct arrayrange*)malloc(sizeof(struct arrayrange));
	memset(tmp->rr,0,sizeof(int)*3*10);
	tmp->d=0;
	return tmp;

}
struct SymTableEntry* findSymbol(char *s) {
    for(int i=0; i<SymbolTable.size; i++) {
		if(SymbolTable.entries[i].scopenum==-1)
			continue;
        if(strcmp(s, SymbolTable.entries[i].name) == 0) {
            return &SymbolTable.entries[i];
        }
    }
    return 0;
}
struct SymTableEntry* findaSymbol(char *s) {
	int samename[SymbolTable.size];
	int y=0;
    for(int i=0; i<SymbolTable.size; i++) {
		if(SymbolTable.entries[i].scopenum==-1)
			continue;
        if(strcmp(s, SymbolTable.entries[i].name) == 0) {
		samename[y] = i;
		y++;
            //return &SymbolTable.entries[i];
        }
    }
	for(int i=0;i<y;i++)
	{
	if(scope == SymbolTable.entries[samename[i]].scopenum)
	{
	return &SymbolTable.entries[samename[i]];
	}
	}
	if(y>0)
	return &SymbolTable.entries[samename[0]];
    return 0;
}

struct SymTableEntry* addVariable(char *s, enum StdType type,struct paramlist* para,enum StdType type1,int scopen,int n,int initial,enum StdType add,struct arrayrange* r,int val) {
	struct SymTableEntry* tmp = findaSymbol(s);
    if(tmp!= 0&&tmp->scopenum == scopen) {
        printf("Error: duplicate declaration of variable %s in line %d\n", s,n);
	noerror = 1;
        return 0;//exit(0);
    }int index = SymbolTable.size;
    SymbolTable.size++;
    strcpy(SymbolTable.entries[index].name, s);
    SymbolTable.entries[index].type = type;
    SymbolTable.entries[index].parameter = para;
	SymbolTable.entries[index].dtype = type1;
	SymbolTable.entries[index].scopenum = scopen;
	SymbolTable.entries[index].init = initial;
	SymbolTable.entries[index].addtype = add;
	SymbolTable.entries[index].range = r;
	SymbolTable.entries[index].val = val;
	//printf("new variable %s in line %d enter in symbol table scope %d\n", s,n,scopen);
	
    return &SymbolTable.entries[index];
}

struct nodeType* nthChild(int n, struct nodeType *node) {
    struct nodeType *child = node->child;
    for(int i=1; i<n; i++) {
        child = child->rsibling;
    }
    return child;
}

void dumpSymbol(int scopek)
{
	printf("name      valuetype    scope    type      addtype     dimension\n");
	for(int i=0;i<SymbolTable.size;i++)
	{
		if(SymbolTable.entries[i].scopenum == scopek)
		{      printf("%-10s",SymbolTable.entries[i].name);
			int x = (int)SymbolTable.entries[i].type;
			switch(x)
			{
				case 0:
					printf("typeInt      ");
					break;
				case 1:
					printf("typeReal     ");
					break;
				case 2:
					printf("typeArray    ");
					break;
				default:
					printf("no type      ");
					break;
			}
			printf("%-9d",scopek);
			x = (int)SymbolTable.entries[i].dtype;
			switch(x)
			{
				case 3:
					printf("function  ");
					break;
				case 4:
					printf("var       ");
					break;
				case 5:
					printf("procedure ");
					break;
				default:
					printf("no type  ");
					break;
			}
			x = (int)SymbolTable.entries[i].addtype;
			if(x==2) 
			{ 
			printf("array type  ");int k=0;
			while(1)
			{
			if(SymbolTable.entries[i].range->rr[k][2]==0)
			break;
			k++;
			}
			printf("%d\n",k);
			}
			else printf("          \n");
		}
	}
	
}
char selectir(enum StdType a)
{
	char k;
	if(a==TypeInt)
		k = 'I';
	else
		k = 'F';
		return k;
}
void destroySymbol(int scopek)
{
	for(int i=0;i<SymbolTable.size;i++)
	{
		if(SymbolTable.entries[i].scopenum == scopek)
		{
			SymbolTable.entries[i].scopenum = -1;
			/*for(int j=i;j<SymbolTable.size-1;j++)
			{
				strcpy(SymbolTable.entries[j].name, SymbolTable.entries[j+1].name);
				SymbolTable.entries[j].scopenum=SymbolTable.entries[j+1].scopenum;
				SymbolTable.entries[j].dtype=SymbolTable.entries[j+1].dtype;
				SymbolTable.entries[j].type=SymbolTable.entries[j+1].type;
				SymbolTable.entries[j].parameter=SymbolTable.entries[j+1].parameter;
			}*/
			//SymbolTable.size--;
			//i--;
		}
	}
	
}
void semanticCheck(struct nodeType *node) {
  
  switch(node->nodeType) {
	
    case NODE_DECLARATION: 
	{
		if(node->child == NULL)
		{
			return;
		}
		int k=1;
		while(1)
		{
			arrdimension = 0;
			struct nodeType *idList = nthChild(k, node);
            struct nodeType *typeNode = nthChild(k+1, node);  //TYPE
          enum StdType type,addtype;
			if(typeNode->nodeType == NODE_TYPE_ARRAY)
			{
				semanticCheck(typeNode);
				addtype = Array;
				//arrdimension =0;
				//dcvalueType = TypeArray;
			}
			else
			{ 	
				if(typeNode->nodeType == NODE_TYPE_INT)
					dcvalueType = TypeInt;
            	else if(typeNode->nodeType == NODE_TYPE_REAL)
					dcvalueType = TypeReal; 
				else{noerror = 1;
					printf("Error:undefined type name in line %d\n",node->line);}
				type = var;
				addtype = var;
				range = newrange();
			}
			struct nodeType *idNode = idList->child;
			struct paramlist* tmp = new_para();
				//range->d = arrdimension;
			//int numinscope = -1;
			do 
			{
				
				struct SymTableEntry* tt = addVariable(idNode->string, dcvalueType,tmp,type,scope,idList->line,0,addtype,range,localnum);           //new variable enter in symbol table
				if(scope!=0)              //local declare
				{
					localnum++;
				}
				idNode->entry = tt;
				//if(tt==0)
				//break;
				idNode = idNode->rsibling;
			} while(idNode != idList->child);
			
			k+=2;
			if(typeNode->rsibling==node->child) break;
		}
	
		return;
    }
	case NODE_SUBPROGRAM_DECLARTION:
	{
		scope++;
		//printf("new scope begin\n");
		struct nodeType *child = node->child;
		semanticCheck(child); //head
		struct SymTableEntry* tt = child->entry;
		
		child = child->rsibling;  //declare var
		semanticCheck(child);//} 
		char c;
		if(tt->dtype == var)
		{
			
			tt->val = localnum;         //stay
			
		}
		child = child->rsibling;   //compound statement
		semanticCheck(child);
		localnum = 0;
		//dumpSymbol(scope);
		destroySymbol(scope);scope--;
		//printf("scope close\n\n");
		return;
	}
	case NODE_TYPE_ARRAY:
	{
		struct nodeType *typearr = nthChild(3, node);
		struct nodeType *num1 = nthChild(1, node);
		struct nodeType *num2 = nthChild(2, node);
		range = newrange();
		int i=0;
		arrdimension = 1;
			while(1)
			{
				range->rr[i][0] = num1->iValue;
				range->rr[i][1] = num2->iValue;
				range->rr[i][2] = i+1;
				if(typearr->nodeType == NODE_TYPE_INT)
				{dcvalueType = TypeInt;break;}
				else if(typearr->nodeType == NODE_TYPE_REAL)
				{dcvalueType = TypeReal;break;}
				else if(typearr->nodeType == NODE_TYPE_ARRAY)
				{
				arrdimension++;
				
				}
				num1 = nthChild(1, typearr);
				num2 = nthChild(2, typearr);typearr = nthChild(3, typearr);i++;	
			}
		range->d = arrdimension;	
		return;
	}
	
	case NODE_PARAMETERLIST:
	{
        struct nodeType *typeNode = nthChild(2, node);     //parameter type
        enum StdType type,addtype;
		arrdimension = 0;
		if(typeNode->nodeType == NODE_TYPE_ARRAY)
		{
			semanticCheck(typeNode);
			addtype = Array;
		}
		else 
		{
			if(typeNode->nodeType == NODE_TYPE_INT)
				dcvalueType = TypeInt;
			else if(typeNode->nodeType == NODE_TYPE_REAL)
				dcvalueType = TypeReal; 
			else{noerror = 1;
			printf("Error:undefined type name in line %d\n",node->line);}
			type = var;
			addtype = var;range = newrange();
		}
        struct nodeType *idList = nthChild(1, node);      //idlist
        struct nodeType *idNode = idList->child;
		range->d = arrdimension;
		struct paramlist* tmp = new_para();
		localnum = 0;
        do{
            struct SymTableEntry* tt = addVariable(idNode->string, dcvalueType,tmp,type,scope,node->line,1,addtype,range,localnum);           //new variable enter in symbol table
			//if(tt == 0)
			//break;				
			localnum++;
			param->para[cnt] = dcvalueType;
			param->dimension[cnt] = arrdimension;
			cnt++;
            idNode = idNode->rsibling;
        } while(idNode != idList->child);
		struct nodeType *thirdNode = nthChild(3, node);
		if(thirdNode->nodeType==NODE_PARAMETERLIST)
			semanticCheck(thirdNode);
        return;
	}
	case NODE_SUBPROGRAM_FUNC: 
	{
		arrdimension = 0;
		enum StdType addtype,type,valueType;
		type = func;
		struct nodeType *idNode = nthChild(1, node);
		struct nodeType *para = nthChild(2, node);      //argument->parameterlist
		struct nodeType *typeNode = nthChild(3, node);
		if(typeNode->nodeType == NODE_TYPE_ARRAY)
		{
			semanticCheck(typeNode);
			addtype = Array;
			valueType = dcvalueType;
		}
		else 
		{
			if(typeNode->nodeType == NODE_TYPE_INT)
               	valueType = TypeInt;
            else if(typeNode->nodeType == NODE_TYPE_REAL)
                valueType = TypeReal; 
			else{noerror = 1;
			printf("Error:undefined type name in line %d\n",node->line);}
			addtype = func;range = newrange();
		}
		param = new_para();
		struct paramlist* tmp = new_para();
		if(para->nodeType!=NODE_lambda)
			semanticCheck(para);
		param->paramsize = cnt;
		cnt = 0;
		struct SymTableEntry* tt = addVariable(idNode->string, valueType,param,type,scope-1,node->line,1,addtype,range,0);
		
		if(tt==0) {/*scope--;*/return;}  //建function不成功
		
		/*if(valueType==TypeInt)
		{
			fprintf(f_assembly,"I\n\t.limit locals 100\n\t.limit stack 100\nldc 0\n");
		}
		else
		{
			fprintf(f_assembly,"F\n\t.limit locals 100\n\t.limit stack 100\nldc 0.0\n");
		}*/
		type = var;
		tt = addVariable(idNode->string, valueType,tmp,type,scope,node->line,0,addtype,range,0);  //一個跟function同名的var(return)
		node->entry = tt;
		if(tt==0) {/*scope--*/;return;}
		//printf("new table create\n");  
			//scope--;
		return;
	}
	case NODE_SUBPROGRAM_PROCEDURE: 
	{
		enum StdType valueType,type,addtype;
		valueType = typeno;
		type = proc;
		addtype= proc;
		struct nodeType *idNode = nthChild(1, node);
		struct nodeType *para = nthChild(2, node);        //argument->parameterlist
		param = new_para();
		semanticCheck(para);
		param->paramsize = cnt;range = newrange();
		cnt = 0;
		struct SymTableEntry* tt = addVariable(idNode->string, valueType,param,type,scope-1,node->line,1,addtype,range,0);
		node->entry = tt;
		if(tt==0) return;
		
		//type = var;
		return;
	}
    case NODE_VAR_PROC:
	{
		struct nodeType *idNode = nthChild(1, node);
		//printf("%s",idNode->string);
		struct SymTableEntry *entry = findSymbol(idNode->string);
		
			/*if(entry == 0) {
                printf("Error: undeclared variable %s in line %d\n", node->string,node->line);
                //return;
			exit(0);
            }*/		
	if(entry!=0&&entry->dtype == var)
		{
			struct SymTableEntry *entry1 = findaSymbol(idNode->string);
			if(entry1->dtype==func)
			entry = entry1;
		}
		if(entry!=0&&entry->init==0)
		{
			if(entry->scopenum==0&&scope!=0){}
			else{
			noerror = 1;
			node->valueType = typeno;
			printf("Error: the factor %s hasn't been initialized in line %d\n", idNode->string,node->line); 
			return;			
			}
  
			//return;
		}
		
		if(entry!=0&&entry->dtype == proc)
		{noerror = 1;
			printf("Error: procedure can not be used as a factor %s in line %d\n", idNode->string,node->line);
            //exit(0);
			node->valueType = typeno;
			return;
		}
		semanticCheck(idNode);	
		node->valueType = idNode->valueType;
		node->arrdimension = idNode->arrdimension;
		//printf("%d",entry->type);
		if(idNode->entry !=NULL)
			node->entry = idNode->entry;
			//node->string = idNode->string;
		return;	
	}
        case NODE_SYM_REF: {
	//printf("symref %s\n",node->string);
			char s[] = "printInt";
			char s1[] = "printReal";
			char s2[] = "printString";
			if(strcmp(s,node->string)==0)
			{
				
				struct nodeType* rsib = node->child->child;
				if(rsib==NULL)
				{
				noerror=1;printf("Error: wrongType in printInt for %s in line %d\n",node->string,node->line);node->valueType = typeno; return;
				}
				leftright = 0;
				do{
					
					semanticCheck(rsib);
					if(rsib->valueType!=TypeInt){noerror=1;printf("Error: wrongType in printInt for %s in line %d\n",node->string,node->line);node->valueType = typeno; return;}
					rsib = rsib->rsibling;
				}while(rsib!=node->child->child);
				return;
			}
			if(strcmp(s1,node->string)==0)
			{
				
				struct nodeType* rsib = node->child->child;
				if(rsib==NULL)
				{
				noerror=1;printf("Error: wrongType in printInt for %s in line %d\n",node->string,node->line);node->valueType = typeno; return;
				}
				leftright = 0;
				do{
					
					semanticCheck(rsib);
					if(rsib->valueType==TypeString){noerror=1;printf("Error: wrongType in printInt for %s in line %d\n",node->string,node->line);node->valueType = typeno; return;}
					rsib = rsib->rsibling;
				}while(rsib!=node->child->child);
				return;
			}
		if(strcmp(s2,node->string)==0)
			{
				
				struct nodeType* rsib = node->child->child;
				if(rsib==NULL)
				{
				noerror=1;printf("Error: wrongType in printInt for %s in line %d\n",node->string,node->line);node->valueType = typeno; return;
				}
				leftright = 0;
				do{
					
					semanticCheck(rsib);
					if(rsib->valueType!=TypeString){noerror=1;printf("Error: wrongType in printInt for %s in line %d\n",node->string,node->line);node->valueType = typeno; return;}
					rsib = rsib->rsibling;
				}while(rsib!=node->child->child);
				return;
			}
            struct SymTableEntry *entry = findaSymbol(node->string);
            if(entry == 0) 
			{
				node->valueType = typeno;
				noerror = 1;
				printf("Error: undeclared variable %s in line %d\n", node->string,node->line);
				return;
				//exit(0);
            }
		/*if(correct == 1)
		{
		if(entry->type==TypeReal)
		{
		fprintf(f_assembly,".0\n");}correct = 0;}*/
		//struct nodeType* rsib = node->rsibling;
		struct nodeType* rsib = node->child;
			//printf("%d",rsib->nodeType);
		if(rsib==NULL)
		{
			if(entry->dtype!=proc){
			printf("Error: %s in line %d\n",node->string,node->line);
			
			node->valueType = typeno;}
			else
			node->entry = entry;	
			return;
		}
		if(rsib->nodeType==NODE_EXPRESSIONLIST)   //檢查parameter
		{ 
			if(entry->dtype!=func&&entry->dtype!=proc)
			entry = findSymbol(node->string);
			struct paramlist* tmppa = entry->parameter;
			if(entry->dtype == var)
			{noerror = 1;
				node->valueType = typeno;
				printf("Error: the parameter construction %s is different in line %d\n",node->string,node->line);
				return;
			}
				
			rsib = rsib->child;
			int k=0;
			//char k[]="";
			
			do{
				semanticCheck(rsib);
				if(rsib->valueType == typeno){node->valueType = typeno; return;}
				/*if(rsib->entry!=NULL&&entry->addtype != rsib->entry->addtype)
				{
					node->valueType = typeno;
						printf("aaError: the parameter construction %s is different in line %d\n",node->string,node->line);
						return;
				}*/
				if(tmppa->para[k]!=rsib->valueType||tmppa->dimension[k]!=rsib->arrdimension)//=================
				{noerror = 1;
					node->valueType = typeno;
					printf("Error: the parameter construction %s is different in line %d\n",node->string,node->line);
					return;//exit(0);
				}
					rsib = rsib->rsibling;	
					k++;
					//printf("%d",tmppa->paramsize);
			}while(rsib!=node->child->child&&k<tmppa->paramsize);  //printf("wwww%d %d",k,tmppa->paramsize);
			if(k!=tmppa->paramsize||rsib!=node->child->child) 
			{noerror = 1;
				node->valueType = typeno;
				printf("Error: the parameter construction %s is different in line %d\n",node->string,node->line);
				return;//exit(0);
			}
			
			
		}
		else if(rsib->nodeType==NODE_TAIL)
		{
				
			if(entry->addtype != Array /*&&entry->dtype == var*/)
			{noerror = 1;
				node->valueType = typeno;
				printf("wrong use of [] in non-Arraytype variable %s in line %d\n",node->string,node->line);
				return;
			}
			if(entry->dtype == func)
			{
				
				
				if(entry->parameter->paramsize!=0)
				{noerror = 1;
					node->valueType = typeno;
                			printf("Error: wrong use of [] in function with parameters %s in line %d\n", node->string,node->line);
					return;
				}
			}
			struct nodeType *tailNode = rsib;
			struct nodeType *exNode;
			//int yy = entry->val; 
			int k=0;
			
			//==============================================================================change
			while(tailNode->nodeType==NODE_TAIL)
			{
				k++;
				tailNode = nthChild(2,tailNode);  //tail or lambda
			}
			if(k>10||k>entry->range->d)
			{noerror = 1;
				node->valueType = typeno;
				printf("Error: dimension [] of array %s is wrong in line %d\n", node->string,node->line);
				return;
			}
			
			tailNode = rsib;
			
			for(int i=0;i<k;i++)
			{
				exNode = nthChild(1,tailNode);  //expression
				tailNode = nthChild(2,tailNode);  //tail or lambda
				semanticCheck(exNode);	        
						//printf("%d\n",exNode->nodeType);
				if(exNode->valueType == typeno) 
				{	noerror = 1;
					printf("Error: iteration in array %s [] is wrong in line %d\n", node->string,node->line);
					node->valueType = typeno;
					return;
				}
				//else yy =  exNode->entry->val;
						//printf("kkk%s %d\n",exNode->entry->name,exNode->entry->val);}
				if(exNode->valueType != TypeInt)
				{noerror = 1;
					node->valueType = typeno;
					printf("Error: iteration in array %s [] is not type integer in line %d\n", node->string,node->line);
					return;
				}
				
					//printf("www%d %d\n",entry->range->rr[k][0],yy);	
				/*if((entry->range->rr[k][0]>yy||entry->range->rr[k][1]<yy)&&exNode->nodeType==NODE_INT)
				{
					node->valueType = typeno;
					printf("Error: iteration in array %s [] is out-of-range in line %d\n", node->string,node->line);
					return;
				}*/
			}
			node->arrdimension = entry->range->d-k;   //if the id which needs to  be referenced is an array->arrdimension !=0 
			
			//==============================================================================change
			
					
				   //node->arrdimension = k;
				   
						
		}
			else  //node_lambda
			{
				/*if(entry->addtype == Array)
				{ 
					printf("Error: array %s doesn't have tail in line %d\n",node->string,node->line);
				  	return;
				} */ 
				if(entry->addtype == Array)
				{ 
					node->arrdimension = entry->range->d;
				}
				//struct SymTableEntry *entry1 = findSymbol(node->string);
				if(entry->dtype==func||entry->dtype==proc)
				{
					struct paramlist* tmppa = entry->parameter;
					//struct SymTableEntry *entry1 = findaSymbol(node->string);
					//if(entry1->dtype==func)
					//{
						if(tmppa->paramsize > 0){
							noerror = 1;
						node->valueType = typeno;
						printf("Error: the parameter construction %s is different in line %d\n",node->string,node->line);
						return;
						}
						
					//}
					//else {entry = entry1;}
					
				}
				
				
				
			}
				node->entry = entry;
            			node->valueType = entry->type;            
            			return;
		}

        case NODE_INT: {
            node->valueType = TypeInt;
			
			
            return;
        }

        case NODE_REAL: {
		
            node->valueType = TypeReal;
			
			
            return;
        }
	case NODE_STRING: {
		
            node->valueType = TypeString;
			
			
            return;
        }
        case NODE_OP:
		{
			struct nodeType *child1 = nthChild(1, node);
            struct nodeType *child2 = nthChild(2, node);
			/*if(child1->nodeType == NODE_INT)
			correct =1;*/
			semanticCheck(child1);
			//fprintf(f_assembly,".0")
			/*if(child2->nodeType==NODE_INT)
			correct ++;*/
            		semanticCheck(child2);
			
			if(child1->valueType == typeno||child2->valueType == typeno)
			{
				node->valueType = typeno;return;
			}
			
			//printf("%d,%d\n",child1->valueType,child2->valueType);
			if(child1->entry!=NULL&&child2->entry!=NULL)
			{
				if((child1->valueType==TypeString||child2->valueType==TypeString)||child1->arrdimension!=child2->arrdimension)
				{ 
					node->valueType = typeno;
					noerror = 1;
                    printf("Error: type mismatch for operator in line %d\n",node->line);
                return;
				}
				
			}
			node->arrdimension = child1->arrdimension;
				if(child1->valueType==TypeReal||child2->valueType==TypeReal)
				node->valueType = TypeReal;
				else node->valueType = TypeInt;
				
				
				//node->entry = child1->entry;
				return;
			
		}
        case NODE_ASSIGN_STATEMENT: 
		{
            struct nodeType *child1 = nthChild(1, node);           //symref
            struct nodeType *child2 = nthChild(2, node);           //expression
			
			semanticCheck(child1);
			
			semanticCheck(child2);  //VAR_PROC OR NODE_OP OR INT/REAL
			
			
		//printf("%d",child1->nodeType);
            
			//struct nodeType *child11 = child1;//symref id //printf("%d",child11->nodeType); 
			
			//semanticCheck(child11);
			if(child1->valueType==typeno)
			{
				return;
			}
			if(child1->entry->dtype == func||child1->entry->dtype == proc)
			{noerror = 1;
                    printf("Error: LHS can't be a function or a procedure in %d\n",node->line);
                return;
            }
			if(child2->valueType==typeno)
			return;
		//printf("%d %d",child11->valueType,child2->valueType);
            if(child1->valueType != child2->valueType) {
			/*if(child11->valueType==TypeReal)
			{
			if(child2->valueType==typeno){printf("Error: type mismatch for assignment in line %d\n",node->line);return;}
			}
                    else*/ noerror = 1;
					printf("Error: type mismatch for assignment in line %d\n",node->line);
                return;
            }
		//printf("%d ",child2->nodeType);
			if(child2->nodeType==NODE_VAR_PROC)
			{struct nodeType *child21 = nthChild(1, child2);
				if(child21->entry==NULL){return;}
				if(/*child11->entry->addtype == TypeArray&&*/child1->arrdimension!=child2->arrdimension)
				{
				
					//if(child2->arrdimension!=child11->arrdimension){
						noerror = 1;
		 				printf("Error: type mismatch for assignment in line %d\n",node->line);
						return;
					//}
				}
				/*if(child2->valueType!=child11->valueType){
				printf("Error: type mismatch for assignment in line %d\n",node->line);
						return;}*/
				
			}
			else
			{
				if(child1->entry->addtype == Array&&child1->arrdimension!=0)
				{
					noerror = 1;
					printf("Error: type mismatch Array and num for assignment in line %d\n",node->line);
						return;}
			}
		/*if(child2->valueType == TypeInt)
		{
		//struct SymTableEntry *aa = findSymbol(child11->entry->name);
		//if(child11->entry->dtype==var&&aa->dtype==func)
		//aa->val = child2->iValue;
		child11->entry->val = child2->iValue;
		//printf("%s %d\n",child11->entry->name,child11->entry->val);
		}*/
		
			child1->entry->init = 1;
			
		            node->valueType = child1->valueType;
            return;
        }
		

    }

    /* Default action for other nodes not listed in the switch-case */
    struct nodeType *child = node->child;
    if(child != 0) {
        do {
            semanticCheck(child);
            child = child->rsibling;
        } while(child != node->child);
    }
}
void codegenerate(struct nodeType *node) {
  
  switch(node->nodeType) {
	
	/*case NODE_DECLARATION:
	return;
	case NODE_SUBPROGRAM_DECLARTIONS:
	return;*/
	case NODE_PROGRAM:
	{
		fprintf(f_assembly,".class public %s\n",classname);
		fprintf(f_assembly,".super java/lang/Object\n");
		break;
	}
    case NODE_DECLARATION: 
	{
	if(node->child == NULL)
		{
			return;
		}
	int k=1;
		while(1)
		{
			arrdimension = 0;
			struct nodeType *idList = nthChild(k, node);
            struct nodeType *typeNode = nthChild(k+1, node);  //TYPE
           
			struct nodeType *idNode = idList->child;
				//range->d = arrdimension;
			//int numinscope = -1;
			do                           
			{
				
				if(scope==0)
				{
					init = 1;
					fprintf(f_assembly,".field public static %s ",idNode->string);
					for(int i=0;i<idNode->entry->range->d;i++)
					{
						fprintf(f_assembly,"[");
					}
					fprintf(f_assembly,"%c\n",selectir(idNode->entry->type));
				}
				else   //local declare
				{
					char c = selectir(idNode->entry->type)+32;
					if(idNode->entry->addtype==Array)
					{
						for(int i=0;i<idNode->entry->range->d;i++)
						{int stack = idNode->entry->range->rr[i][1]-idNode->entry->range->rr[i][0]+1;
						fprintf(f_assembly,"bipush %d\n",stack);
						}
						fprintf(f_assembly,"multianewarray ");
					for(int j=0;j<idNode->entry->range->d;j++)
					{
						fprintf(f_assembly,"[");
					}
					fprintf(f_assembly,"%c %d\n",selectir(idNode->entry->type),idNode->entry->range->d);
					fprintf(f_assembly,"astore %d\n",idNode->entry->val);
					}
					else{
					if(c=='i')
					{
						fprintf(f_assembly,"ldc 0\n%c",c);
						
					}	
					else{
						fprintf(f_assembly,"ldc 0.0\n%c",c);}
					//if(idNode->entry->addtype==Array)
					//	fprintf(f_assembly,"a");
					fprintf(f_assembly,"store %d\n",idNode->entry->val);}
				}
				//if(tt==0)
				//break;
				idNode = idNode->rsibling;
			} while(idNode != idList->child);
			k+=2;
			if(typeNode->rsibling==node->child) break;
		}
	
		return;
    }
	case NODE_SUBPROGRAM_DECLARTIONS:
	{
		if(init == 1&&scope==0)
		{
			fprintf(f_assembly,".method public static vinit()V\n\t.limit locals 100\n\t.limit stack 100\n");
			for(int i=0;i<SymbolTable.size;i++)
			{
				if(SymbolTable.entries[i].scopenum!=0||SymbolTable.entries[i].dtype==func||SymbolTable.entries[i].dtype==proc)
				continue;
				if(SymbolTable.entries[i].addtype == Array)
				{
					for(int k = 0 ;k < SymbolTable.entries[i].range->d;k++)
					{
						int stack = SymbolTable.entries[i].range->rr[k][1]-SymbolTable.entries[i].range->rr[k][0]+1;
						fprintf(f_assembly,"\tbipush %d\n",stack);
					}
					fprintf(f_assembly,"\tmultianewarray ");
					for(int j=0;j<SymbolTable.entries[i].range->d;j++)
					{
						fprintf(f_assembly,"[");
					}
					fprintf(f_assembly,"%c",selectir(SymbolTable.entries[i].type));
					fprintf(f_assembly," %d",SymbolTable.entries[i].range->d);
					fprintf(f_assembly,"\n");
					fprintf(f_assembly,"\tputstatic %s/%s ",classname,SymbolTable.entries[i].name);
					for(int j=0;j<SymbolTable.entries[i].range->d;j++)
					{
						fprintf(f_assembly,"[");
					}
					fprintf(f_assembly,"%c\n",selectir(SymbolTable.entries[i].type));
				}
				else
				{
					
					if(SymbolTable.entries[i].type==TypeInt)
					{
						fprintf(f_assembly,"\tldc 0\n\tputstatic %s/%s ",classname,SymbolTable.entries[i].name);
						fprintf(f_assembly,"I\n");
					}
					else
					{
						fprintf(f_assembly,"\tldc 0.0\n\tputstatic %s/%s ",classname,SymbolTable.entries[i].name);
						fprintf(f_assembly,"F\n");
					}
				}
			}
			fprintf(f_assembly,"\treturn\n.end method\n");
		}
		fprintf(f_assembly,".method public <init>()V\n\taload_0\n\tinvokenonvirtual java/lang/Object/<init>()V\n\treturn\n.end method\n");
		struct nodeType *child = node->child;
		if(child != 0) {
			do {
				codegenerate(child);
				child = child->rsibling;
			} while(child != node->child);
		}
		fprintf(f_assembly,".method public static main([Ljava/lang/String;)V\n\t.limit locals 100\n\t.limit stack 100\n");
		if(init == 1)
		{
			fprintf(f_assembly,"\tinvokestatic %s/vinit()V\n",classname);
			init = 0;
		}
			
		return;
	}
	case NODE_SUBPROGRAM_DECLARTION:
	{
		scope++;
		//printf("new scope begin\n");
		struct nodeType *child = node->child;
		codegenerate(child); //head
		//--------------------------------------------------------------------------
		struct SymTableEntry* tt = child->entry;
		
		child = child->rsibling;  //declare var
		codegenerate(child);//} 
		char c;
		if(tt->dtype == var)
		{
			c = selectir(tt->type) +32;
			if(tt->addtype==Array)
			{
				for(int i=0;i<tt->range->d;i++)
						{int stack = tt->range->rr[i][1]-tt->range->rr[i][0]+1;
						fprintf(f_assembly,"bipush %d\n",stack);
						}
						fprintf(f_assembly,"multianewarray ");
					for(int j=0;j<tt->range->d;j++)
					{
						fprintf(f_assembly,"[");
					}
					fprintf(f_assembly,"%c %d\n",selectir(tt->type),tt->range->d);
					fprintf(f_assembly,"astore %d\n",tt->val);
			}
			else{
			if(c=='i')
				fprintf(f_assembly,"ldc 0\n");
			else 
				fprintf(f_assembly,"ldc 0.0\n");
			//fprintf(f_assembly,"%c",c);
			
				//fprintf(f_assembly,"a");
		fprintf(f_assembly,"%c",c);
			fprintf(f_assembly,"store %d\n",tt->val);}
		}
		child = child->rsibling;   //compound statement
		codegenerate(child);
		
		if(tt->dtype == var)
		{
		fprintf(f_assembly,"%c",c);
		if(tt->addtype==Array)
		c = 'a';
				//fprintf(f_assembly,"a");
		fprintf(f_assembly,"load %d\n%c",tt->val,c);
		}
		fprintf(f_assembly,"return\n");
		fprintf(f_assembly,".end method\n");
		//dumpSymbol(scope);
		scope--;
		//printf("scope close\n\n");
		return;
	}
	case NODE_TYPE_ARRAY:
	return;
	
	case NODE_PARAMETERLIST:
	return;
	case NODE_SUBPROGRAM_FUNC: 
	{
		
		fprintf(f_assembly,".method public static %s(",node->entry->name);
		struct SymTableEntry* tt = findSymbol(node->entry->name);
		for(int i=0;i<tt->parameter->paramsize;i++)
		{
			for(int j=0;j<tt->parameter->dimension[i];j++)
			{
				fprintf(f_assembly,"[");
			}
			
				fprintf(f_assembly,"%c",selectir(tt->parameter->para[i]));
			
		}
		fprintf(f_assembly,")");
		for(int j=0;j<tt->range->d;j++)
		{
			fprintf(f_assembly,"[");
		}
		fprintf(f_assembly,"%c\n\t.limit locals 100\n\t.limit stack 100\n",selectir(tt->type));
		/*if(valueType==TypeInt)
		{
			fprintf(f_assembly,"I\n\t.limit locals 100\n\t.limit stack 100\nldc 0\n");
		}
		else
		{
			fprintf(f_assembly,"F\n\t.limit locals 100\n\t.limit stack 100\nldc 0.0\n");
		}*/
		
		//printf("new table create\n");  
			//scope--;
		return;
	}
	case NODE_SUBPROGRAM_PROCEDURE: 
	{
		
		fprintf(f_assembly,".method public static %s(",node->entry->name);
		for(int i=0;i<node->entry->parameter->paramsize;i++)
		{
			for(int j=0;j<node->entry->parameter->dimension[i];j++)
			{
				fprintf(f_assembly,"[");
			}
			fprintf(f_assembly,"%c",selectir(node->entry->parameter->para[i]));
		}
		fprintf(f_assembly,")V\n\t.limit locals 100\n\t.limit stack 100\n");
		//type = var;
		return;
	}
	case NODE_END:
	{
		//dumpSymbol(scope);
		destroySymbol(0); 
		fprintf(f_assembly,"return\n.end method\n");
		return;
	}
        case NODE_SYM_REF: {
	//printf("symref %s\n",node->string);
			char s[] = "printInt";
			char s1[] = "printReal";
			char s2[] = "printString";
			if(strcmp(s,node->string)==0)
			{
				
				fprintf(f_assembly,"getstatic java/lang/System/out Ljava/io/PrintStream;\n");
				struct nodeType* rsib = node->child->child;
				leftright = 0;
				do{
					codegenerate(rsib);
					fprintf(f_assembly,"invokestatic java/lang/String/valueOf(");
					for(int i=0;i<rsib->arrdimension;i++)
					fprintf(f_assembly,"[");
					fprintf(f_assembly,"I)Ljava/lang/String;\n");
					fprintf(f_assembly,"invokevirtual java/io/PrintStream/println(Ljava/lang/String;)V\n");
					rsib = rsib->rsibling;
				}while(rsib!=node->child->child);
				return;
			}
			if(strcmp(s1,node->string)==0)
			{
				fprintf(f_assembly,"getstatic java/lang/System/out Ljava/io/PrintStream;\n");
				struct nodeType* rsib = node->child->child;
				leftright = 0;
				do{
					codegenerate(rsib);
					fprintf(f_assembly,"invokestatic java/lang/String/valueOf(");
					for(int i=0;i<rsib->arrdimension;i++)
					fprintf(f_assembly,"[");
					
					fprintf(f_assembly,"F)Ljava/lang/String;\n");
					fprintf(f_assembly,"invokevirtual java/io/PrintStream/println(Ljava/lang/String;)V\n");
					rsib = rsib->rsibling;
				}while(rsib!=node->child->child);
				return;
			}
			if(strcmp(s2,node->string)==0)
			{
				fprintf(f_assembly,"getstatic java/lang/System/out Ljava/io/PrintStream;\n");
				struct nodeType* rsib = node->child->child;
				leftright = 0;
				do{
					codegenerate(rsib);
					//fprintf(f_assembly,"invokestatic java/lang/String/valueOf(");
					
					//fprintf(f_assembly,"F)Ljava/lang/String;\n");
					fprintf(f_assembly,"invokevirtual java/io/PrintStream/println(Ljava/lang/String;)V\n");
					rsib = rsib->rsibling;
				}while(rsib!=node->child->child);
				return;
			}
			struct SymTableEntry* entry = node->entry;
		/*if(correct == 1)
		{
		if(entry->type==TypeReal)
		{
		fprintf(f_assembly,".0\n");}correct = 0;}*/
		struct nodeType* rsib = node->child;
			//printf("%d",rsib->nodeType);
			struct paramlist* entrypara = entry->parameter;
		if(rsib == NULL)
		{
			fprintf(f_assembly,"invokestatic %s/%s(",classname,node->string);
			
			
			fprintf(f_assembly,")");
			
			fprintf(f_assembly,"V\n");
			return;
		}
		if(rsib->nodeType==NODE_EXPRESSIONLIST)   //檢查parameter
		{ 
			rsib = rsib->child;
			do{
				codegenerate(rsib);
					rsib = rsib->rsibling;	
					
					//printf("%d",tmppa->paramsize);
			}while(rsib!=node->child->child);  //printf("wwww%d %d",k,tmppa->paramsize);

			fprintf(f_assembly,"invokestatic %s/%s(",classname,node->string);
			for(int i=0;i<entrypara->paramsize;i++)
			{
			for(int j=0;j<entrypara->dimension[i];j++)
			fprintf(f_assembly,"[");
			fprintf(f_assembly,"%c",selectir(entrypara->para[i]));
			}
			
			fprintf(f_assembly,")");
			if(entry->dtype==proc)
			fprintf(f_assembly,"V\n");
			else
			fprintf(f_assembly,"%c\n",selectir(entry->type));
		}
		else if(rsib->nodeType==NODE_TAIL)
		{
				
			struct nodeType *tailNode = rsib;
			struct nodeType *exNode;
			//int yy = entry->val; 
			char c = selectir(entry->type)+32;
			//==============================================================================change
			int x=entry->range->d - node->arrdimension;
			struct paramlist* entrypara = entry->parameter;
			if(entry->dtype==func)
			{
				fprintf(f_assembly,"invokestatic %s/%s(",classname,node->string);
			for(int i=0;i<entrypara->paramsize;i++)
			{
			for(int j=0;j<entrypara->dimension[i];j++)
			fprintf(f_assembly,"[");
			fprintf(f_assembly,"%c",selectir(entrypara->para[i]));
			}
			
			fprintf(f_assembly,")");
			}
			else if(entry->scopenum==0)
			{
				fprintf(f_assembly,"getstatic %s/%s ",classname,entry->name);
				
				for(int i=0;i<entry->range->d;i++)
					fprintf(f_assembly,"[");
				fprintf(f_assembly,"%c\n",selectir(entry->type));
			}
			else
			{
				fprintf(f_assembly,"aload %d\n",entry->val); //?????????
			}
			
			int ori = leftright;
			leftright = 0;
			for(int i=0;i<x;i++)
			{
				exNode = nthChild(1,tailNode);  //expression
				tailNode = nthChild(2,tailNode);  //tail or lambda
				codegenerate(exNode);	        
						//printf("%d\n",exNode->nodeType);
				fprintf(f_assembly,"ldc %d\nisub\n",entry->range->rr[i][0]);
				if(i!=x-1)
					fprintf(f_assembly,"aaload\n");
					//printf("www%d %d\n",entry->range->rr[k][0],yy);	
				/*if((entry->range->rr[k][0]>yy||entry->range->rr[k][1]<yy)&&exNode->nodeType==NODE_INT)
				{
					node->valueType = typeno;
					printf("Error: iteration in array %s [] is out-of-range in line %d\n", node->string,node->line);
					return;
				}*/
			}
			if(ori == 0)
			{
				if(node->arrdimension == 0)
				{
					char c = selectir(entry->type)+32;
					fprintf(f_assembly,"%caload\n",c);
				}
				else
					fprintf(f_assembly,"aaload");
			}
			//==============================================================================change
			
					
				   //node->arrdimension = k;
				   
						
		}
			else  //node_lambda
			{
				/*if(entry->addtype == Array)
				{ 
					printf("Error: array %s doesn't have tail in line %d\n",node->string,node->line);
				  	return;
				} */ 
				//struct SymTableEntry *entry1 = findSymbol(node->string);
				if(entry->dtype==func||entry->dtype==proc)
				{
						fprintf(f_assembly,"invokestatic %s/%s()",classname,node->string);
						if(entry->dtype==func)
							fprintf(f_assembly,"%c\n",selectir(entry->type));
						else
							fprintf(f_assembly,"V\n");	
					//}
					//else {entry = entry1;}
				}
				else
				{
					
					if(entry->scopenum==0)
					{
						if(leftright == 1)
						{
							/*fprintf(f_assembly,"putstatic %s/%s ",classname,entry->name);
							for(int i=0;i<entry->range->d;i++)
							{
								fprintf(f_assembly,"[");
							}
								fprintf(f_assembly,"%c\n",selectir(entry->type));*/
						}
						else
						{
							
							fprintf(f_assembly,"getstatic %s/%s ",classname,entry->name);
							for(int i=0;i<entry->range->d;i++)
							{
								fprintf(f_assembly,"[");
							}
							fprintf(f_assembly,"%c\n",selectir(entry->type));
					
				
						}
					}
					else   //local variable
					{
						char c = selectir(entry->type)+32;
						if(leftright == 1)
						{
							/*fprintf(f_assembly,"%c",c);
							//if(entry->addtype==Array)
							//fprintf(f_assembly,"a");
							fprintf(f_assembly,"store %d\n",entry->val);*/
						}
						else
						{
							
							if(entry->addtype==Array)
							fprintf(f_assembly,"a");
							else
							fprintf(f_assembly,"%c",c);
							fprintf(f_assembly,"load %d\n",entry->val);
					
				
						}
					}
				}
				
				
			}     
            			return;
		}

        case NODE_INT: {
            
			fprintf(f_assembly,"ldc %d\n",node->iValue);
            return;
        }

        case NODE_REAL: {
            if(strlen(node->string)!=0)
		{
			fprintf(f_assembly,"ldc %s\n",node->string);
		}
		else	fprintf(f_assembly,"ldc %f\n",node->rValue);
            return;
        }
	case NODE_STRING: {
            
			fprintf(f_assembly,"ldc %s\n",node->string);
		            return;
        }
        case NODE_OP:
		{
			struct nodeType *child1 = nthChild(1, node);
            struct nodeType *child2 = nthChild(2, node);
			/*if(child1->nodeType == NODE_INT)
			correct =1;*/
			if(child1->nodeType == NODE_INT&&node->valueType == TypeReal)
			{
				fprintf(f_assembly,"ldc %d.0\n",child1->iValue);
			}
			else
			codegenerate(child1);
			//fprintf(f_assembly,".0")
			/*if(child2->nodeType==NODE_INT)
			correct ++;*/
			if(child2->nodeType == NODE_INT&&node->valueType == TypeReal)
			{
				fprintf(f_assembly,"ldc %d.0\n",child2->iValue);
			}
			else
            codegenerate(child2);
			char arr = 0;
				if(child1->arrdimension!=0/*nodeType==NODE_VAR_PROC&&child1->entry->addtype == Array*/)
				{
					arr = 'a';
				}
				char c = selectir(node->valueType) +32;
				if(ifwhile>0)
				{
					if(arr!=0)
						fprintf(f_assembly,"if_acmp");
					else
						fprintf(f_assembly,"if_%ccmp",c);
				}
				else 
				fprintf(f_assembly,"%c",c);
				switch(node->op)
				{
					case OP_ADD:
					fprintf(f_assembly,"add\n");
					break;
					case OP_SUB:
					fprintf(f_assembly,"sub\n");
					break;
					case OP_MUL:
					fprintf(f_assembly,"mul\n");
					break;
					case OP_DIV:
					fprintf(f_assembly,"div\n");
					break;
					case OP_GE:
					if(ifwhile==2)
						fprintf(f_assembly,"lt L%d\n",labelnum);
					else
						fprintf(f_assembly,"ge L%d\n",labelnum);
					labelnum+=2;
					break;
					case OP_GT:
					if(ifwhile==2)
						fprintf(f_assembly,"le L%d\n",labelnum);
					else
					fprintf(f_assembly,"gt L%d\n",labelnum);
					labelnum+=2;
					break;
					case OP_LE:
					if(ifwhile==2)
						fprintf(f_assembly,"gt L%d\n",labelnum);
					else
					fprintf(f_assembly,"le L%d\n",labelnum);
					labelnum+=2;
					break;
					case OP_LT:
					if(ifwhile==2)
						fprintf(f_assembly,"ge L%d\n",labelnum);
					else
					fprintf(f_assembly,"lt L%d\n",labelnum);
					labelnum+=2;
					break;
					case OP_NE:
					if(ifwhile==2)
						fprintf(f_assembly,"eq L%d\n",labelnum);
					else
					fprintf(f_assembly,"ne L%d\n",labelnum);
					labelnum+=2;
					break;
					case OP_EQ:
					if(ifwhile==2)
						fprintf(f_assembly,"ne L%d\n",labelnum);
					else
					fprintf(f_assembly,"eq L%d\n",labelnum);
					labelnum+=2;
					break;
					
				}
				//node->entry = child1->entry;
				return;
			
		}
        case NODE_ASSIGN_STATEMENT: 
		{
            struct nodeType *child1 = nthChild(1, node);           //symref
            struct nodeType *child2 = nthChild(2, node);           //expression
			leftright = 1;
			codegenerate(child1);
			leftright = 0;
			codegenerate(child2);  //VAR_PROC OR NODE_OP OR INT/REAL
			
			
		//printf("%d",child1->nodeType);
            
			//struct nodeType *child11 = child1;//symref id //printf("%d",child11->nodeType); 
			leftright = 0;
			//codegenerate(child11);
		//printf("%d ",child2->nodeType);
			
		/*if(child2->valueType == TypeInt)
		{
		//struct SymTableEntry *aa = findSymbol(child11->entry->name);
		//if(child11->entry->dtype==var&&aa->dtype==func)
		//aa->val = child2->iValue;
		child11->entry->val = child2->iValue;
		//printf("%s %d\n",child11->entry->name,child11->entry->val);
		}*/
		char c = selectir(child1->entry->type)+32;
		if(child1->entry->addtype==Array)
		{
			
			if(child1->arrdimension == child1->entry->range->d)
			{
				if(child1->entry->scopenum==0){
				fprintf(f_assembly,"putstatic %s/%s ",classname,child1->entry->name);
				for(int i=0;i<child1->arrdimension;i++)
				fprintf(f_assembly,"[");
				fprintf(f_assembly,"%c\n",selectir(child1->entry->type));}
				else
				fprintf(f_assembly,"astore %d\n",child1->entry->val);
			}
			else 
			{
				if(child1->arrdimension!=0){
				c='a';
				
				}
				fprintf(f_assembly,"%castore\n",c);
				/*if(child1->entry->scopenum==0)
				{
				fprintf(f_assembly,"%c",c);
				if(child1->arrdimension!=0)
				fprintf(f_assembly,"a");
				fprintf(f_assembly,"store\n");}
				else
				{
					if(child1->arrdimension!=0)
					c = 'a';
					fprintf(f_assembly,"%cstore %d\n",c,child1->entry->val);
				}*/
			}
		}
		else if(child1->entry->scopenum==0)
		{
			fprintf(f_assembly,"putstatic %s/%s %c\n",classname,child1->entry->name,selectir(child1->entry->type));
		}
		else 
		{
			fprintf(f_assembly,"%cstore %d\n",c,child1->entry->val);
		}
			
            return;
        }
		case NODE_IF_STATEMENT:
		{
			struct nodeType *ifnode = nthChild(1, node);           //expression
            struct nodeType *then = nthChild(2, node);           //statement
			struct nodeType *elsenode = nthChild(3, node);           //statement
			ifwhile = 1;
			codegenerate(ifnode);
			int thisiflabel = labelnum -2;
			ifwhile = 0;
			codegenerate(elsenode);
			
			fprintf(f_assembly,"goto L%d\n",thisiflabel+1);
			fprintf(f_assembly,"L%d:\n",thisiflabel);
			codegenerate(then);
			fprintf(f_assembly,"L%d:\n",thisiflabel+1);
			return;
		}
		case NODE_WHILE_STATEMENT:
		{
			struct nodeType *whilenode = nthChild(1, node);           //expression
            struct nodeType *donode = nthChild(2, node);           //statement
			ifwhile = 2;
			fprintf(f_assembly,"L%d:\n",labelnum);
			int thiswhilenode = labelnum;
			labelnum++;
			codegenerate(whilenode);
			labelnum--;
			ifwhile = 0;
			codegenerate(donode);
			fprintf(f_assembly,"goto L%d\nL%d:\n",thiswhilenode,thiswhilenode+1);			
			return;
		}

    }

    /* Default action for other nodes not listed in the switch-case */
    struct nodeType *child = node->child;
    if(child != 0) {
        do {
            codegenerate(child);
            child = child->rsibling;
        } while(child != node->child);
    }
}


