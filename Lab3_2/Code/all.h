#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "syntax.tab.h"
#include <unistd.h>

// 指针
typedef struct Domain_Item_* Domain_Item;
typedef struct Var_* Var;
typedef struct Type_* Type;
typedef struct Common_* Common;
typedef struct FieldList_* FieldList;


/*
    下面是有关语法树的头文件定义
*/
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
    ParamDec0, ExtDef0, StmtList0, Stmt0
};

enum ID_Type 
{
    // ALL用于search_common函数，表示搜索所有类型的common
    VAR, STRU, FIELD, FUNC, ALL
};

struct Node
{
    int tag;
    char* name;// 类型名称或标识符名称或relop； 根据tag而定
    struct Node *kids[8];// kid nodes
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


/*
    下面是有关语义分析的头文件定义
*/
// semantics.h部分

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
    int var_no;
    unsigned hash;
    Type type;
    Var nxt_var;
    
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
    int size;// 类型大小
};


// 域结构体
struct FieldList_
{
    char* name; // 域的名字
    Type type; // 域的类型
    // int position; // 域的相对位置
    FieldList tail; // 下一个域
};

void recursive_semantic(struct Node* root);
Common search_common(char* name,int kind);
Var search_var(char* name,int if_global);
void push_domain();
void pop_domain();
void insert_common(Common common);
/*
    下面是有关IR的头文件定义
*/
 

// IR的属性
typedef struct IR_Attribute_* IR_Attribute;
struct IR_Attribute_
{
    int label_num;
    int line_num;
    int line_count;
};

typedef struct Operand_* Operand;
typedef struct Arg_List_* Arg_List;
typedef struct InterCodes_* InterCodes;
typedef struct Operand_List_* Operand_List;

struct Arg_List_
{
    Operand arg;
    Arg_List nxt;
};

// 操作数
// ADDRESS用于表示数组和结构体的地址，且只会对VARIABLE取值
struct Operand_ 
{
    enum { VARIABLE, CONSTANT, TEMP} kind;
    union 
    {
        int no;// VARIBLE/ADDRESS/TEMP的序号，分开统计 
        int value;// CONSTANT的值
        // Operand pointer;// 改地址所对应的指针变量名(结构体变量名、数组变量名)
    } u;
    int if_addr;// 用于判断是否为地址，若为地址，则不会对VARIABLE取址(&v)
    int if_deref;
    int if_take_addr;
};

// 记录操作数的链表
struct Operand_List_
{
    Operand ope;
    struct Operand_List_* nxt;
};

// 中间代码
// 可能不需要用，增量式翻译
struct InterCode
{
    enum { ASSIGN_IR, ADD_IR, SUB_IR, MUL_IR, DIV_IR, 
            LABEL_IR, RETURN_IR, GOTO_IR, IF_IR, 
            READ_IR, WRITE_IR,FUNC_IR, ARG_IR, PARAM_IR, CALL_IR, 
            DEC_IR } ir_kind;
    union 
    {
        struct { Operand right, left; } assign;
        struct { Operand result, op1, op2; } binary_op;
        struct { Operand result, op1; } unary_op;
        struct { int label_no; } label;
        struct { Operand return_operand; } return_op;
        struct { Operand left, right; int label_no; char* relop; } if_op;
        struct { Operand place; } read_write_arg_param_return;// READ_IR、WRITE_IR、PARAM_IR、RETURN_IR共用
        struct { Operand left; char* func; } func_call;// FUNC_IR、CALL_IR共用
        struct { Operand place; int size; } dec;
    } u;
    
};


// 加上链表
struct InterCodes_
{ 
    struct InterCode code; 
    InterCodes prev, nxt; 
};

void generate_IR(struct Node* nd,char* ir_file);
void translate_Specifier(struct Node* nd);
void translate_Cond(struct Node* nd,int label_1,int label_2);
void translate_Exp(struct Node* nd,Operand place,int reamain_addr);
void translate_CompSt(struct Node* nd);
Operand create_tem();
char* ope2str(Operand op);