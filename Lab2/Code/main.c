#include <stdio.h>
#include "tree_semantics.h"

extern int fail;

extern void yyrestart(FILE* f);
extern FILE* yyin;
struct Node* root;
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
        // stack_print(root);
        // recursion_print(root);
        recursive_semantic(root);
    }
    
    return 0;
}
