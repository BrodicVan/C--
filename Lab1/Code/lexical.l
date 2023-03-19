%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "syntax.tab.h"
#include "tree.h"
int yycolumn = 1;
#define YY_USER_ACTION  \
    yylloc.first_line = yylloc.last_line = yylineno;\
    yylloc.first_column = yycolumn; \
    yylloc.last_column = yycolumn+yyleng;\
    yycolumn += yyleng;
extern int fail;

%}

%option yylineno


delim [ \t]
whitespace {delim}+
letter [A-Za-z_]
digit [0-9]


COMMENT_SINGLE \/\/[^\n]*
COMMENT_MULTI "/*"([^\*]|(\*)*[^\*\/])*(\*)*"*/"

DEC 0|[1-9]{digit}*
OCT 0[0-7]+
HEX 0[xX]([0-9A-F]|[0-9a-f])+ 
FLOAT {digit}+\.{digit}+|{digit}+\.{digit}*([eE][+-]?{digit}+)?

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

{delim} {}


{TYPE} {
    yylval.type_node = createNode(yylineno,TYPE,yytext,0,0);
    return TYPE;
    }
{RELOP} {return RELOP;}
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


{DEC} {
    yylval.type_node = createNode(yylineno,INT,NULL, atoi(yytext),0);
    // yylval.type_int = atoi(yytext);
    return INT;
    // return createNode(yylineno,INT,NULL, atoi(yytext),0);
}

{OCT} {
    int len = strlen(yytext);
    int sum = 0;
    for(int i = 1; i < len;i++)
    {
        sum += sum*8 + yytext[i] - '0';
    }
    yylval.type_node = createNode(yylineno,INT,NULL,sum,0);
    return INT;
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
    return INT;
}

{FLOAT} {   
    // struct Node* t = createNode(yylineno,INT,NULL,0,strtof(yytext,NULL));
    // printf("%d\n",t->tag);
    yylval.type_node = createNode(yylineno,FLOAT,NULL,0,strtof(yytext,NULL));
    
    // yylval.type_double = strtof(yytext,NULL);
    return FLOAT;
    // return createNode(yylineno,INT,NULL,0,strtof(yytext,NULL));
}

{ID} {
    yylval.type_node = createNode(yylineno,ID,yytext,0,0);
    return ID;
}


. {
    fail = 1;
    printf("Error type A at Line %d:Mysterious character \'%s\'\n",yylineno,yytext);
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


