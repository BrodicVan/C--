#include "syntax.tab.h"
#include "all.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

// extern int strcpy_s();
extern int max_level;

struct Node *createNode(int r,int tag, char *text,int i,double d)
{
	struct Node *nd=(struct Node*) malloc(sizeof(struct Node));
	nd->kid_num=0;
	nd->tag=tag;
    nd->name=NULL;
    nd->row = r; 
    nd->if_empty = 0;
    nd->l_r = RIGHT;
    nd->type = (Type) malloc(sizeof(struct Type_));
    nd->inh_type = (Type) malloc(sizeof(struct Type_));
    nd->return_type = (Type) malloc(sizeof(struct Type_));
    nd->generation = Common_Generation;
    nd->id_type = VAR;
    nd->if_structspecifier = 0;
    nd->id_usage = DEF;

    switch(tag)
    {
        case INT:
            nd->value.type_int = i;// store int_value
            break;
        case FLOAT:
            nd->value.type_double = d;// store double_value
            break;
        case ID:
        case TYPE:
        case RELOP:
            nd->name=(char*)malloc(sizeof(char)*strlen(text));
            nd->name = (char*) malloc(strlen(text)+1);
            strcpy(nd->name,text);
            break;
        default:
            // nd->value.type_int=i;// store line_num
            break;
    }
	return nd;
}
 
void addNode(struct Node* fa,struct Node* kid)
{
    if(fa->kid_num>=10)
    {
        fprintf(stderr, "添加语法树子节点错误1\n");
        exit(1);
    }
    fa->kids[fa->kid_num] = kid;
    fa->kid_num++;
}
 
void addNodes(struct Node* fa, int num,...) {
    // step 1 
    va_list valist; 
    // step 2 
    int i; 
    va_start(valist, num); // step 3 
    struct Node* cur_kid;
    for (i = 0; i < num; i++) { 
        fa->kids[fa->kid_num] = va_arg(valist, struct Node*); // step 4 
        fa->kid_num++;
    } 
    va_end(valist); // step 5 

} 

void treePrintLevel(struct Node *nd)
{
    if(nd==NULL)
    {
        printf("null node");
        return;
    }
    if(nd->if_empty) 
    {
        return;
    }
    int level = nd->level;
    while(level>0)
    {
        printf("  ");
        level--;
    }
    char* token_name = (char*) malloc(sizeof(char)*20);
    switch(nd->tag)
    {
        case INT: token_name = "INT";break;
        case INT_ERR: token_name = "INT_ERR";break;
        case FLOAT: token_name = "FLOAT";break;
        case FLOAT_ERR: token_name = "FLOAT_ERR";break;
        case ID: token_name = "ID";break;
        case SEMI: token_name = "SEMI";break;
        case COMMA: token_name = "COMMA";break;
        case TYPE: token_name = "TYPE";break;
        case LC: token_name = "LC";break;
        case RC: token_name = "RC";break;
        case STRUCT: token_name = "STRUCT";break;
        case RETURN: token_name = "RETURN";break;
        case IF: token_name = "IF";break;
        case ELSE: token_name = "ELSE";break;
        case WHILE: token_name = "WHILE";break;
        case ASSIGNOP: token_name = "ASSIGNOP";break;
        case OR: token_name = "OR";break;
        case AND: token_name = "AND";break;
        case RELOP: token_name = "RELOP";break;
        case MINUS: token_name = "MINUS";break;
        case PLUS : token_name = "PLUS";break;
        case STAR: token_name = "STAR";break;
        case DIV: token_name = "DIV";break;
        case NOT: token_name = "NOT";break;
        case LP: token_name = "LP";break;
        case RP: token_name = "RP";break;
        case LB: token_name = "LB";break;
        case RB: token_name = "RB";break;
        case DOT: token_name = "DOT";break;
        case Program: token_name = "Program";break;
        case ExtDecList: token_name = "ExtDecList";break;
        case ExtDef: token_name = "ExtDef";break;
        case ExtDefList: token_name = "ExtDefList";break;
        case Specifier: token_name = "Specifier";break;
        case StructSpecifier: token_name = "StructSpecifier";break;
        case OptTag: token_name = "OptTag";break;
        case Tag: token_name = "Tag";break;
        case VarDec: token_name = "VarDec";break;
        case FunDec: token_name = "FunDec";break;
        case VarList: token_name = "VarList";break;
        case ParamDec: token_name = "ParamDec";break;
        case CompSt: token_name = "CompSt";break;
        case StmtList: token_name = "StmtList";break;
        case Stmt: token_name = "Stmt";break;
        case DefList: token_name = "DefList";break;
        case Def: token_name = "Def";break;
        case Exp: token_name = "Exp";break;
        case Args: token_name = "Args";break;
        case DecList: token_name = "DecList";break;
        case Dec: token_name = "Dec";break;
        default: token_name = "error";break;
    }

                        

    switch(nd->tag)
    {
        case INT:
            printf("%s: %d",token_name,nd->value.type_int);
            
            break;
        case INT_ERR:
        case FLOAT_ERR:
            printf("%s: NaN",token_name);
            break;
        case FLOAT:
            printf("%s: %lf",token_name,nd->value.type_double);
            break;
        case ID:
            printf("%s: %s",token_name,nd->name);
            break;
        case TYPE:
            printf("%s: %s",token_name,nd->name);
            break;
        case Program: 
        case ExtDecList: 
        case ExtDef: 
        case ExtDefList:
        case Specifier:
        case StructSpecifier: 
        case OptTag: 
        case Tag: 
        case VarDec:
        case FunDec:
        case VarList:
        case ParamDec:
        case CompSt: 
        case StmtList:
        case Stmt: 
        case DefList: 
        case Def: 
        case Exp: 
        case Args: 
        case DecList: 
        case Dec: 
            printf("%s (%d)",token_name,nd->row);
            // printf(" kid_num: %d",nd->kid_num);
            break;
        default:
            printf("%s",token_name);
            break;
    }
    
    printf("\n");
}
 
struct Node* stack[500];// top is null
void stack_print(struct Node *nd)
{
    if(nd==NULL)
    {
        // printf("poniter is null\n");
        fprintf(stderr, "Fail building syntax tree\n");
        return;
    }

    
    int top = 0;
    
    stack[top] = nd;
    top++;
    struct Node* cur;
    int i,kid_num;
    nd->level = 0;
    int cur_level;
    while(top>0)
    {
        cur = stack[top-1];
        kid_num = cur->kid_num;
        treePrintLevel(cur);
        cur_level = cur->level;
        
        top--;// pop
        
        // printf("%d\n",kid_num);
        for( i = kid_num-1; i >= 0; i--)
        {
            if(cur->kids[i]->if_empty)
            {
                continue;
            }
            cur->kids[i]->level = cur_level+1; 
            stack[top] = cur->kids[i];// push
            top++;
        }
    }
}

void r_p(struct Node *nd)
{
    if(nd==NULL)
    {
        printf("poniter is null\n");
        return;
    }
    int cur_level = nd->level;
    treePrintLevel(nd);
    int kid_num = nd->kid_num;
    int i;
    for( i = 0; i < kid_num; i++)
    {
        nd->kids[i]->level = cur_level+1; 
        r_p(nd->kids[i]);
    }
}

void recursion_print(struct Node *nd)
{
    if(nd==NULL)
    {
        fprintf(stderr, "Fail building syntax tree\n");
        // printf("poniter is null\n");
        return;
    }
    nd->level = 0;
    r_p(nd);
}
