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
typedef struct Mip_Operand_* Mip_Operand;
typedef struct IR_Attribute_* IR_Attribute;
typedef struct IR_Operand_* IR_Operand;
typedef struct Arg_List_* Arg_List;
typedef struct InterCodes_* InterCodes;
typedef struct IR_Operand_List_* IR_Operand_List;
typedef struct Mips_List_* Mips_List; 
typedef struct Var_Desc_* Var_Desc;
typedef struct Var_Desc_List_* Var_Desc_List;
typedef struct Var_List_* Var_List;
typedef struct Mips_List_* Mips_List; 
/***********************************************************************************************
                                    有关语法树的头文件定义
************************************************************************************************/

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
    ParamDec0, ExtDef0, StmtList0, Stmt0, ExtDecList0, ExtDecList1
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
    Type VarDec_type;// VarDec的类型，用于数组
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


/************************************************************************************************
                                    有关语义分析的头文件定义
************************************************************************************************/
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

/************************************************************************************************
                                    有关IR的头文件定义
************************************************************************************************/



// IR的属性
struct IR_Attribute_
{
    int label_num;
    int line_num;
    int line_count;
};


struct Arg_List_
{
    IR_Operand arg;
    Arg_List nxt;
};

// 操作数
// ADDRESS用于表示数组和结构体的地址，且只会对VARIABLE取值
struct IR_Operand_ 
{
    enum { VARIABLE, CONSTANT_IR, TEMP} kind;
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
struct IR_Operand_List_
{
    IR_Operand ope;
    struct IR_Operand_List_* nxt;
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
        struct { IR_Operand right, left; } assign;
        struct { IR_Operand result, op1, op2; } binary_op;
        struct { IR_Operand result, op1; } unary_op;
        // struct { int label_no; } label;
        struct { IR_Operand return_operand; } return_op;
        struct { IR_Operand left, right; int label_no; char* relop; } if_op;// GOTO_IR、IF_IR、LABEL_IR
        struct { IR_Operand place; } read_write_arg_param_return;// READ_IR、WRITE_IR、PARAM_IR、RETURN_IR共用
        struct { IR_Operand left; char* func; int frame_size;} func_call;// FUNC_IR、CALL_IR共用，frame_size代表对应函数整个帧的大小
        struct { IR_Operand place; int size; } dec;
    } u;
    
};


// 加上链表
struct InterCodes_
{ 
    struct InterCode code; 
    InterCodes prev, nxt; 
    int if_block_head;
};

void generate_IR(struct Node* nd,char* ir_file);
void translate_Specifier(struct Node* nd);
void translate_Cond(struct Node* nd,int label_1,int label_2);
void translate_Exp(struct Node* nd,IR_Operand place,int reamain_addr);
void translate_CompSt(struct Node* nd);
IR_Operand create_tem();
char* ope2str(IR_Operand op);
int get_num_len(int num);
void clear_IR_Operand_List(IR_Operand_List head);

/************************************************************************************************
                                    有关目标代码生成的头文件定义
************************************************************************************************/
#define FRAME_FIXED_SIZE 80 // 栈帧(活动记录)固定大小：$ra(1),$fp(1),$t?(8),$s(8)
// 寄存器结构体
struct Register_
{
    int ID;
    struct
    {
        enum { zero_REG,at_REG,a_REG,v_REG,t_REG,s_REG,k_REG,gp_REG,sp_REG,fp_REG,ra_REG } kind;// 具体含义看实践指导
        int no;
    } alias;
};

/*********************************************************
                    有关MIP操作数的头文件定义
**********************************************************/

// MIP操作数结构体
struct Mip_Operand_
{
    enum {REG,CONSTANT_MIP,FUNC_MIP} mip_ope_kind;
    union 
    {
        struct Register_ reg;
        struct {int value;int if_label;} const_;
        char* func_name;
    } u;
    
};

Mip_Operand const_MIP_operand(int val,int if_label);
Mip_Operand reg_MIP_operand_alias(int reg_kind,int no);
Mip_Operand reg_MIP_operand_ID(int reg_ID);
Mip_Operand func_MIP_operand(char* func_name);
void init_all_reg_operand();
Mip_Operand get_reg_ope_by_ID(int reg_ID);
Mip_Operand empty_MIP_operand();
char* MIP_ope2str(Mip_Operand mip_ope);
/*********************************************************
                    有关MIP指令的头文件定义
**********************************************************/
// MIP指令列表结构体
struct Mip_
{
    enum {nop_MIP,label_MIP,func_MIP,li_MIP,move_MIP,addi_MIP,subu_MIP,add_MIP,sub_MIP,mul_MIP,div_MIP,mflo_MIP,
        lw_MIP,sw_MIP,j_MIP,jal_MIP,jr_MIP,beq_MIP,bne_MIP,bgt_MIP,blt_MIP,bge_MIP,ble_MIP} mip_kind;
    Mip_Operand orerands[3];
};

struct Mips_List_
{
    struct Mip_ mip;
    // Mips_List prev;
    Mips_List nxt;
};

void output_MIPs(Mips_List head,char* file_name);
Mips_List empty_MIP();
void IRs2MIPs(InterCodes head);
Mips_List IF_IR2MIP(InterCodes if_ir);
void generate_code(InterCodes ir_head,char* mip_file);
void insert_MIP(Mips_List new_mip);
Mips_List create_lw_mip(int reg_ID,int bia,int base_reg_ID);
Mips_List create_sw_mip(int reg_ID,int bia,int base_reg_ID);
Mips_List create_add_mip(int result_ID,int left_ID,int right_ID);
Mips_List create_sub_mip(int result_ID,int left_ID,int right_ID);
Mips_List create_mul_mip(int result_ID,int left_ID,int right_ID);
Mips_List create_div_mip(int result_ID,int left_ID,int right_ID);
Mips_List create_addi_mip(int dst_ID,int src_ID,int const_val);
Mips_List create_subu_mip(int dst_ID,int src_ID,int const_val);
Mips_List create_move_mip(int dst_ID,int src_ID);
Mips_List create_li_mip(int dst_ID,int const_val);
Mips_List create_mul_mip(int result_ID,int left_ID,int right_ID);
Mips_List create_mflo_mip(int reg_ID);
/*********************************************************
                    有关变量处理的头文件定义
**********************************************************/

// 变量描述符结构体
struct Var_Desc_
{
    unsigned int hash;// 使用哈希值区分不同的变量(临时和常驻)
    int reg_tag;// 存放存储在哪个寄存器中
    // int if_memory;// 是否在内存中
    int bia;// 相对于sp/fp指针的偏移量
};

// struct Var_Desc_List_
// {
//     struct Var_Desc_ var_dec;
//     Var_Desc_List prev;
//     Var_Desc_List nxt;
// };


// 运算量(变量/常数)列表结构体
struct Var_List_
{
    int if_var;// 是否为(局部/临时)变量
    union 
    {
        int hash;
        int val;
    };// 记录一个变量的哈希值或一个常量的真值
    Var_List nxt;
};

int hash_var_attr(int kind,int no);
int hash_var_operand(IR_Operand ope);
int get_var_bia(int var_hash);
void free_VAR_LIST(Var_List head);
Var_List var_list(int hash);
void split_basic_block(InterCodes ir_head);
void init_all_var_list();
/*********************************************************
                    有关寄存器分配的头文件定义
**********************************************************/

// // 记录一个函数中的变量列表头
// typedef struct FUNC_VAR_* FUNC_VAR;
// struct FUNC_VAR_
// {
//     VAR_LIST head;
//     FUNC_VAR nxt_func;
// };
// FUNC_VAR empty_FUNC_VAR();

// // 寄存器结构体
// struct Register_
// {
//     int ID;
//     struct
//     {
//         enum { zero_REG,at_REG,a_REG,v_REG,t_REG,s_REG,k_REG,gp_REG,sp_REG,fp_REG,ra_REG } kind;// 具体含义看实践指导
//         int no;
//     } alias;
// };


void free_all_regs();
void free_reg(int ID);
void use_reg(int reg_ID,int var_hash);
int ensure(IR_Operand ope,int use_assgin);
int alias2ID(int reg_kind,int no);
int find_latest_use_reg(InterCodes cur_ir,int var_type);
int allocate(int var_type);
void spill_reg(int reg_ID);