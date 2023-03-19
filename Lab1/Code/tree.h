typedef union
{
    int type_int;
    double type_double;
}Type;

struct Node
{
    int tag;
    char* name;// type_name or id_name
    struct Node *kids[10];// kid nodes
    int kid_num;
    Type value;
    int level;// node_height
    int row;
    int if_empty;
};

struct Node *createNode(int r,int tag, char *text,int i,double d);
void treePrintLevel(struct Node *nd);
void stack_print(struct Node * nd);
void recursion_print(struct Node *nd);
 
enum yyNTtype
{
    Program=10000, ExtDecList, ExtDef, ExtDefList, Specifier, StructSpecifier, OptTag, Tag, VarDec, FunDec, VarList, ParamDec, CompSt, StmtList, Stmt, DefList, Def, Exp, Args, DecList, Dec

};