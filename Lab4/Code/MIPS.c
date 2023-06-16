#include "all.h"

extern InterCodes ir_head;// 存储IR链表，head不存放
extern Var_Desc* all_var_head;

Mips_List mip_head;// 存储MIP链表，head不存放
Mips_List mip_tail;// MIP链表的最后一个元素
int mip_count;
InterCodes mip_cur_ir;// 当前转换到哪一个IR指令
InterCodes cur_func_ir;// 当前语句属于哪一个FUNC_IR

// FUNC_VAR func_var_bottom;// 记录最外层函数内的变量
// FUNC_VAR func_var_top;// 记录在内层函数的变量
Var_List var_bb_head;// 记录一个基本块内的变量，本身不存放
IR_Operand_List arg_list_head;// 自身不存
int param_count;


extern int reg_zero;
extern int reg_at;
extern int reg_v[2];
extern int reg_a[4];
extern int reg_t[10];
extern int reg_s[9];
extern int reg_k[2];
extern int reg_gp;
extern int reg_sp;
extern int reg_fp;
extern int reg_ra;

// 初始化MIP指令生成阶段
void init_MIP_process()
{
    mip_head = empty_MIP();
    mip_tail = mip_head;
    mip_count = 0;
    // var_bb_head = var_list(-1);
    arg_list_head = (IR_Operand_List) malloc(sizeof(struct IR_Operand_List_));
    arg_list_head->nxt = NULL;
    param_count = 0;
    init_all_var_list();
    split_basic_block(ir_head);
    init_all_reg_operand();

}


// 插入new_mip引导的一串MIP指令(包含new_mip)
void insert_MIP(Mips_List new_mip)
{
    // printf("debug: 进入insert_MIP\n");
    // printf("debug: insert_MIP mip_kind=%d\n",new_mip->mip.mip_kind);
    mip_tail->nxt = new_mip;
    mip_tail = new_mip;
    mip_count += 1;
    // if(mip_count >= 5)
    // {
    //     exit(1);
    // }
    // printf("debug: 退出insert_MIP\n");
}

// 初始化一条mip
Mips_List empty_MIP()
{
    Mips_List res = (Mips_List) malloc(sizeof(struct Mips_List_));
    res->mip.mip_kind = nop_MIP;
    int i;
    for(i=0;i<3;i++) 
    {
        res->mip.orerands[i]=NULL;
    }
    // res->prev = NULL;
    res->nxt = NULL;
    return res;
}

// 往head引导的链表中插入new_var
void insert_var_list(Var_List head,Var_List new_var)
{
    Var_List cur_var = head;
    while(cur_var->nxt!=NULL)
    {
        cur_var = cur_var->nxt;
    }
    cur_var->nxt = new_var;

}

// // 清空一条VAR_List，但是head不会free
// void free_var_list(Var_List head)
// {
//     Var_List cur_var = head->nxt;
//     Var_List nxt_var;
//     while(cur_var!=NULL)
//     {
//         nxt_var = cur_var->nxt;
//         free(cur_var);
//         cur_var = nxt_var;
//     }
// }

// 对于非addi和subu指令中IR操作数的处理，返回的是存储最终的寄存器编号
// 如果是ir_ope一个(临时/局部)变量，返回对应寄存器
// 如果是常数，返回$t8或$t9，后面需要立即用到$t8或$t9，以免$t8或$t9中的值被替代掉
// choice就是表明用哪个寄存器：$t8——0，$t9——1
int IR_ope2MIP_ope(IR_Operand ir_ope,int choice)
{
    int res_ID;
    // printf("%d\n",ir_ope==NULL);
    int ir_ope_kind = ir_ope->kind;
    
    // 根据choice
    int inter_reg;
    if(choice==0)
    {
        inter_reg = reg_t[8];
    }
    else
    {
        inter_reg = reg_t[9];
    }
    // #k
    if(ir_ope_kind == CONSTANT_IR)
    {
        // printf("debug: IR_ope2MIP_ope CONSTANT\n");
        // 使用$at作为过渡寄存器
        if(choice==0)
        {
            insert_MIP(create_li_mip(inter_reg,ir_ope->u.value));
            return inter_reg;
        }
        // 使用$v1作为过渡
        else
        {
            insert_MIP(create_li_mip(inter_reg,ir_ope->u.value));
            return inter_reg;
        }
    }
    // x
    else if(ir_ope_kind == VARIABLE || ir_ope_kind == TEMP)
    {
        // printf("debug: IR_ope2MIP_ope VAR\n");
        // *x
        if(ir_ope->if_deref)
        {
            int x_reg = ensure(ir_ope,0);
            insert_MIP(create_lw_mip(inter_reg,0,x_reg));
            return inter_reg;
        }
        // &x
        else if(ir_ope->if_take_addr)
        {
            // 创建一个addi指令：addi reg(t), reg(fp), offset_x
            int var_hash = hash_var_operand(ir_ope);
            insert_MIP(create_addi_mip(inter_reg,reg_fp,-(FRAME_FIXED_SIZE+4+all_var_head[var_hash]->bia)));
            return inter_reg;
        }
        // x
        else
        {
            return ensure(ir_ope,0);
        }
    }
    else
    {
        printf("error: IR_ope2MIP_ope错误\n");
    }
    return -1;
}

// TODO：如果变量在后面都没用到，free对应寄存器
// 单条IR转成MIP指令
void IR2MIP(InterCodes cur_ir)
{
    if(cur_ir->if_block_head)
    {
        free_all_regs();
        // free_VAR_LIST(var_bb_head);
    }
    InterCodes next_ir = cur_ir->nxt;// 是否窥孔
    int ir_kind = cur_ir->code.ir_kind;
    
    switch(ir_kind)
    {
        case ASSIGN_IR:
            {

                // printf("debug: IR2MIP ASSIGN_IR\n");
                IR_Operand left_operand = cur_ir->code.u.assign.left;
                IR_Operand right_operand = cur_ir->code.u.assign.right;

                
                int left_hash = hash_var_operand(left_operand);

                // *x = ...
                if(left_operand->if_deref)
                {
                    int left_reg  = ensure(left_operand,0);
                    // printf("debug: IR2MIP ASSIGN_IR 解引用\n");
                    // 不会出现*x = &y
                    // *x = #k
                    if(right_operand->kind==CONSTANT_IR)
                    {
                        // li reg(x), k
                        int const_reg = IR_ope2MIP_ope(right_operand,0);                        

                        // sw const_reg, offset($fp)
                        
                        insert_MIP(create_sw_mip(const_reg,0,left_reg)); 
                        
                    }
                    // *x = y
                    else
                    {
                        // printf("debug: find if_const 3:%d\n",right_operand->kind==CONSTANT_IR);
                        int right_reg = ensure(right_operand,0);
                        insert_MIP(create_sw_mip(right_reg,0,left_reg)); 
                    }

                }
                // x := ，不会出现&x = 
                else
                {
                    int left_reg  = ensure(left_operand,1);
                    // printf("debug: IR2MIP ASSIGN_IR 变量赋值\n");
                    // 不会出现 x =: &y
                    // x := #k
                    if(right_operand->kind==CONSTANT_IR)
                    {
                        // printf("debug: IR2MIP ASSIGN_IR 变量赋值 常数\n");
                        // printf("debug: %d\n",right_operand->u.value);
                        // li reg(x), k
                        insert_MIP(create_li_mip(left_reg,right_operand->u.value));
                        // printf("debug: IR2MIP ASSIGN_IR 变量赋值 常数2\n");
                    }
                    // x := y
                    else
                    {
                        // move reg(x), reg(y)
                        insert_MIP(create_move_mip(left_reg,IR_ope2MIP_ope(right_operand,0)));
                    }
                }    
            }
            
            break; 
        case ADD_IR:
            {
                // printf("debug: IR2MIP ADD_IR\n");
                IR_Operand result_operand = cur_ir->code.u.binary_op.result;
                IR_Operand left_operand = cur_ir->code.u.binary_op.op1;
                IR_Operand right_operand = cur_ir->code.u.binary_op.op2;

                int left_cons = left_operand->kind==CONSTANT_IR;
                int right_cons = right_operand->kind==CONSTANT_IR;
                // printf("debug: right_cons %d\n",right_cons);

                Mips_List res_mip = empty_MIP();
                // 两个都是常数
                if(left_cons && right_cons)
                {   
                    // 使用汇编器保留寄存器存储中间结果，再进行赋值
                    // li $at, k_r
                    int inter_reg = IR_ope2MIP_ope(left_operand,0);
                    // addi reg(x),$at, k_r
                    
                    insert_MIP(create_addi_mip(ensure(result_operand,1),inter_reg,right_operand->u.value));
                }
                // 左边是常数
                else if(left_cons)
                {
                    // 处理右边的变量
                    int inter_reg = IR_ope2MIP_ope(right_operand,0);
                    // addi reg(x),inter_reg, k_l
                    
                    insert_MIP(create_addi_mip(ensure(result_operand,1),inter_reg,left_operand->u.value));
                }
                // 右边是常数
                else if(right_cons)
                {
                    // printf("debug: 右边是常数\n");
                    // 处理左边的变量
                    int inter_reg = IR_ope2MIP_ope(left_operand,0);
                    // addi reg(x),inter_reg, k_l
                    insert_MIP(create_addi_mip(ensure(result_operand,1),inter_reg,right_operand->u.value));
                }
                // 全为变量
                else
                {
                    // printf("debug: 全部是变量\n");
                    int result_reg,left_reg,right_reg;
                    
                    result_reg = ensure(result_operand,1);
                    left_reg = IR_ope2MIP_ope(left_operand,0);
                    right_reg = IR_ope2MIP_ope(right_operand,1);

                    insert_MIP(create_add_mip(result_reg,left_reg,right_reg));
                }
            }
            break;
        case SUB_IR:
            {
                // printf("debug: IR2MIP SUB_IR\n");
                IR_Operand result_operand = cur_ir->code.u.binary_op.result;
                IR_Operand left_operand = cur_ir->code.u.binary_op.op1;
                IR_Operand right_operand = cur_ir->code.u.binary_op.op2;

                int left_cons = left_operand->kind==CONSTANT_IR;
                int right_cons = right_operand->kind==CONSTANT_IR;

                Mips_List res_mip = empty_MIP();
                // 两个都是常数
                if(left_cons && right_cons)
                {   
                    // 使用汇编器保留寄存器存储中间结果，再进行赋值
                    // li $at, k_r
                    int inter_reg = IR_ope2MIP_ope(left_operand,0);
                    // addi reg(x),$at, k_r
                    
                    insert_MIP(create_addi_mip(ensure(result_operand,1),inter_reg,-right_operand->u.value));
                }
                // 左边是常数
                else if(left_cons)
                {
                    int result_reg,left_reg,right_reg;
                    
                    result_reg = ensure(result_operand,1);// 实际用不到第二个实参
                    left_reg = IR_ope2MIP_ope(left_operand,0);
                    right_reg = IR_ope2MIP_ope(right_operand,1);
                    
                    insert_MIP(create_sub_mip(result_reg,left_reg,right_reg));
                }
                // 右边是常数
                else if(right_cons)
                {
                    // 处理左边的变量
                    int inter_reg = IR_ope2MIP_ope(left_operand,0);
                    // addi reg(x),inter_reg, k_l
                    
                    insert_MIP(create_addi_mip(ensure(result_operand,1),inter_reg,-right_operand->u.value));
                }
                // 全为变量
                else
                {
                    int result_reg,left_reg,right_reg;
                    
                    result_reg = ensure(result_operand,1);
                    left_reg = IR_ope2MIP_ope(left_operand,0);
                    right_reg = IR_ope2MIP_ope(right_operand,1);

                    insert_MIP(create_sub_mip(result_reg,left_reg,right_reg));
                }
            }
            break;
        case MUL_IR:
            {
                // printf("debug: IR2MIP MUL_IR\n");
                IR_Operand result_operand = cur_ir->code.u.binary_op.result;
                IR_Operand left_operand = cur_ir->code.u.binary_op.op1;
                IR_Operand right_operand = cur_ir->code.u.binary_op.op2;
                Mips_List res_mip = empty_MIP();

                int left_reg = IR_ope2MIP_ope(left_operand,0);
                int right_reg = IR_ope2MIP_ope(right_operand,1);
                
                insert_MIP(create_mul_mip(ensure(result_operand,1),left_reg,right_reg));
            }
            break;
        case DIV_IR:
            {
                // printf("debug: IR2MIP DIV_IR\n");
                IR_Operand result_operand = cur_ir->code.u.binary_op.result;
                IR_Operand left_operand = cur_ir->code.u.binary_op.op1;
                IR_Operand right_operand = cur_ir->code.u.binary_op.op2;
                Mips_List res_mip = empty_MIP();
   
                int result_reg = ensure(result_operand,1);
                int left_reg = IR_ope2MIP_ope(left_operand,0);
                int right_reg = IR_ope2MIP_ope(right_operand,1);
                
                insert_MIP(create_div_mip(result_reg,left_reg,right_reg));

                insert_MIP(create_mflo_mip(result_reg));

            }
            break;
        case LABEL_IR:
            {
                // printf("debug: IR2MIP LABEL_IR\n");
                Mips_List label_mip = empty_MIP();
                label_mip->mip.mip_kind = label_MIP;
                label_mip->mip.orerands[0] = const_MIP_operand(cur_ir->code.u.if_op.label_no,1);
                insert_MIP(label_mip);
            }
            
            break;
        case GOTO_IR:
            {
                // printf("debug: IR2MIP GOTO_IR\n");
                Mips_List goto_mip = empty_MIP();
                goto_mip->mip.mip_kind = j_MIP;
                goto_mip->mip.orerands[0] = const_MIP_operand(cur_ir->code.u.if_op.label_no,1);
                insert_MIP(goto_mip);
            }
            break;
        case IF_IR:
            {
                // printf("debug: IR2MIP IF_IR\n");
                insert_MIP(IF_IR2MIP(cur_ir));
            }
            
            break;
        case RETURN_IR:
            {
                // printf("debug: IR2MIP RETURN_IR\n");
                // printf("debug: find if_const 4:%d\n",result_operand->kind==CONSTANT_IR);
                insert_MIP(create_move_mip(reg_v[0],IR_ope2MIP_ope(cur_ir->code.u.return_op.return_operand,0)));
                
                // Epilogue
                // lw reg, offset($fp)
                int i = 0;
                for(;i < 8;i++)
                {
                    insert_MIP(create_lw_mip(reg_s[i],-(48+4+4*i),reg_fp));
                }

                // lw $ra, offset($fp)
                insert_MIP(create_lw_mip(reg_ra,-4,reg_fp));
                // lw $fp, offset($fp)
                insert_MIP(create_lw_mip(reg_fp,-8,reg_fp));

                // jr $ra
                Mips_List jr_mip = empty_MIP();
                jr_mip->mip.mip_kind = jr_MIP;
                jr_mip->mip.orerands[0] = get_reg_ope_by_ID(reg_ra);

                insert_MIP(jr_mip);
            }
            break;
        case ARG_IR:
            {
                // printf("debug: IR2MIP ARG_IR\n");
                // 先把参数用一个链表存起来，到CALL_IR的时候再读取出来
                // 倒序添加
                IR_Operand_List tem = (IR_Operand_List) malloc(sizeof(struct IR_Operand_List_));
                tem->ope = cur_ir->code.u.read_write_arg_param_return.place;
                tem->nxt = arg_list_head->nxt;
                arg_list_head->nxt = tem;
            }
            
            break;
        case CALL_IR:
        case WRITE_IR:
        case READ_IR:
            {
                // printf("debug: IR2MIP CALL_IR\n");

                // 保存$t，使用$fp
                int i = 0;
                if(ir_kind!=WRITE_IR && ir_kind!=READ_IR)
                {
                    for(;i < 10; i++)
                    {
                        // 这里不应使用spill_reg()，spill_reg()是把寄存器内的值放到变量区
                        // 而是把值放到保留的寄存器区，计算偏移值
                        insert_MIP(create_sw_mip(reg_t[i],-8-4-4*i,reg_fp));
                    }
                }
                
                // printf("debug: IR2MIP CALL_IR 1\n");

                int arg_count = 0;// 数现在到哪儿实参，仅CALL_IR使用

                if(ir_kind==WRITE_IR)
                {
                    insert_MIP(create_move_mip(reg_a[0],IR_ope2MIP_ope(cur_ir->code.u.read_write_arg_param_return.place,0)));
                    // 跳转并把地址保留到$ra中
                    Mips_List jal_mip = empty_MIP();
                    jal_mip->mip.mip_kind = jal_MIP;
                    jal_mip->mip.orerands[0] = func_MIP_operand("write");
                    insert_MIP(jal_mip);
                }
                else if(ir_kind==READ_IR)
                {
                    // insert_MIP(create_move_mip(reg_a[0],IR_ope2MIP_ope(cur_ir->code.u.read_write_arg_param_return.place,0)));
                    // 跳转并把地址保留到$ra中
                    Mips_List jal_mip = empty_MIP();
                    jal_mip->mip.mip_kind = jal_MIP;
                    jal_mip->mip.orerands[0] = func_MIP_operand("read");
                    insert_MIP(jal_mip);
                }
                else
                {
                    // printf("debug: IR2MIP CALL_IR 普通函数\n");
                    IR_Operand_List cur_arg = arg_list_head->nxt;
                    
                    // printf("debug: IR2MIP CALL_IR 普通函数-1\n");
                    // 实参数目 <= 4
                    while(arg_count < 4 && cur_arg!=NULL)
                    {
                        int inter_reg = IR_ope2MIP_ope(cur_arg->ope,0);
                        insert_MIP(create_move_mip(reg_a[arg_count],inter_reg));
                        arg_count += 1;
                        cur_arg = cur_arg->nxt;
                        // printf("debug: arg_count %d\n",arg_count);
                    }
                    
                    // printf("debug: IR2MIP CALL_IR 普通函数-2\n");
                    // 实参数目 > 4
                    if(cur_arg!=NULL)
                    {
                        IR_Operand_List tem_arg = cur_arg;
                        while(tem_arg!=NULL)
                        {
                            arg_count += 1;
                            tem_arg = tem_arg->nxt;
                        }
                        // 对应：subu $sp, $sp, max{0, 4 * (n-5)}
                        if(arg_count > 5)
                        {
                            insert_MIP(create_subu_mip(reg_sp,reg_sp,4*(arg_count-5)));
                        }
                        

                        while(cur_arg!=NULL)
                        {
                            insert_MIP(create_sw_mip(IR_ope2MIP_ope(cur_arg->ope,0),4*(arg_count-5),reg_sp));
                            i += 1;
                            cur_arg = cur_arg->nxt;
                        }
                    }
                    
                    // 清空实参链表，留到下一次使用

                    // free_VAR_LIST(arg_list_head);
                    clear_IR_Operand_List(arg_list_head);

                    // 跳转并把地址保留到$ra中
                    Mips_List jal_mip = empty_MIP();
                    jal_mip->mip.mip_kind = jal_MIP;
                    jal_mip->mip.orerands[0] = func_MIP_operand(cur_ir->code.u.func_call.func);
                    insert_MIP(jal_mip);
                }

                // printf("debug: IR2MIP CALL_IR 2\n");
                

                // 恢复sp指针
                if(arg_count>=5)
                {
                    insert_MIP(create_addi_mip(reg_sp,reg_sp,4*(arg_count-5)));
                }

                if(ir_kind!=WRITE_IR && ir_kind!=READ_IR)
                {
                    // 恢复寄存器$t?(8),使用$fp
                    i = 0;
                    for(;i < 10; i++)
                    {
                        // 把值从保留的寄存器区移回寄存器，计算偏移值
                        // lw $t?, offset(fp)
                        insert_MIP(create_lw_mip(reg_t[i],-8-4-4*i,reg_fp));
                    }
                }

                // 把返回值移到对应的变量中
                if(ir_kind==CALL_IR)
                {   
                    // place存储返回值，即进行赋值
                    insert_MIP(create_move_mip(ensure(cur_ir->code.u.func_call.left,1),reg_v[0]));
                }
                else if(ir_kind==READ_IR)
                {
                    // place存储返回值，即进行赋值
                    insert_MIP(create_move_mip(ensure(cur_ir->code.u.read_write_arg_param_return.place,1),reg_v[0]));
                }

                
            }
            break;
        case FUNC_IR:
            {
                // printf("debug: IR2MIP FUNC_IR\n");
                Mips_List func_mip = empty_MIP();
                func_mip->mip.mip_kind = func_MIP;
                func_mip->mip.orerands[0] = func_MIP_operand(cur_ir->code.u.func_call.func); 

                insert_MIP(func_mip);


                // printf("func_name: --%s--\n",cur_ir->code.u.func_call.func);

                // main函数是整个程序的入口，不用做以下操作
                if(1)
                // if(strcmp(cur_ir->code.u.func_call.func,"main")!=0)
                {
                    // Prologue
                    int cur_func_frame_size = cur_func_ir->code.u.func_call.frame_size;
                    // 移动$sp：subu $sp, $sp, framesize                
                    insert_MIP(create_subu_mip(reg_sp,reg_sp,cur_func_frame_size));

                    // 保存旧$ra中的值：sw $ra, (framesize-4)($sp)
                    insert_MIP(create_sw_mip(reg_ra,cur_func_frame_size-4,reg_sp));

                    // 保存旧$fp中的值
                    insert_MIP(create_sw_mip(reg_fp,cur_func_frame_size-8,reg_sp));

                    // 获取当前$fp：addi $fp, $sp, framesize
                    insert_MIP(create_addi_mip(reg_fp,reg_sp,cur_func_frame_size));

                    // 保存寄存器
                    int i = 0;
                    for(;i < 8 ;i++)
                    {
                        // 48 + 4*i
                        insert_MIP(create_sw_mip(reg_s[i],-48-4-4*i,reg_fp));
                    }
                }
                // 加载参数留到param中做，在param_ir才知道参数是什么
    
            }
            break;
        case PARAM_IR:
            {
                // printf("debug: IR2MIP PARAM_IR\n");
                // 获取形参变量
                IR_Operand param_ope = cur_ir->code.u.read_write_arg_param_return.place;
                int var_hash = hash_var_operand(param_ope);

                param_count += 1;
                // 所有参数直接先存到对应的变量内存空间中

                // 小于等于4个的参数
                if(param_count <= 4 )
                {

                    // sw $a?, (offset)$fp
                    insert_MIP(create_sw_mip(reg_a[param_count-1],-(FRAME_FIXED_SIZE+4+all_var_head[var_hash]->bia),reg_fp));

                }
                // 大于4个的参数
                else
                {
                    // 先取到v1中，再存到对应内存中
                    insert_MIP(create_lw_mip(reg_v[1],(param_count-5)*4,reg_fp));
                    
                    insert_MIP(create_sw_mip(reg_v[1],-(FRAME_FIXED_SIZE+4+all_var_head[var_hash]->bia),reg_fp));
                }
                 
            }
            break;
        case DEC_IR:
            // 啥也不做
            break;
        default:
        
            printf("error: switch错误1\n");
            break;
    }
}

// 将所有的IR(ir_head指引)转换成MIP
void IRs2MIPs(InterCodes head)
{
    if(head==NULL)
    {
        printf("error:翻译IR时IR链表为空1\n");
        return;
    }
    InterCodes cur_ir = head->nxt;
    if(cur_ir->code.ir_kind==FUNC_IR)
    {
        cur_func_ir = cur_ir;
    }
    mip_cur_ir = cur_ir;
    if(cur_ir==NULL)
    {
        printf("error:翻译IR时IR链表为空2\n");
        return;
    }
    
    // 直接在哪里生成就在哪里插入，免得弄混 
    InterCodes next_ir;
    while(cur_ir!=NULL)
    {
        next_ir = cur_ir->nxt;
        int if_jump = (cur_ir->code.ir_kind==GOTO_IR || cur_ir->code.ir_kind==IF_IR);
        // 基本块结尾，溢出所有寄存器，针对跳转语句结尾，在跳转前就要溢出
        if(next_ir!=NULL && next_ir->if_block_head==1  && if_jump==1)
        {
            // printf("debug:这里 %d\n",cur_ir->code.ir_kind);
            int i = 0;
            for(;i < 32 ;i++)
            {
                spill_reg(i);
            }
        }

        // printf("debug: IRs2MIPs 一条开始\n");
        if(cur_ir->code.ir_kind==FUNC_IR)
        {
            cur_func_ir = cur_ir;
        }
        
        // insert_MIPs(IR2MIP(cur_ir)); 
        IR2MIP(cur_ir);// 这个函数中会直接完成MIP链表的插入

        // 基本块结尾，溢出所有寄存器，针对非跳转语句结尾，在跳转前就要溢出
        if(next_ir!=NULL && next_ir->if_block_head==1  && if_jump==0)
        {
            int i = 0;
            for(;i < 32 ;i++)
            {
                spill_reg(i);
            }
        }

        cur_ir = next_ir;
        

        mip_cur_ir =cur_ir;
        // printf("debug: IRs2MIPs 一条结束\n");
    }
}

// IF_IR转成MIP
Mips_List IF_IR2MIP(InterCodes if_ir)
{
    Mips_List tar_mip = empty_MIP();
    char* rel_op =  if_ir->code.u.if_op.relop;
    if(strcmp(rel_op,"==")==0)
    {
        // printf("debug: IF_IR2MIP ==\n");
        tar_mip->mip.mip_kind = beq_MIP;
        tar_mip->mip.orerands[0] = get_reg_ope_by_ID(IR_ope2MIP_ope(if_ir->code.u.if_op.left,0));
        tar_mip->mip.orerands[1] = get_reg_ope_by_ID(IR_ope2MIP_ope(if_ir->code.u.if_op.right,1));
        tar_mip->mip.orerands[2] = const_MIP_operand(if_ir->code.u.if_op.label_no,1);
    }
    else if(strcmp(rel_op,"!=")==0)
    {
        // printf("debug: IF_IR2MIP !=\n");
        tar_mip->mip.mip_kind = bne_MIP;
        tar_mip->mip.orerands[0] = get_reg_ope_by_ID(IR_ope2MIP_ope(if_ir->code.u.if_op.left,0));
        tar_mip->mip.orerands[1] = get_reg_ope_by_ID(IR_ope2MIP_ope(if_ir->code.u.if_op.right,1));
        tar_mip->mip.orerands[2] = const_MIP_operand(if_ir->code.u.if_op.label_no,1);
    }
    else if(strcmp(rel_op,">")==0)
    {
        // printf("debug: IF_IR2MIP >\n");
        tar_mip->mip.mip_kind = bgt_MIP;
        tar_mip->mip.orerands[0] = get_reg_ope_by_ID(IR_ope2MIP_ope(if_ir->code.u.if_op.left,0));
        tar_mip->mip.orerands[1] = get_reg_ope_by_ID(IR_ope2MIP_ope(if_ir->code.u.if_op.right,1));
        tar_mip->mip.orerands[2] = const_MIP_operand(if_ir->code.u.if_op.label_no,1);
    }
    else if(strcmp(rel_op,"<")==0)
    {
        // printf("debug: IF_IR2MIP <\n");
        tar_mip->mip.mip_kind = blt_MIP;
        int left_reg = IR_ope2MIP_ope(if_ir->code.u.if_op.left,0);
        // printf("%d\n",left_reg);
        tar_mip->mip.orerands[0] = get_reg_ope_by_ID(left_reg);
        tar_mip->mip.orerands[1] = get_reg_ope_by_ID(IR_ope2MIP_ope(if_ir->code.u.if_op.right,1));
        tar_mip->mip.orerands[2] = const_MIP_operand(if_ir->code.u.if_op.label_no,1);
    }
    else if(strcmp(rel_op,">=")==0)
    {
        // printf("debug: IF_IR2MIP >=\n");
        tar_mip->mip.mip_kind = bge_MIP;
        tar_mip->mip.orerands[0] = get_reg_ope_by_ID(IR_ope2MIP_ope(if_ir->code.u.if_op.left,0));
        tar_mip->mip.orerands[1] = get_reg_ope_by_ID(IR_ope2MIP_ope(if_ir->code.u.if_op.right,1));
        tar_mip->mip.orerands[2] = const_MIP_operand(if_ir->code.u.if_op.label_no,1);
    }
    else if(strcmp(rel_op,"<=")==0)
    {
        // printf("debug: IF_IR2MIP <=\n");
        tar_mip->mip.mip_kind = ble_MIP;
        tar_mip->mip.orerands[0] = get_reg_ope_by_ID(IR_ope2MIP_ope(if_ir->code.u.if_op.left,0));
        tar_mip->mip.orerands[1] = get_reg_ope_by_ID(IR_ope2MIP_ope(if_ir->code.u.if_op.right,1));
        tar_mip->mip.orerands[2] = const_MIP_operand(if_ir->code.u.if_op.label_no,1);
    }
    else
    {
        printf("error: IF_IR2MIP的关系运算符错误\n");
    }
    return tar_mip;
}

Mips_List create_lw_mip(int reg_ID,int bia,int base_reg_ID)
{
    Mips_List lw_mip = empty_MIP();
    lw_mip->mip.mip_kind = lw_MIP;
    lw_mip->mip.orerands[0] = get_reg_ope_by_ID(reg_ID);
    lw_mip->mip.orerands[1] = const_MIP_operand(bia,0);
    lw_mip->mip.orerands[2] = get_reg_ope_by_ID(base_reg_ID);
    return lw_mip;
}

Mips_List create_sw_mip(int reg_ID,int bia,int base_reg_ID)
{
    Mips_List sw_mip = empty_MIP();
    sw_mip->mip.mip_kind = sw_MIP;
    sw_mip->mip.orerands[0] = get_reg_ope_by_ID(reg_ID);
    sw_mip->mip.orerands[1] = const_MIP_operand(bia,0);
    sw_mip->mip.orerands[2] = get_reg_ope_by_ID(base_reg_ID);
    return sw_mip;
}

Mips_List create_addi_mip(int dst_ID,int src_ID,int const_val)
{
    Mips_List addi_mip = empty_MIP();
    addi_mip->mip.mip_kind = addi_MIP;
    addi_mip->mip.orerands[0] = get_reg_ope_by_ID(dst_ID);
    addi_mip->mip.orerands[1] = get_reg_ope_by_ID(src_ID);
    addi_mip->mip.orerands[2] = const_MIP_operand(const_val,0);
    return addi_mip;
}

Mips_List create_subu_mip(int dst_ID,int src_ID,int const_val)
{
    Mips_List subu_mip = empty_MIP();
    subu_mip->mip.mip_kind = subu_MIP;
    subu_mip->mip.orerands[0] = get_reg_ope_by_ID(dst_ID);
    subu_mip->mip.orerands[1] = get_reg_ope_by_ID(src_ID);
    subu_mip->mip.orerands[2] = const_MIP_operand(const_val,0);
    return subu_mip;
}

Mips_List create_move_mip(int dst_ID,int src_ID)
{
    Mips_List move_mip = empty_MIP();
    move_mip->mip.mip_kind = move_MIP;
    move_mip->mip.orerands[0] = get_reg_ope_by_ID(dst_ID);
    move_mip->mip.orerands[1] = get_reg_ope_by_ID(src_ID);
    return move_mip;
}

Mips_List create_li_mip(int dst_ID,int const_val)
{
    Mips_List li_mip = empty_MIP();
    li_mip->mip.mip_kind = li_MIP;
    li_mip->mip.orerands[0] = get_reg_ope_by_ID(dst_ID);
    li_mip->mip.orerands[1] = const_MIP_operand(const_val,0);

    return li_mip;
}

Mips_List create_add_mip(int result_ID,int left_ID,int right_ID)
{
    Mips_List add_mip = empty_MIP();
    add_mip->mip.mip_kind = add_MIP;
    add_mip->mip.orerands[0] = get_reg_ope_by_ID(result_ID);
    add_mip->mip.orerands[1] = get_reg_ope_by_ID(left_ID);
    add_mip->mip.orerands[2] = get_reg_ope_by_ID(right_ID);
    return add_mip;
}

Mips_List create_sub_mip(int result_ID,int left_ID,int right_ID)
{
    Mips_List sub_mip = empty_MIP();
    sub_mip->mip.mip_kind = sub_MIP;
    sub_mip->mip.orerands[0] = get_reg_ope_by_ID(result_ID);
    sub_mip->mip.orerands[1] = get_reg_ope_by_ID(left_ID);
    sub_mip->mip.orerands[2] = get_reg_ope_by_ID(right_ID);
    return sub_mip;
}

Mips_List create_mul_mip(int result_ID,int left_ID,int right_ID)
{
    Mips_List mul_mip = empty_MIP();
    mul_mip->mip.mip_kind = mul_MIP;
    mul_mip->mip.orerands[0] = get_reg_ope_by_ID(result_ID);
    mul_mip->mip.orerands[1] = get_reg_ope_by_ID(left_ID);
    mul_mip->mip.orerands[2] = get_reg_ope_by_ID(right_ID);
    return mul_mip;
}

Mips_List create_div_mip(int result_ID,int left_ID,int right_ID)
{
    Mips_List div_mip = empty_MIP();
    div_mip->mip.mip_kind = div_MIP;
    div_mip->mip.orerands[0] = get_reg_ope_by_ID(result_ID);
    div_mip->mip.orerands[1] = get_reg_ope_by_ID(left_ID);
    div_mip->mip.orerands[2] = get_reg_ope_by_ID(right_ID);
    return div_mip;
}

Mips_List create_mflo_mip(int reg_ID)
{
    Mips_List mflo_mip = empty_MIP();
    mflo_mip->mip.mip_kind = mflo_MIP;
    mflo_mip->mip.orerands[0] = get_reg_ope_by_ID(reg_ID);
    return mflo_mip;
}

void output_env(char* file_name)
{
    FILE* MIP_file;
    MIP_file = fopen(file_name,"w");
    fprintf(MIP_file,".data\n_prompt: .asciiz \"Enter an integer:\"\n_ret: .asciiz \"\\n\"\n.globl main\n.text\n");
    // 伪指令
    fprintf(MIP_file,"read:\n  li $v0, 4\n  la $a0, _prompt\n  syscall\n  li $v0, 5\n  syscall\n  jr $ra\n\n");
    fprintf(MIP_file,"write:\n  li $v0, 1\n  syscall\n  li $v0, 4\n  la $a0, _ret\n  syscall\n  move $v0, $0\n  jr $ra\n");
    // 真实指令
    // fprintf(MIP_file,"read:\n  ori $v0, $0, 4\n  lui $a0, _prompt\n  syscall\n  ori $v0, $0, 5\n  syscall\n  jr $ra\n\n");
    // fprintf(MIP_file,"write:\n  ori $v0, $0, 1\n  syscall\n  ori $v0, $0, 4\n  lui $a0, _ret\n  syscall\n  move $v0, $0\n  jr $ra\n");
    fclose(MIP_file);
}

// 打印MIP指令
void output_MIPs(Mips_List head,char* file_name)
{
    FILE* MIP_file;
    // MIP_file = fopen(file_name,"a");
    Mips_List cur_mip = mip_head->nxt;

    while(cur_mip!=NULL)
    {
        // printf("debug: one mip start\n");
        int mip_kind = cur_mip->mip.mip_kind;
        // printf("debug: output_mips MIP_type:%d\n",mip_kind);
        MIP_file = fopen(file_name,"a");
        if(mip_kind!=label_MIP && mip_kind!=func_MIP)
        {
            fprintf(MIP_file,"  ");
        }
        switch (mip_kind)
        {
            // 一个操作数
            case label_MIP:
                fprintf(MIP_file,"%s:",MIP_ope2str(cur_mip->mip.orerands[0]));
                break; 
            case func_MIP:
                fprintf(MIP_file,"\n%s:",MIP_ope2str(cur_mip->mip.orerands[0]));
                break;
            case mflo_MIP:
                fprintf(MIP_file,"mflo %s",MIP_ope2str(cur_mip->mip.orerands[0]));
                break; 
            case j_MIP:
                fprintf(MIP_file,"j %s",MIP_ope2str(cur_mip->mip.orerands[0]));
                break;
            case jal_MIP:
                fprintf(MIP_file,"jal %s",MIP_ope2str(cur_mip->mip.orerands[0]));
                break;
            case jr_MIP:
                fprintf(MIP_file,"jr %s",MIP_ope2str(cur_mip->mip.orerands[0]));
                break;
            // 两个操作数
            case li_MIP:
                // 伪指令
                fprintf(MIP_file,"li %s, %s",MIP_ope2str(cur_mip->mip.orerands[0]),MIP_ope2str(cur_mip->mip.orerands[1]));
                // 真实指令
                // fprintf(MIP_file,"ori %s, $0, %s",MIP_ope2str(cur_mip->mip.orerands[0]),MIP_ope2str(cur_mip->mip.orerands[1]));
                break;
            case move_MIP:
                // 伪指令
                fprintf(MIP_file,"move %s, %s",MIP_ope2str(cur_mip->mip.orerands[0]),MIP_ope2str(cur_mip->mip.orerands[1]));
                // 真实指令
                // fprintf(MIP_file,"addu %s, %s, $0",MIP_ope2str(cur_mip->mip.orerands[0]),MIP_ope2str(cur_mip->mip.orerands[1]));
                break;
            
            // 三个操作数
            case lw_MIP:
                fprintf(MIP_file,"lw %s, %s(%s)",MIP_ope2str(cur_mip->mip.orerands[0]),MIP_ope2str(cur_mip->mip.orerands[1]),MIP_ope2str(cur_mip->mip.orerands[2]));
                break;
            case sw_MIP:
                fprintf(MIP_file,"sw %s, %s(%s)",MIP_ope2str(cur_mip->mip.orerands[0]),MIP_ope2str(cur_mip->mip.orerands[1]),MIP_ope2str(cur_mip->mip.orerands[2]));
                break;
            case addi_MIP:
                fprintf(MIP_file,"addi %s, %s, %s",MIP_ope2str(cur_mip->mip.orerands[0]),MIP_ope2str(cur_mip->mip.orerands[1]),MIP_ope2str(cur_mip->mip.orerands[2]));
                break;
            case subu_MIP:
                fprintf(MIP_file,"subu %s, %s, %s",MIP_ope2str(cur_mip->mip.orerands[0]),MIP_ope2str(cur_mip->mip.orerands[1]),MIP_ope2str(cur_mip->mip.orerands[2]));
                break;
            case add_MIP:
                fprintf(MIP_file,"add %s, %s, %s",MIP_ope2str(cur_mip->mip.orerands[0]),MIP_ope2str(cur_mip->mip.orerands[1]),MIP_ope2str(cur_mip->mip.orerands[2]));
                break;
            case sub_MIP:
                fprintf(MIP_file,"sub %s, %s, %s",MIP_ope2str(cur_mip->mip.orerands[0]),MIP_ope2str(cur_mip->mip.orerands[1]),MIP_ope2str(cur_mip->mip.orerands[2]));
                break;
            case mul_MIP:
                fprintf(MIP_file,"mul %s, %s, %s",MIP_ope2str(cur_mip->mip.orerands[0]),MIP_ope2str(cur_mip->mip.orerands[1]),MIP_ope2str(cur_mip->mip.orerands[2]));
                break;
            case div_MIP:
                fprintf(MIP_file,"div %s, %s, %s",MIP_ope2str(cur_mip->mip.orerands[0]),MIP_ope2str(cur_mip->mip.orerands[1]),MIP_ope2str(cur_mip->mip.orerands[2]));
                break;
            case beq_MIP:
                fprintf(MIP_file,"beq %s, %s, %s",MIP_ope2str(cur_mip->mip.orerands[0]),MIP_ope2str(cur_mip->mip.orerands[1]),MIP_ope2str(cur_mip->mip.orerands[2]));
                break;
            case bne_MIP:
                fprintf(MIP_file,"bne %s, %s, %s",MIP_ope2str(cur_mip->mip.orerands[0]),MIP_ope2str(cur_mip->mip.orerands[1]),MIP_ope2str(cur_mip->mip.orerands[2]));
                break;
            case bgt_MIP:
                // 伪指令
                fprintf(MIP_file,"bgt %s, %s, %s",MIP_ope2str(cur_mip->mip.orerands[0]),MIP_ope2str(cur_mip->mip.orerands[1]),MIP_ope2str(cur_mip->mip.orerands[2]));
                // 真实指令
                // fprintf(MIP_file,"slt $1, %s, %s\n",MIP_ope2str(cur_mip->mip.orerands[0]),MIP_ope2str(cur_mip->mip.orerands[1]));
                // fprintf(MIP_file,"  bne $1, $0, %s",MIP_ope2str(cur_mip->mip.orerands[2]));
                break;
            case blt_MIP:
                // 伪指令
                fprintf(MIP_file,"blt %s, %s, %s",MIP_ope2str(cur_mip->mip.orerands[0]),MIP_ope2str(cur_mip->mip.orerands[1]),MIP_ope2str(cur_mip->mip.orerands[2]));
                // 真实指令
                // fprintf(MIP_file,"sle $1, %s, %s\n",MIP_ope2str(cur_mip->mip.orerands[0]),MIP_ope2str(cur_mip->mip.orerands[1]));
                // fprintf(MIP_file,"  bne $1, $0, %s",MIP_ope2str(cur_mip->mip.orerands[2]));
                break;
            case bge_MIP:
                // 伪指令
                fprintf(MIP_file,"bge %s, %s, %s",MIP_ope2str(cur_mip->mip.orerands[0]),MIP_ope2str(cur_mip->mip.orerands[1]),MIP_ope2str(cur_mip->mip.orerands[2]));
                // 真实指令
                // fprintf(MIP_file,"sgt $1, %s, %s\n",MIP_ope2str(cur_mip->mip.orerands[0]),MIP_ope2str(cur_mip->mip.orerands[1]));
                // fprintf(MIP_file,"  bne $1, $0, %s",MIP_ope2str(cur_mip->mip.orerands[2]));
                break;
            case ble_MIP:
                // 伪指令
                fprintf(MIP_file,"ble %s, %s, %s",MIP_ope2str(cur_mip->mip.orerands[0]),MIP_ope2str(cur_mip->mip.orerands[1]),MIP_ope2str(cur_mip->mip.orerands[2]));
                // 真实指令
                // fprintf(MIP_file,"sgt $1, %s, %s\n",MIP_ope2str(cur_mip->mip.orerands[0]),MIP_ope2str(cur_mip->mip.orerands[1]));
                // fprintf(MIP_file,"  bne $1, $0, %s",MIP_ope2str(cur_mip->mip.orerands[2]));
                break;
            default:
                break;
        }
        fprintf(MIP_file,"\n");
        // printf("debug: one mip end\n");
        cur_mip = cur_mip->nxt;
        fclose(MIP_file);
    }
    
}

void generate_code(InterCodes ir_head,char* mip_file)
{
    printf("IR转MIP初始化开始\n");
    init_MIP_process();
    printf("IR转MIP初始化结束\n");
    printf("IR转MIP过程开始\n");
    IRs2MIPs(ir_head);
    printf("IR转MIP过程结束\n");
    output_env(mip_file);
    printf("输出MIP过程开始\n");
    output_MIPs(mip_head,mip_file);
    printf("输出MIP过程结束\n");
}