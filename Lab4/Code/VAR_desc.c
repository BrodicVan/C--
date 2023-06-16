#include "all.h"

int cur_bia;// 当前函数内的最后的变量偏移量
Var_Desc* all_var_head;// 从1开始存储变量，从1+var_count开始存储临时变量
extern int cur_var_no;
extern int cur_tem_no;

// 计算IR操作数的哈希值，变量在前，临时变量在后，从1开始增长
int hash_var_attr(int ir_kind,int no)
{
    int bia = (ir_kind==TEMP)?(cur_var_no-1):0;
    return no + bia;
}

// 计算IR操作数的哈希值，变量在前，临时变量在后
int hash_var_operand(IR_Operand ope)
{
    if(ope->kind==CONSTANT_IR)
    {
        printf("error: 对常量操作数求哈希值\n");
        return 0;
    }
    int bia = (ope->kind==TEMP)?(cur_var_no-1):0;
    return ope->u.no + bia;
}

// 生成空变量描述符
Var_Desc empty_Var_Desc()
{
    Var_Desc tem = (Var_Desc) malloc(sizeof(struct Var_Desc_));
    // memset(tem,0,sizeof(tem));
    tem->bia = -1;
    tem->hash = 0;
    // tem->if_memory = 1;
    tem->reg_tag = -1;
    return tem;
}

// 根据已分配的临时变量和常驻变量的数目来确定变量描述符的大小
void init_all_var_list()
{
    int var_count = cur_var_no - 1;
    int tem_count = cur_tem_no - 1;

    int total_var_count = var_count+tem_count;
    all_var_head = (Var_Desc*) malloc(sizeof(Var_Desc)*total_var_count+8);// 为什么+3，因为偷懒，不想算
    int i;
    Var_Desc tem;
    for(i = 1; i <= total_var_count;i++)
    {
        tem = empty_Var_Desc();
        // 局部变量
        if(i <= var_count)
        {   
            tem->hash = hash_var_attr(VARIABLE,i);
        }
        // 临时变量
        else
        {
            tem->hash = hash_var_attr(TEMP,i-var_count);
        }
        all_var_head[i] = tem;
    }

}

int my_count = 0;

// 处理变量的偏移量
void process_bia_var(IR_Operand ir_ope,int size)
{
    // printf("debug: 进入process_bia_var\n");
    
    int ir_ope_kind = ir_ope->kind;
    switch (ir_ope_kind)
    {
        case CONSTANT_IR:
            // printf("error: 常数无需处理偏移量");
            break;
        case VARIABLE:
        case TEMP:
            int ope_no = ir_ope->u.no;
            int ope_hash = hash_var_attr(ir_ope_kind,ope_no);
            // printf("----------------------\n");
            // printf("debug: process_bia_var ope_kind %d\n",ir_ope_kind);
            // printf("debug: process_bia_var ope_no %d\n",ir_ope->u.no);
            // printf("debug: process_bia_var ope_hash %d\n",ope_hash);
            // printf("debug: process_bia_var 断点1\n");
            // 变量可能重复出现，只有第一次出现再分配偏移量
            
            int var_bia = all_var_head[ope_hash]->bia;
            
            if(var_bia < 0)
            {
                all_var_head[ope_hash]->bia = cur_bia;
                // printf("debug: cur_count %d\n",++my_count);
                // printf("debug: cur_bia %d\n",cur_bia);
                cur_bia += size;
            }
            
            // printf("debug: process_bia_var 断点2\n");
            break;
        default:
            printf("error: 处理变量偏移switch错误\n");
            break;
    }
    // printf("debug: 退出process_bia_var\n");
}

// 处理IR中的变量偏移量
void process_bia_ir(InterCodes ir)
{
    // printf("debug: 进入 process_bia_ir\n");
    
    int ir_kind = ir->code.ir_kind;
    switch (ir_kind)
    {
        case ASSIGN_IR:
            // printf("debug: process_bia_ir ASSIGN_IR\n");
            process_bia_var(ir->code.u.assign.left,4);
            process_bia_var(ir->code.u.assign.right,4);
            break; 
        case ADD_IR:
            // printf("debug: process_bia_ir ADD_IR\n");
            process_bia_var(ir->code.u.binary_op.result,4);
            process_bia_var(ir->code.u.binary_op.op1,4);
            process_bia_var(ir->code.u.binary_op.op2,4);
            break;
        case SUB_IR:
            // printf("debug: process_bia_ir SUB_IR\n");
            process_bia_var(ir->code.u.binary_op.result,4);
            process_bia_var(ir->code.u.binary_op.op1,4);
            process_bia_var(ir->code.u.binary_op.op2,4);
            break;
        case MUL_IR:
            // printf("debug: process_bia_ir MUL_IR\n");
            process_bia_var(ir->code.u.binary_op.result,4);
            process_bia_var(ir->code.u.binary_op.op1,4);
            process_bia_var(ir->code.u.binary_op.op2,4);
            break;
        case DIV_IR:
            // printf("debug: process_bia_ir DIV_IR\n");
            process_bia_var(ir->code.u.binary_op.result,4);
            process_bia_var(ir->code.u.binary_op.op1,4);
            process_bia_var(ir->code.u.binary_op.op2,4);
            break;
        case IF_IR:
            // printf("debug: process_bia_ir IF_IR\n");
            process_bia_var(ir->code.u.if_op.left,4);
            process_bia_var(ir->code.u.if_op.right,4);
            break;
        case DEC_IR:
            // printf("debug: process_bia_ir DEC_IR\n");
            process_bia_var(ir->code.u.dec.place,ir->code.u.dec.size);
            break;
        case PARAM_IR:
            // printf("debug: process_bia_ir PARAM_IR\n");
            process_bia_var(ir->code.u.read_write_arg_param_return.place,4);
            break;
        // 一下IR语句中的操作数要么是前面出现过，要么是不存在
        // case LABEL_IR:
        // case GOTO_IR:
        // case RETURN_IR:
        // case ARG_IR:
        // case CALL_IR:
        // case FUNC_IR:
        // case READ_IR:
        // case WRITE_IR:
        default:
            break;
    }
    // printf("debug: 退出 process_bia_ir\n");
}

// 获取变量的偏移量
int get_var_bia(int var_hash)
{
    return all_var_head[var_hash]->bia;
}

// TODO：顺便计算变量的偏移量
// ir_head不不存放信息
void split_basic_block(InterCodes ir_head)
{
    // printf("debug: 进入 split_basic_block\n");

    InterCodes tem_func;
    cur_bia = 0;
    InterCodes cur_ir = ir_head->nxt;
    if(cur_ir==NULL)
    {
        printf("error:划分基本块时IR链表为空\n");
    }
    cur_ir->if_block_head = 1;// 第一个IR是基本块的头
    process_bia_ir(cur_ir);
    // 标记当前FUNC_IR，用来后面修改其偏移量
    if(cur_ir->code.ir_kind==FUNC_IR)
    {
        tem_func = cur_ir;
        cur_bia = 0;
    }
    InterCodes next_ir = cur_ir->nxt;

    while(next_ir!=NULL)
    {
        // printf("debug: next_ir kind %d\n",next_ir->code.ir_kind);
        if(next_ir->code.ir_kind==LABEL_IR)
        {
            next_ir->if_block_head = 1;
        }
        else if(next_ir->code.ir_kind==FUNC_IR)
        {
            // 计算FUNC_IR对应函数的帧大小
            // printf("debug: func_main:%s,frame_size:%d\n",tem_func->code.u.func_call.func,tem_func->code.u.func_call.frame_size);
            tem_func->code.u.func_call.frame_size = FRAME_FIXED_SIZE + cur_bia;
            cur_bia = 0;
            tem_func = next_ir;
            next_ir->if_block_head = 1;
        }
        else if(cur_ir->code.ir_kind==GOTO_IR || cur_ir->code.ir_kind==IF_IR)
        {
            next_ir->if_block_head = 1;
        }
        else
        {
            next_ir->if_block_head = 0;
        }
        cur_ir = next_ir;
        process_bia_ir(cur_ir);
        next_ir = cur_ir->nxt;
    }
    // 最后一个函数的处理
    tem_func->code.u.func_call.frame_size = FRAME_FIXED_SIZE + cur_bia;
    cur_bia = 0;
    // printf("debug: 退出 split_basic_block\n");
}

Var_List var_list(int hash)
{
    Var_List res_var = (Var_List) malloc(sizeof(struct Var_List_));
    res_var->hash = hash;
    res_var->nxt =NULL;
    return res_var;
}

// head不删除
void free_VAR_LIST(Var_List head)
{
    if(head==NULL)
    {
        printf("error: 释放基本块变量head为NULL\n");
        return;
    }
    Var_List cur = head->nxt;
    Var_List nxt;
    while(cur!=NULL)
    {
        nxt = cur->nxt;
        free(cur);
        cur = nxt;
    }
}
