
// 指针
typedef struct Domain_Item_* Domain_Item;
typedef struct Signal_* Signal;
typedef struct Type_* Type;
typedef struct FieldList_* FieldList;


// tree.h部分，有关语法树
typedef union
{
    int type_int;
    double type_double;
} Value;

struct Node
{
    int tag;
    char* name;// 类型名称或标识符名称； 根据tag而定
    struct Node *kids[10];// kid nodes
    int kid_num;
    Value value;
    int level;// node_height
    int row;
    int if_empty;

    // Type inherited_type;// 继承下来的类型
    Type type;// 节点类型

    char* struct_specifier_name;// 结构体标识符所代表的结构
    Type struct_specifier_type;// 结构体标识符所代表的结构
};

struct Node *createNode(int r,int tag, char *text,int i,double d);

void treePrintLevel(struct Node *nd);
void stack_print(struct Node * nd);
void recursion_print(struct Node *nd);
 
enum yyNTtype
{
    Program=10000, ExtDecList, ExtDef, ExtDefList, Specifier, StructSpecifier, OptTag, Tag, VarDec, FunDec, VarList, ParamDec, CompSt, StmtList, Stmt, DefList, Def, Exp, Args, DecList, Dec
};


// semantics.h部分，有关语义分析

#define SIGNAL_TABLE_SIZE 0x3fff
#define STRUCTURE_TABLE_SIZE 0x3fff
#define DOMAIN_LAYER 10




// 作用域结构体
struct Domain_Item_
{
    Signal signal;
    Domain_Item nxt_item;
};

// 符号表结构体
struct Signal_
{
    char* name;// 符号名称
    enum {VAR,FUNC,STRU} kind;// 变量或者函数
    /*
        若为VAR，则type记录--变量结构
        若为STRU，则type记录--这种结构体的结构
    */
    Type type;
    Signal nxt_signal;
    unsigned hash;
};

// 类型结构体
// 若类型是结构体，那么就根据FieldList是否为空判断一个结构体类型是否被找完
struct Type_
{
    enum { BASIC, ARRAY, STRUCTURE } kind;
    union
    {
        // 基本类型
        int basic;
        // 数组类型信息包括元素类型与数组大小构成
        struct { Type elem; int size; } array;
        // 结构体类型信息是一个链表
        FieldList structure;
    } u;
};


// 域结构体
struct FieldList_
{
    char* name; // 域的名字
    Type type; // 域的类型
    FieldList tail; // 下一个域
};

void init_semantics();
unsigned hash_pjw(char* name);
void push_domain();
void pop_domain();
Signal search_signal(char* name);
void insert_signal(Signal signal);
void recursive_semantic(struct Node* root);

