
// 指针
typedef struct Domain_Item_* Domain_Item;
typedef struct Var_* Var;
typedef struct Type_* Type;
typedef struct Common_* Common;
typedef struct FieldList_* FieldList;


// tree.h部分，有关语法树
typedef union
{
    int type_int;
    double type_double;
} Value;

enum Generation_Type
{
    Common_Generation, StructSpecifier0, Def0, CompSt0, DecList0, DecList1, Dec0, Dec1,
    VarDec0, VarDec1, DefList0, FunDec0, FunDec1, OptTag0, Tag0, Exp0, Exp1, Exp2, Exp3, 
    ParamDec0, ExtDef0, StmtList0
};

enum ID_Type 
{
    // ALL用于search_common函数，表示搜索所有类型的common
    VAR, STRU, FIELD, FUNC, ALL
};

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

    enum Generation_Type generation;// 产生式类型，用于计算继承属性
    Type inh_type;// 继承下来的类型
    Type return_type;// 函数的返回值类型
    Type type;// 节点类型,功能多样: 记录变量/式子类型，确定结构体内部类型
    enum {LEFT,RIGHT} l_r;// 左值右值 
    int if_stru_deflist;// 记录DefList是否是在某个结构体定义里，从而判断插入哪个符号表；属于继承属性
    Common struct_specifier;// Common表中的对应的结构体条目
    int if_structspecifier;// 记录一个Specifier是否为StructSpecifier
    enum ID_Type id_type;// 记录一个ID所代表的的含义
    enum {DEF,USE} id_usage;// 记录一个ID的用处
};

struct Node *createNode(int r,int tag, char *text,int i,double d);
void addNode(struct Node* fa,struct Node* kid);
void addNodes(struct Node* fa, int num,...);
void treePrintLevel(struct Node *nd);
void stack_print(struct Node * nd);
void recursion_print(struct Node *nd);
 
enum yyNTtype
{
    Program=10000, ExtDecList, ExtDef, ExtDefList, Specifier, StructSpecifier, OptTag, Tag, VarDec, FunDec, VarList, ParamDec, CompSt, StmtList, Stmt, DefList, Def, Exp, Args, DecList, Dec
};



// semantics.h部分，有关语义分析

#define SIGNAL_TABLE_SIZE 0x3fff
#define COMMON_TABLE_SIZE 0x3fff
#define DOMAIN_LAYER 10


// 作用域结构体
struct Domain_Item_
{
    Var var;
    Domain_Item nxt_item;
};

// 一般结构体
struct Common_
{
    char* name;// 结构体名字
    enum ID_Type kind;// 结构体/域/函数
    /*
        若为STRU，则type记录--这种结构体的结构
        若为FUNC，则type记录--返回值和参数
    */
    Type type;// 结构体域
    Common nxt_common;

};

// 符号表结构体
struct Var_
{
    char* name;// 符号名称
    Type type;
    Var nxt_var;
    unsigned hash;
};

// 类型结构体
// 若类型是结构体，那么就根据FieldList是否为空判断一个结构体类型是否被找完
struct Type_
{
    enum { BASIC, ARRAY, STRUCTURE, UNKNOWN } kind;
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
unsigned hash_pjw(char* name,unsigned size);
void push_domain();
void pop_domain();
// 
void recursive_semantic(struct Node* root);

