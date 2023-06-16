#include "all.h"

// extern InterCodes ir_head;// 存储IR链表，head不存放
// 标记寄存器被哪个变量占用
unsigned int reg_occupancy[32];
extern Var_Desc* all_var_head;// 所有的变量描述符，从1开始存储变量，从1+var_count开始存储临时变量
extern InterCodes mip_cur_ir;// 当前转换到哪一条IR指令
extern Mip_Operand reg_operands[32];

int reg_zero = 0;
int reg_at = 1;
int reg_v[2] = {2,3};
int reg_a[4] = {4,5,6,7};
int reg_t[10] = {8,9,10,11,12,13,14,15,24,25};
int reg_s[9] = {16,17,18,19,20,21,22,23,30};
int reg_k[2] = {26,27};
int reg_gp = 28;
int reg_sp = 29;
int reg_fp = 30;
int reg_ra = 31;

// // 创建空FUNC_VAR
// FUNC_VAR empty_FUNC_VAR()
// {
//     FUNC_VAR func_var = (FUNC_VAR) malloc(sizeof(struct FUNC_VAR_));
//     func_var->head = var_list(-1);// head不存储数据
//     func_var->nxt_func = NULL;   
// }

// 释放所有寄存器
void free_all_regs()
{
    memset(reg_occupancy,0,sizeof(reg_occupancy));
}

// 标记某个寄存器空闲，对应变量的寄存器标记设为-1
void free_reg(int reg_ID)
{
    all_var_head[reg_occupancy[reg_ID]]->reg_tag = -1;
    reg_occupancy[ID] = 0;

}

// 标记某个寄存器不空闲
void use_reg(int reg_ID,int var_hash)
{
    reg_occupancy[reg_ID] = var_hash;
    all_var_head[var_hash]->reg_tag = reg_ID;
}

// 变量存在哪个寄存器中
int var_in_reg(IR_Operand ir_ope)
{
    int var_hash = hash_var_operand(ir_ope);
    if(var_hash==0)
    {
        return -1;
    }
    return all_var_head[var_hash]->reg_tag;
    // unsigned int reg_tag = all_var_head[var_hash]->reg_tag;
    // int i = 0;
    // while(i < 32)
    // {
    //     if(reg_tag & 1<<i)
    //     {
    //         return i;
    //     }
    //     i += 1;
    // }
    // return -1;
}


// 寄存器别名到寄存器编号
int alias2ID(int reg_kind,int no)
{

    switch(reg_kind)
    {
        case zero_REG:
            return reg_zero;
            break;
        case at_REG:
            return reg_at;
            break;
        case a_REG:
            return reg_a[no];
            break;
        case v_REG:
            return reg_v[no];
            break;
        case t_REG:
            return reg_t[no];
            break;
        case s_REG:
            return reg_s[no];
            break;
        case k_REG:
            return reg_k[no];
            break;
        case gp_REG:
            return reg_gp;
            break;
        case sp_REG:
            return reg_sp;
            break;
        case fp_REG:
            return reg_fp;
            break;
        case ra_REG:
            return reg_ra;
            break;
        default:
            return -1;
            break;
    }
}

// 寄存器编号到寄存器别名，暂未实现
Mip_Operand ID2alias(int reg_ID)
{
    // TODO：这个不知道要不要写
    return NULL;
}

// $t0-$t7是开放的，$t8-$9用作过渡寄存器，不open
int if_open_reg_t(int reg_ID)
{
    // return (reg_ID >= 8 && reg_ID <= 15) || (reg_ID >= 24 && reg_ID <= 25);
    return (reg_ID >= 8 && reg_ID <= 15);
}

int if_reg_s(int reg_ID)
{
    return (reg_ID >= 16 && reg_ID <= 23);
}

int find_latest_use_reg(InterCodes cur_ir,int ir_var_type)
{
    int reg_wait[32] = {0};
    int (*check_reg) (int regID);
    int left_reg_num;
    if(ir_var_type==VARIABLE)
    {
        left_reg_num = 8;
        check_reg = if_reg_s;
    }
    else if(ir_var_type==TEMP)
    {
        // open的只有8个
        left_reg_num = 8;
        check_reg = if_open_reg_t;
    }
    else
    {
        printf("error: find_latest_use_reg error\n");
    }

    int i;
    for(i = 0; i < 32;i++)
    {
        if((*check_reg)(i))
        {
            reg_wait[i] = 1;
        }
    }
    while(cur_ir!=NULL)
    {
        if(left_reg_num <= 1 ) {break;}
        if(cur_ir->if_block_head)
        {
            break;
        }
        int tem;
        switch(cur_ir->code.ir_kind)
        {
            case ASSIGN_IR:
                {
                    tem = var_in_reg(cur_ir->code.u.assign.left);
                    if((*check_reg)(tem))
                    {
                        reg_wait[tem] = 0;
                        left_reg_num--;
                        if(left_reg_num <= 1 ) {break;}
                    }

                    IR_Operand right_ope =  cur_ir->code.u.assign.right;
                    if(right_ope->kind!=CONSTANT_IR)
                    {
                        tem = var_in_reg(right_ope);
                        if((*check_reg)(tem))
                        {
                            reg_wait[tem] = 0;
                            left_reg_num--;
                            if(left_reg_num <= 1 ) {break;}
                        }
                    }
                    
                }
                
                break; 
            case ADD_IR:
            case SUB_IR:
            case MUL_IR:
            case DIV_IR:
                {
                    IR_Operand op1 = cur_ir->code.u.binary_op.op1;
                    IR_Operand op2 = cur_ir->code.u.binary_op.op2;
                    IR_Operand result = cur_ir->code.u.binary_op.result;

                    if(op1->kind!=CONSTANT_IR)
                    {
                        tem = var_in_reg(op1);
                        if((*check_reg)(tem))
                        {
                            reg_wait[tem] = 0;
                            left_reg_num--;
                            if(left_reg_num <= 1 ) {break;}
                        }
                    }
                    
                    if(op2->kind!=CONSTANT_IR)
                    {
                        tem = var_in_reg(op2);
                        if((*check_reg)(tem))
                        {
                            reg_wait[tem] = 0;
                            left_reg_num--;
                            if(left_reg_num <= 1 ) {break;}
                        }
                    }
                    
                    tem = var_in_reg(result);
                    if((*check_reg)(tem))
                    {
                        reg_wait[tem] = 0;
                        left_reg_num--;
                        if(left_reg_num <= 1 ) {break;}
                    }
                }
                break;
            case IF_IR:
                {
                    IR_Operand left_ope = cur_ir->code.u.if_op.left;
                    if(left_ope->kind!=CONSTANT_IR)
                    {
                        tem = var_in_reg(left_ope);
                        if((*check_reg)(tem))
                        {
                            reg_wait[tem] = 0;
                            left_reg_num--;
                            if(left_reg_num <= 1 ) {break;}
                        }
                    }
                    
                    IR_Operand right_ope = cur_ir->code.u.if_op.right;
                    if(right_ope->kind!=CONSTANT_IR)
                    {
                        tem = var_in_reg(right_ope);
                        if((*check_reg)(tem))
                        {
                            reg_wait[tem] = 0;
                            left_reg_num--;
                            if(left_reg_num <= 1 ) {break;}
                        }
                    }

                }
                break;
            case RETURN_IR:
                {
                    IR_Operand return_ope = cur_ir->code.u.return_op.return_operand;
                    if(return_ope->kind!=CONSTANT_IR)
                    {
                        tem = var_in_reg(return_ope);
                        if((*check_reg)(tem))
                        {
                            reg_wait[tem] = 0;
                            left_reg_num--;
                            if(left_reg_num <= 1 ) {break;}
                        }
                    }
                    
                }
                break;
            case ARG_IR:
            case READ_IR:
            case WRITE_IR:
            case PARAM_IR:
                {
                    tem = var_in_reg(cur_ir->code.u.read_write_arg_param_return.place);
                    if((*check_reg)(tem))
                    {
                        reg_wait[tem] = 0;
                        left_reg_num--;
                        if(left_reg_num <= 1 ) {break;}
                    }
                }
                break;
            case CALL_IR:
                {
                    tem = var_in_reg(cur_ir->code.u.func_call.left);
                    if((*check_reg)(tem))
                    {
                        reg_wait[tem] = 0;
                        left_reg_num--;
                        if(left_reg_num <= 1 ) {break;}
                    }
                }
                break;
            default:
            // case LABEL_IR:
            // case GOTO_IR:
            // case FUNC_IR:
            // case DEC_IR:
                // printf("error: switch错误 register\n");
                break;
        }
        // printf("debug: find_latest_use_reg%d\n",left_reg_num);
        cur_ir = cur_ir->nxt;
    }

    int j;
    for( j=0; j < 32;j++)
    {
        if(reg_wait[j]==1)
        {
            return j;
        }
    }
    return -1;
}

// 将某个寄存器中的变量溢出到内存中
void spill_reg(int reg_ID)
{
    int var_hash = reg_occupancy[reg_ID];
    // 该寄存器内没有存变量
    if(var_hash==0)
    {
        return;
    }
    // 之后不在某个寄存器中
    all_var_head[var_hash]->reg_tag = -1;

    // 此处生成一个sw指令，利用偏移量存取，同时要更新reg所存变量的哈希值
    insert_MIP(create_sw_mip(reg_ID,-(FRAME_FIXED_SIZE+4+all_var_head[var_hash]->bia),reg_fp));
    free_reg(reg_ID);
    // use_reg(result,var_hash);// 使用寄存器
}

// 根据变量类型分配不同的寄存器类型
// 分配时若不在寄存器中会从寄存器中取出
// 局部变量：$s
// 临时变量：$t
int allocate(int ir_var_type)
{
    int (*check_reg) (int regID);
    if(ir_var_type==VARIABLE)
    {
        check_reg = if_reg_s;
    }
    else if(ir_var_type==TEMP)
    {
        check_reg = if_open_reg_t;
    }
    else
    {
        printf("error: find_latest_use_reg error\n");
    }
    // 找到空闲寄存器
    int i;
    for(i = 4; i < 32; i++)
    {
        if((*check_reg)(i) && reg_occupancy[i]==0)
        {
            return i;
        }
    }
    int result = find_latest_use_reg(mip_cur_ir,ir_var_type);
    if(result==-1)
    {
        printf("error: allocate error\n");
    }
    spill_reg(result);
    return result;
}

// use_assign表明使用该变量还是赋值该变量
// use——0，assign——1
int ensure(IR_Operand ir_ope,int use_assign)
{
    // printf("debug: 进入ensure\n");
    int tem = var_in_reg(ir_ope);
    int result;
    int var_hash = hash_var_operand(ir_ope);
    if(tem >= 0)
    {
        result = tem;
    }
    else
    {
        result = allocate(ir_ope->kind);

        if(use_assign==0)
        {
            int var_bia = all_var_head[var_hash]->bia;
            // printf("debug: ensure var_hash %d\n",var_hash);
            // printf("debug: ensure var_bia %d\n",var_bia);
            // 此处生成一个lw指令，利用偏移量存储
            insert_MIP(create_lw_mip(result,-(FRAME_FIXED_SIZE+4+var_bia),reg_fp));
        }
    }
    // 更新reg所存变量的哈希值
    use_reg(result,var_hash);// 使用寄存器
    // printf("debug: 退出ensure\n");
    return result;
}

