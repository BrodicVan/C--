
%{
    #include <stdio.h>
    #include "tree.h"
    extern int yylex();
    void yyerror(const char* msg);
    void my_yyerror(const char* msg);
    extern struct Node* top;
    extern int yylineno;
    extern char* yytext;
    int fail = 0;
    
%}

%union
{
    struct Node *type_node;
}


%start Program
%locations

%define parse.error verbose

%token <type_node> INT INT_ERR;
%token  <type_node> FLOAT FLOAT_ERR;
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
%nonassoc LOWER_THAN_RB

%type<type_node> Program ExtDecList ExtDef ExtDefList Specifier StructSpecifier OptTag Tag VarDec FunDec VarList ParamDec CompSt StmtList Stmt DefList Def Exp Args DecList Dec

%%


Program :ExtDefList {$$ = createNode($1->row,Program,NULL,0,0);$$->kid_num = 1;$$->kids[0] = $1;top = $$;}
;

ExtDefList : {$$=createNode(0,ExtDefList,NULL,0,0);$$->if_empty=1;}
| ExtDef ExtDefList {$$ = createNode($1->row,ExtDefList,NULL,0,0);$$->kid_num = 2;$$->kids[0] = $1;$$->kids[1] = $2;}
;

ExtDef :Specifier ExtDecList SEMI {$$ = createNode($1->row,ExtDef,NULL,0,0);$$->kid_num = 3;$$->kids[0] = $1;$$->kids[1] = $2;$$->kids[2] = $3;}
| Specifier SEMI {$$ = createNode($1->row,ExtDef,NULL,0,0);$$->kid_num = 2;$$->kids[0] = $1;$$->kids[1] = $2;}
| Specifier FunDec CompSt  {$$ = createNode($1->row,ExtDef,NULL,0,0);$$->kid_num = 3;$$->kids[0] = $1;$$->kids[1] = $2;$$->kids[2] = $3;}
| Specifier error SEMI {my_yyerror("error ExtDef");}
| error SEMI {my_yyerror("error ExtDef");}
| Specifier error {my_yyerror("error ExtDef");}
;


ExtDecList :VarDec {$$ = createNode($1->row,ExtDecList,NULL,0,0);$$->kid_num = 1;$$->kids[0] = $1;}
| VarDec COMMA ExtDecList {$$ = createNode($1->row,ExtDefList,NULL,0,0);$$->kid_num = 3;$$->kids[0] = $1;$$->kids[1] = $2;$$->kids[2] = $3;}
| VarDec error COMMA ExtDecList {my_yyerror("error ExtDecList");}
;



Specifier :TYPE {$$ = createNode($1->row,Specifier,NULL,0,0);$$->kid_num = 1;$$->kids[0] = $1;}
| StructSpecifier {$$ = createNode($1->row,Specifier,NULL,0,0);$$->kid_num = 1;$$->kids[0] = $1;}
;

StructSpecifier :STRUCT OptTag LC DefList RC {$$ = createNode($1->row,StructSpecifier,NULL,0,0);$$->kid_num = 5;$$->kids[0] = $1;$$->kids[1] = $2;$$->kids[2] = $3;$$->kids[3] = $4;$$->kids[4] = $5;}                                                    
| STRUCT Tag {$$ = createNode($1->row,StructSpecifier,NULL,0,0);$$->kid_num = 2;$$->kids[0] = $1;$$->kids[1] = $2;}
| STRUCT error LC DefList RC {my_yyerror("error StructSpecifier");}
| STRUCT OptTag LC error RC {my_yyerror("error StructSpecifier");}
| STRUCT OptTag LC error {my_yyerror("error StructSpecifier");}
| STRUCT error {my_yyerror("error StructSpecifier");}

;

OptTag : {$$=createNode(0,OptTag,NULL,0,0);$$->if_empty=1;}
| ID {$$ = createNode($1->row,OptTag,NULL,0,0);$$->kid_num = 1;$$->kids[0] = $1;}
;

Tag :ID {$$ = createNode($1->row,Tag,NULL,0,0);$$->kid_num = 1;$$->kids[0] = $1;}
;



VarDec :ID {$$ = createNode($1->row,VarDec,NULL,0,0);$$->kid_num = 1;$$->kids[0] = $1;}
| VarDec LB INT RB {$$ = createNode($1->row,VarDec,NULL,0,0);$$->kid_num = 4;$$->kids[0] = $1;$$->kids[1] = $2;$$->kids[2] = $3;$$->kids[3] = $4;}
| VarDec LB INT_ERR RB {$$ = createNode($1->row,VarDec,NULL,0,0);$$->kid_num = 4;$$->kids[0] = $1;$$->kids[1] = $2;$$->kids[2] = $3;$$->kids[3] = $4;} 
| VarDec LB error {my_yyerror("error VarDec");}
;

FunDec :ID LP VarList RP {$$ = createNode($1->row,FunDec,NULL,0,0);$$->kid_num = 4;$$->kids[0] = $1;$$->kids[1] = $2;$$->kids[2] = $3;$$->kids[3] = $4;}
| ID LP RP {$$ = createNode($1->row,FunDec,NULL,0,0);$$->kid_num = 3;$$->kids[0] = $1;$$->kids[1] = $2;$$->kids[2] = $3;}
| ID LP error RP {my_yyerror("error FunDec");}
| ID LP error {my_yyerror("error FunDec");}
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
| Exp error {my_yyerror("missing ';'");}
| CompSt {$$ = createNode($1->row,Stmt,NULL,0,0);$$->kid_num = 1;$$->kids[0] = $1;}
| RETURN Exp SEMI {$$ = createNode($1->row,Stmt,NULL,0,0);$$->kid_num = 3;$$->kids[0] = $1;$$->kids[1] = $2;$$->kids[2] = $3;}
| IF LP Exp RP Stmt %prec LOWER_THAN_ELSE {$$ = createNode($1->row,Stmt,NULL,0,0);$$->kid_num = 5;$$->kids[0] = $1;$$->kids[1] = $2;$$->kids[2] = $3;$$->kids[3] = $4;$$->kids[4] = $5;}
| IF LP Exp RP Stmt ELSE Stmt {$$ = createNode($1->row,Stmt,NULL,0,0);$$->kid_num = 7;$$->kids[0] = $1;$$->kids[1] = $2;$$->kids[2] = $3;$$->kids[3] = $4;$$->kids[4] = $5;$$->kids[5] = $6;$$->kids[6] = $7;}
| WHILE LP Exp RP Stmt {$$ = createNode($1->row,Stmt,NULL,0,0);$$->kid_num = 5;$$->kids[0] = $1;$$->kids[1] = $2;$$->kids[2] = $3;$$->kids[3] = $4;$$->kids[4] = $5;} 
| error SEMI {my_yyerror("error Stmt");}
| IF LP error RP Stmt %prec LOWER_THAN_ELSE {my_yyerror("error IF Stmt");}
| IF LP Exp RP error ELSE Stmt {my_yyerror("error IF Stmt");}
| IF LP error RP ELSE Stmt {my_yyerror("error IF Stmt");}
| error LP Exp RP Stmt {my_yyerror("error Stmt");}
;



DefList : {$$=createNode(0,DefList,NULL,0,0);$$->if_empty=1;}
| Def DefList {$$ = createNode($1->row,DefList,NULL,0,0);$$->kid_num = 2;$$->kids[0] = $1;$$->kids[1] = $2;}
;

Def :Specifier DecList SEMI {$$ = createNode($1->row,Def,NULL,0,0);$$->kid_num = 3;$$->kids[0] = $1;$$->kids[1] = $2;$$->kids[2] = $3;}
;

DecList :Dec {$$ = createNode($1->row,DecList,NULL,0,0);$$->kid_num = 1;$$->kids[0] = $1;}
| Dec COMMA DecList {$$ = createNode($1->row,DefList,NULL,0,0);$$->kid_num = 3;$$->kids[0] = $1;$$->kids[1] = $2;$$->kids[2] = $3;}
| Dec error DecList {my_yyerror("error DecList");}
;

Dec :VarDec {$$ = createNode($1->row,Dec,NULL,0,0);$$->kid_num = 1;$$->kids[0] = $1;}
| VarDec ASSIGNOP Exp {$$ = createNode($1->row,Dec,NULL,0,0);$$->kid_num = 3;$$->kids[0] = $1;$$->kids[1] = $2;$$->kids[2] = $3;}
| error ASSIGNOP Exp {my_yyerror("error Dec");}
;



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
| INT_ERR {$$ = createNode($1->row,Exp,NULL,0,0);$$->kid_num = 1;$$->kids[0] = $1;}
| FLOAT {$$ = createNode($1->row,Exp,NULL,0,0);$$->kid_num = 1;$$->kids[0] = $1;}
| FLOAT_ERR {$$ = createNode($1->row,Exp,NULL,0,0);$$->kid_num = 1;$$->kids[0] = $1;}

| Exp ASSIGNOP error {my_yyerror("error Exp");}
| Exp AND error {my_yyerror("error Exp");}
| Exp OR error {my_yyerror("error Exp");}
| Exp RELOP error {my_yyerror("error Exp");}
| Exp PLUS error {my_yyerror("error Exp");}
| Exp MINUS error {my_yyerror("error Exp");}
| Exp STAR error {my_yyerror("error Exp");}
| Exp DIV error {my_yyerror("error Exp");}
| ID LP error RP {my_yyerror("error Exp");}
| Exp LB error RB {my_yyerror("error Exp");}
;

Args :Exp COMMA Args {$$ = createNode($1->row,Args,NULL,0,0);$$->kid_num = 3;$$->kids[0] = $1;$$->kids[1] = $2;$$->kids[2] = $3;}
| Exp {$$ = createNode($1->row,Args,NULL,0,0);$$->kid_num = 1;$$->kids[0] = $1;}
;


%%

void yyerror(const char* msg)
{
    fail = 1;
    extern int yylineno;
	extern char *yytext;
	// int len=strlen(yytext);
	// int i;
	// char buf[512]={0};
	// for (i=0;i<len;++i)
	// {
	// 	sprintf(buf,"%s%d ",buf,yytext[i]);
	// }
	fprintf(stderr, "Error type B at line %d: %s around symbol '%s'\n", yylineno, msg, yytext);
}

void my_yyerror(const char* msg)
{
    fail = 1;
    extern int yylineno;
	extern char *yytext;
	// int len=strlen(yytext);
	// int i;
	// char buf[512]={0};
	// for (i=0;i<len;++i)
	// {
	// 	sprintf(buf,"%s%d ",buf,yytext[i]);
	// }
	fprintf(stderr, "Error type B at line %d: %s around symbol '%s'\n", yylineno, msg, yytext);
}