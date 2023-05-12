#include <stdio.h>
#include "all.h"
#include <sys/types.h>
#include <sys/stat.h>

extern int fail;

extern void yyrestart(FILE* f);
extern FILE* yyin;
struct Node* root;
extern int yyparse();

// extern void yylex();
extern int yyparse();

FILE* IR_file;

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
        printf("语义分析开始\n");
        recursive_semantic(root);
        printf("语义分析结束\n");
        
    }
    if(1)
    if(!fail)
    {
        if(access("../IR",0) == -1)
        {
            mkdir("../IR",0777);
        }
        char* ir_file = (char*) malloc(strlen(argv[1])+12);
        strcpy(ir_file,"../IR/");
        strcat(ir_file,argv[1]+8);
        strcat(ir_file,".ir");
        generate_IR(root,ir_file);
    }

    return 0;
}
