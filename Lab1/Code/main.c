#include <stdio.h>
#include "syntax.tab.h"
#include "tree.h"


extern int fail;
void yyerror(char* msg)
{
    fail = 1;
    extern int yylineno;	// defined and maintained in lex
	extern char *yytext;	// defined and maintained in lex
	// int len=strlen(yytext);
	// int i;
	// char buf[512]={0};
	// for (i=0;i<len;++i)
	// {
	// 	sprintf(buf,"%s%d ",buf,yytext[i]);
	// }
	fprintf(stderr, "Error type B at line %d: %s at symbol '%s'\n", yylineno, msg, yytext);
}

void my_yyerror(char* msg, int yylineno)
{
    fprintf(stderr,"Error type B at Line %d: %s\n",yylineno,msg);
}

extern void yyrestart(FILE* f);
extern FILE* yyin;
struct Node* top;
extern int yyparse();

// extern void yylex();
extern int yyparse();


int main(int argc, char **argv)
{
    
    if(argc <= 1) return 1;
    FILE* f = fopen(argv[1],"r");
    if(!f)
    {
        perror(argv[1]);
        return 1;
    }
    yyrestart(f);
    // yylex();
   
    yyparse();
    if(!fail)
    {
        stack_print(top);
        // recursion_print(top);
    }
    
    return 0;
}

