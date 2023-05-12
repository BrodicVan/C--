%{
#include <stdio.h>
#include "syntax.tab.h"
#include "tree_semantics.h"
int yycolumn = 1;
#define YY_USER_ACTION  \
    yylloc.first_line = yylloc.last_line = yylineno;\
    yylloc.first_column = yycolumn; \
    yylloc.last_column = yycolumn+yyleng;\
    yycolumn += yyleng;
extern int fail;

%}

%option yylineno


delim [ \r\t]
whitespace {delim}+
letter [A-Za-z_]
digit [0-9]


COMMENT_SINGLE \/\/[^\n]*
COMMENT_MULTI "/*"([^\*]|(\*)*[^\*\/])*(\*)*"*/"
COMMENT_INCOMPLETE "/*"([^\*]|(\*)*[^\*\/])*(\*)*
DEC 0|[1-9]{digit}*
OCT 0[0-7]+
OCT_ERR 0[0-9a-wyzA-WYZ][0-9a-zA-Z]*
HEX 0[xX]([0-9A-Fa-f])+ 
HEX_ERR 0[xX][0-9A-Za-z]+
FLOAT {digit}+\.{digit}+|{digit}+\.{digit}*([eE][+-]?{digit}+)?
FLOAT_ERR {digit}+\.{digit}*[eE][+-]?
ID  {letter}({letter}|{digit})*
RELOP ==|>=|<=|!=|>|<

TYPE int|float


%%

{COMMENT_SINGLE} {
    // printf("COMMENT_SINGLE:\n%s\n",yytext);
    // printf("%d\n",yylloc.first_line);
}
{COMMENT_MULTI} {
    int len = strlen(yytext);
    for(int i = 2; i < len-1; i++)
    {
        if(yytext[i]=='*' && yytext[i+1]=='/' && i!=len-2)
        {
            printf("error_multi_line:line%d:\n",yylineno);
            exit(1);
        }
    }
    // printf("COMMENT_MULTI:\n%s\n",yytext);
}

{COMMENT_INCOMPLETE} {printf("Error type B at Line %d: incomplete comment\n",yylineno);}

{delim} {}


{TYPE} {
    yylval.type_node = createNode(yylineno,TYPE,yytext,0,0);
    
    return TYPE;
    }
{RELOP} {yylval.type_node = createNode(yylineno,RELOP,yytext,0,0);return RELOP;}
"\n" {yycolumn=1;}
";" {yylval.type_node = createNode(yylineno,SEMI,NULL,yylineno,0);return SEMI;}
"," {yylval.type_node = createNode(yylineno,COMMA,NULL,yylineno,0);return COMMA;}
"=" {yylval.type_node = createNode(yylineno,ASSIGNOP,NULL,yylineno,0);return ASSIGNOP;}

"+" {yylval.type_node = createNode(yylineno,PLUS,NULL,yylineno,0);return PLUS;}
"-" {yylval.type_node = createNode(yylineno,MINUS,NULL,yylineno,0);return MINUS;}
"*" {yylval.type_node = createNode(yylineno,STAR,NULL,yylineno,0);return STAR;}
"/" {yylval.type_node = createNode(yylineno,DIV,NULL,yylineno,0);return DIV;}
"&&" {yylval.type_node = createNode(yylineno,AND,NULL,yylineno,0);return AND;}
"||" {yylval.type_node = createNode(yylineno,OR,NULL,yylineno,0);return OR;}
"." {yylval.type_node = createNode(yylineno,DOT,NULL,yylineno,0);return DOT;}
"!" {yylval.type_node = createNode(yylineno,NOT,NULL,yylineno,0);return NOT;}

"(" {yylval.type_node = createNode(yylineno,LP,NULL,yylineno,0);return LP;}
")" {yylval.type_node = createNode(yylineno,RP,NULL,yylineno,0);return RP;}
"[" {yylval.type_node = createNode(yylineno,LB,NULL,yylineno,0);return LB;}
"]" {yylval.type_node = createNode(yylineno,RB,NULL,yylineno,0);return RB;}
"{" {yylval.type_node = createNode(yylineno,LC,NULL,yylineno,0);return LC;}
"}" {yylval.type_node = createNode(yylineno,RC,NULL,yylineno,0);return RC;}
"struct" {yylval.type_node = createNode(yylineno,STRUCT,NULL,yylineno,0);return STRUCT;}
"return" {yylval.type_node = createNode(yylineno,RETURN,NULL,yylineno,0);return RETURN;}
"if" {yylval.type_node = createNode(yylineno,IF,NULL,yylineno,0);return IF;}
"else" {yylval.type_node = createNode(yylineno,ELSE,NULL,yylineno,0);return ELSE;}
"while" {yylval.type_node = createNode(yylineno,WHILE,NULL,yylineno,0);return WHILE;}



{OCT} {
    int len = strlen(yytext);
    int sum = 0;
    for(int i = 1; i < len;i++)
    {
        sum += sum*8 + yytext[i] - '0';
    }
    yylval.type_node = createNode(yylineno,INT,NULL,sum,0);
    Type t = (Type) malloc(sizeof(struct Type_));
    yylval.type_node->type = t;
    t->kind = BASIC;
    t->u.basic = INT;
    // printf("OCT\n");
    return INT;
}

{OCT_ERR} {
    printf("Error type A at Line %d: illegal OCT number \"%s\"\n",yylineno,yytext);
    fail = 1;
    yylval.type_node = createNode(yylineno,INT_ERR,NULL,0,0);return INT_ERR;
}


{HEX} {
    int len = strlen(yytext);
    int sum = 0;
    char c;
    for(int i = 2; i < len;i++)
    {
        c = yytext[i];
        if(c>='0'&&c<='9')
        {
            sum += sum*16 + yytext[i] - '0';
        }
        else if(c>='a'&&c<='f')
        {
            sum += sum*16 + yytext[i] - 'a' + 10;
        }
        else if(c>='A'&&c<='F')
        {
            sum += sum*16 + yytext[i] - 'A' + 10;
        }
    }
    yylval.type_node = createNode(yylineno,INT,NULL,sum,0);
    Type t = (Type) malloc(sizeof(struct Type_));
    yylval.type_node->type = t;
    t->kind = BASIC;
    t->u.basic = INT;
    return INT;
}

{HEX_ERR} {
    printf("Error type A at Line %d: illegal HEX number \"%s\"\n",yylineno,yytext);
    fail = 1;
    yylval.type_node = createNode(yylineno,INT_ERR,NULL,0,0);return INT_ERR;
}

{DEC} {
    yylval.type_node = createNode(yylineno,INT,NULL, atoi(yytext),0);
    // yylval.type_int = atoi(yytext);
    Type t = (Type) malloc(sizeof(struct Type_));
    yylval.type_node->type = t;
    t->kind = BASIC;
    t->u.basic = INT;

    return INT;
    // return createNode(yylineno,INT,NULL, atoi(yytext),0);
}



{FLOAT} {   
    yylval.type_node = createNode(yylineno,FLOAT,NULL,0,strtof(yytext,NULL));
    Type t = (Type) malloc(sizeof(struct Type_));
    yylval.type_node->type = t;
    t->kind = BASIC;
    t->u.basic = FLOAT;
    // yylval.type_double = strtof(yytext,NULL);
    return FLOAT;
    // return createNode(yylineno,INT,NULL,0,strtof(yytext,NULL));
}

{FLOAT_ERR} {
    printf("Error type A at Line %d: illegal FLOAT number \"%s\"\n",yylineno,yytext);
    fail = 1;
    yylval.type_node = createNode(yylineno,FLOAT_ERR,NULL,0,0);return INT_ERR;
}

{ID} {
    yylval.type_node = createNode(yylineno,ID,yytext,0,0);
    // printf("ID: %s\n",yytext);
    return ID;
}


. {
    fail = 1;
    printf("Error type A at Line %d: Mysterious characters \"%s\"\n",yylineno,yytext);
    exit(0);
}
%%

// ";" {printf("SEMI\n");}
// "," {printf("COMMA\n");}
// "=" {printf("ASSIGNOP\n");}

// "+" {printf("PLUS\n");}
// "-" {printf("MINUS\n");}
// "*" {printf("STAR\n");}
// "/" {printf("DIV\n");}
// "&&" {printf("AND\n");}
// "||" {printf("OR\n");}
// "." {printf("DOT\n");}
// "!" {printf("NOT\n");}

// "(" {printf("LP\n");}
// ")" {printf("RP\n");}
// "[" {printf("LB\n");}
// "]" {printf("RB\n");}
// "{" {printf("LC\n");}
// "}" {printf("RC\n");}
// "struct" {printf("STRUCT\n");}
// "return" {printf("RETURN\n");}
// "if" {printf("IF\n");}
// "else" {printf("ELSE\n");}
// "while" {printf("WHILE\n");}



// {whitespace} {;}
// {TYPE} {printf("TYPE\n");}
// {RELOP} {printf("RELOP\n");}



// {OCT} {
//     int len = strlen(yytext);
//     int sum = 0;
//     for(int i = 1; i < len;i++)
//     {
//         sum += sum*8 + yytext[i] - '0';
//     }
//     printf("OCT:%d\n",sum);
// }

// {HEX} {
//     int len = strlen(yytext);
//     int sum = 0;
//     char c;
//     for(int i = 2; i < len;i++)
//     {
//         c = yytext[i];
//         if(c>='0'&&c<='9')
//         {
//             sum += sum*16 + yytext[i] - '0';
//         }
//         else if(c>='a'&&c<='f')
//         {
//             sum += sum*16 + yytext[i] - 'a' + 10;
//         }
//         else if(c>='A'&&c<='F')
//         {
//             sum += sum*16 + yytext[i] - 'A' + 10;
//         }
//     }
//     printf("HEX:%d\n",sum);
// }

// {INT} {
//     printf("INT:%d\n",atoi(yytext));
// }

// {FLOAT} {   
    
//     printf("FLOAT:%f\n",strtof(yytext,NULL));
// }

// {ID} {
//     printf("%s : %s\n","ID",yytext);
// }


