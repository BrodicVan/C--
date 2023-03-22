#include <stdio.h>
#include "syntax.tab.h"
#include "tree.h"


extern int fail;



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
    if(1)
    if(!fail)
    {
        // stack_print(top);
        recursion_print(top);
    }
    
    return 0;
}

