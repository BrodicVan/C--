
%{
    #include <stdio.h>
    #include "tree.h"
    extern int yylex();
    extern void my_yyerror(char* msg,int line);
    extern void yyerror(char* msg);
    extern struct Node* top;
    extern int yylineno;
    int fail = 0;
    
%}

%union
{
    struct Node *type_node;
}


%start Program
%locations

%token <type_node> INT;
%token  <type_node> FLOAT;
%token <type_node> ID SEMI COMMA   
%token <type_node> TYPE
%token <type_node> LC RC
%token <type_node> STRUCT RETURN IF ELSE WHILE



%right <type_node> ASSIGNOP
%left <type_node> OR
%left <type_node> AND
%left <type_node> RELOP
%left <type_node> PLUS MINUS
%left <type_node> STAR DIV
%right <type_node> NOT
%left <type_node> LP RP LB RB DOT
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%type<type_node> Program ExtDecList ExtDef ExtDefList Specifier StructSpecifier OptTag Tag VarDec FunDec VarList ParamDec CompSt StmtList Stmt DefList Def Exp Args DecList Dec

%%
Exp :Exp ASSIGNOP Exp {$$ = createNode($1->row,Exp,NULL,0,0);$$->kid_num = 3;$$->kids[0] = $1;$$->kids[1] = $2;$$->kids[2] = $3;}
| Exp AND Exp {$$ = createNode($1->row,Exp,NULL,0,0);$$->kid_num = 3;$$->kids[0] = $1;$$->kids[1] = $2;$$->kids[2] = $3;}
| Exp OR Exp {$$ = createNode($1->row,Exp,NULL,0,0);$$->kid_num = 3;$$->kids[0] = $1;$$->kids[1] = $2;$$->kids[2] = $3;}
| Exp RELOP Exp {$$ = createNode($1->row,Exp,NULL,0,0);$$->kid_num = 3;$$->kids[0] = $1;$$->kids[1] = $2;$$->kids[2] = $3;}
| Exp PLUS Exp {$$ = createNode($1->row,Exp,NULL,0,0);$$->kid_num = 3;$$->kids[0] = $1;$$->kids[1] = $2;$$->kids[2] = $3;}
| Exp MINUS Exp {$$ = createNode($1->row,Exp,NULL,0,0);$$->kid_num = 3;$$->kids[0] = $1;$$->kids[1] = $2;$$->kids[2] = $3;}
| Exp STAR Exp {$$ = createNode($1->row,Exp,NULL,0,0);$$->kid_num = 3;$$->kids[0] = $1;$$->kids[1] = $2;$$->kids[2] = $3;}
| Exp DIV Exp {$$ = createNode($1->row,Exp,NULL,0,0);$$->kid_num = 3;$$->kids[0] = $1;$$->kids[1] = $2;$$->kids[2] = $3;}
| LP Exp RP {$$ = createNode($1->row,Exp,NULL,0,0);$$->kid_num = 3;$$->kids[0] = $1;$$->kids[1] = $2;$$->kids[2] = $3;}
| MINUS Exp {$$ = createNode($1->row,Exp,NULL,0,0);$$->kid_num = 2;$$->kids[0] = $1;$$->kids[1] = $2;}
| NOT Exp {$$ = createNode($1->row,Exp,NULL,0,0);$$->kid_num = 2;$$->kids[0] = $1;$$->kids[1] = $2;}
| ID LP Args RP {$$ = createNode($1->row,Exp,NULL,0,0);$$->kid_num = 4;$$->kids[0] = $1;$$->kids[1] = $2;$$->kids[2] = $3;$$->kids[3] = $4;}
| ID LP RP {$$ = createNode($1->row,Exp,NULL,0,0);$$->kid_num = 3;$$->kids[0] = $1;$$->kids[1] = $2;$$->kids[2] = $3;}
| Exp LB Exp RB {$$ = createNode($1->row,Exp,NULL,0,0);$$->kid_num = 4;$$->kids[0] = $1;$$->kids[1] = $2;$$->kids[2] = $3;$$->kids[3] = $4;}
| Exp DOT ID {$$ = createNode($1->row,Exp,NULL,0,0);$$->kid_num = 3;$$->kids[0] = $1;$$->kids[1] = $2;$$->kids[2] = $3;}
| ID {$$ = createNode($1->row,Exp,NULL,0,0);$$->kid_num = 1;$$->kids[0] = $1;}
| INT {$$ = createNode($1->row,Exp,NULL,0,0);$$->kid_num = 1;$$->kids[0] = $1;}
| FLOAT {$$ = createNode($1->row,Exp,NULL,0,0);$$->kid_num = 1;$$->kids[0] = $1;}
;

Program :ExtDefList {$$ = createNode($1->row,Program,NULL,0,0);$$->kid_num = 1;$$->kids[0] = $1;top = $$;}
;

ExtDefList : {$$=createNode(0,ExtDefList,NULL,0,0);$$->if_empty=1;}
| ExtDef ExtDefList {$$ = createNode($1->row,ExtDefList,NULL,0,0);$$->kid_num = 2;$$->kids[0] = $1;$$->kids[1] = $2;}
;

ExtDef :Specifier ExtDecList SEMI {$$ = createNode($1->row,ExtDef,NULL,0,0);$$->kid_num = 3;$$->kids[0] = $1;$$->kids[1] = $2;$$->kids[2] = $3;}

| Specifier SEMI {$$ = createNode($1->row,ExtDef,NULL,0,0);$$->kid_num = 2;$$->kids[0] = $1;$$->kids[1] = $2;}

| Specifier FunDec CompSt  {$$ = createNode($1->row,ExtDef,NULL,0,0);$$->kid_num = 3;$$->kids[0] = $1;$$->kids[1] = $2;$$->kids[2] = $3;}

ExtDecList :VarDec {$$ = createNode($1->row,ExtDecList,NULL,0,0);$$->kid_num = 1;$$->kids[0] = $1;}
| VarDec COMMA ExtDecList {$$ = createNode($1->row,ExtDefList,NULL,0,0);$$->kid_num = 3;$$->kids[0] = $1;$$->kids[1] = $2;$$->kids[2] = $3;}
;



Specifier :TYPE {$$ = createNode($1->row,Specifier,NULL,0,0);$$->kid_num = 1;$$->kids[0] = $1;}
| StructSpecifier {$$ = createNode($1->row,Specifier,NULL,0,0);$$->kid_num = 1;$$->kids[0] = $1;}
;

StructSpecifier :STRUCT OptTag LC DefList RC {$$ = createNode($1->row,StructSpecifier,NULL,0,0);$$->kid_num = 5;$$->kids[0] = $1;$$->kids[1] = $2;$$->kids[2] = $3;$$->kids[3] = $4;$$->kids[4] = $5;}                                                    
| STRUCT Tag {$$ = createNode($1->row,StructSpecifier,NULL,0,0);$$->kid_num = 2;$$->kids[0] = $1;$$->kids[1] = $2;}
;

OptTag : {$$=createNode(0,OptTag,NULL,0,0);$$->if_empty=1;}
| ID {$$ = createNode($1->row,OptTag,NULL,0,0);$$->kid_num = 1;$$->kids[0] = $1;}
;

Tag :ID {$$ = createNode($1->row,Tag,NULL,0,0);$$->kid_num = 1;$$->kids[0] = $1;}
;



VarDec :ID {$$ = createNode($1->row,VarDec,NULL,0,0);$$->kid_num = 1;$$->kids[0] = $1;}
| VarDec LB INT RB {$$ = createNode($1->row,VarDec,NULL,0,0);$$->kid_num = 4;$$->kids[0] = $1;$$->kids[1] = $2;$$->kids[2] = $3;$$->kids[3] = $4;}

;

FunDec :ID LP VarList RP {$$ = createNode($1->row,FunDec,NULL,0,0);$$->kid_num = 4;$$->kids[0] = $1;$$->kids[1] = $2;$$->kids[2] = $3;$$->kids[3] = $4;}
| ID LP RP {$$ = createNode($1->row,FunDec,NULL,0,0);$$->kid_num = 3;$$->kids[0] = $1;$$->kids[1] = $2;$$->kids[2] = $3;}
;

VarList :ParamDec COMMA VarList {$$ = createNode($1->row,VarList,NULL,0,0);$$->kid_num = 3;$$->kids[0] = $1;$$->kids[1] = $2;$$->kids[2] = $3;}
| ParamDec {$$ = createNode($1->row,VarList,NULL,0,0);$$->kid_num = 1;$$->kids[0] = $1;}
;

ParamDec :Specifier VarDec {$$ = createNode($1->row,ParamDec,NULL,0,0);$$->kid_num = 2;$$->kids[0] = $1;$$->kids[1] = $2;}
;



CompSt :LC DefList StmtList RC {$$ = createNode($1->row,CompSt,NULL,0,0);$$->kid_num = 4;$$->kids[0] = $1;$$->kids[1] = $2;$$->kids[2] = $3;$$->kids[3] = $4;}
;

StmtList : {$$=createNode(0,StmtList,NULL,0,0);$$->if_empty=1;}
| Stmt StmtList {$$ = createNode($1->row,StmtList,NULL,0,0);$$->kid_num = 2;$$->kids[0] = $1;$$->kids[1] = $2;}
;

Stmt :Exp SEMI {$$ = createNode($1->row,Stmt,NULL,0,0);$$->kid_num = 2;$$->kids[0] = $1;$$->kids[1] = $2;}
| CompSt {$$ = createNode($1->row,Stmt,NULL,0,0);$$->kid_num = 1;$$->kids[0] = $1;}
| RETURN Exp SEMI {$$ = createNode($1->row,Stmt,NULL,0,0);$$->kid_num = 3;$$->kids[0] = $1;$$->kids[1] = $2;$$->kids[2] = $3;}

| IF LP Exp RP Stmt %prec LOWER_THAN_ELSE {$$ = createNode($1->row,Stmt,NULL,0,0);$$->kid_num = 5;$$->kids[0] = $1;$$->kids[1] = $2;$$->kids[2] = $3;$$->kids[3] = $4;$$->kids[4] = $5;}
| IF LP Exp RP Stmt ELSE Stmt {$$ = createNode($1->row,Stmt,NULL,0,0);$$->kid_num = 7;$$->kids[0] = $1;$$->kids[1] = $2;$$->kids[2] = $3;$$->kids[3] = $4;$$->kids[4] = $5;$$->kids[5] = $6;$$->kids[6] = $7;}
| WHILE LP Exp RP Stmt {$$ = createNode($1->row,Stmt,NULL,0,0);$$->kid_num = 5;$$->kids[0] = $1;$$->kids[1] = $2;$$->kids[2] = $3;$$->kids[3] = $4;$$->kids[4] = $5;} 
;



DefList : {$$=createNode(0,DefList,NULL,0,0);$$->if_empty=1;}
| Def DefList {$$ = createNode($1->row,DefList,NULL,0,0);$$->kid_num = 2;$$->kids[0] = $1;$$->kids[1] = $2;}
;

Def :Specifier DecList SEMI {$$ = createNode($1->row,Def,NULL,0,0);$$->kid_num = 3;$$->kids[0] = $1;$$->kids[1] = $2;$$->kids[2] = $3;}
;

DecList :Dec {$$ = createNode($1->row,DecList,NULL,0,0);$$->kid_num = 1;$$->kids[0] = $1;}
| Dec COMMA DecList {$$ = createNode($1->row,DefList,NULL,0,0);$$->kid_num = 3;$$->kids[0] = $1;$$->kids[1] = $2;$$->kids[2] = $3;}
;

Dec :VarDec {$$ = createNode($1->row,Dec,NULL,0,0);$$->kid_num = 1;$$->kids[0] = $1;}
| VarDec ASSIGNOP Exp {$$ = createNode($1->row,Dec,NULL,0,0);$$->kid_num = 3;$$->kids[0] = $1;$$->kids[1] = $2;$$->kids[2] = $3;}
;





Args :Exp COMMA Args {$$ = createNode($1->row,Args,NULL,0,0);$$->kid_num = 3;$$->kids[0] = $1;$$->kids[1] = $2;$$->kids[2] = $3;}
| Exp {$$ = createNode($1->row,Args,NULL,0,0);$$->kid_num = 1;$$->kids[0] = $1;}
;


%%
