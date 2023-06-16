#include "all.h"

// 因为只有32个寄存器，这里可以打表初始化他们
Mip_Operand reg_operands[32];

// 生成空MIP操作数
Mip_Operand empty_MIP_operand()
{
    Mip_Operand res_ope = (Mip_Operand) malloc(sizeof(struct Mip_Operand_));
    memset(res_ope,0,sizeof(res_ope));
    return res_ope;
}

// 根据常数的值生成MIP操作数
Mip_Operand const_MIP_operand(int val,int if_label)
{
    Mip_Operand res_ope = empty_MIP_operand();
    res_ope->mip_ope_kind = CONSTANT_MIP;
    res_ope->u.const_.value = val;
    res_ope->u.const_.if_label = if_label;
    return res_ope;
}

// 通过寄存器别名生成操作数
Mip_Operand reg_MIP_operand_alias(int reg_kind,int no)
{
    Mip_Operand res_ope = empty_MIP_operand();
    res_ope->mip_ope_kind = REG;
    res_ope->u.reg.alias.kind = reg_kind;
    res_ope->u.reg.alias.no = no;
    res_ope->u.reg.ID = alias2ID(reg_kind,no);
    return res_ope;
}

Mip_Operand func_MIP_operand(char* func_name)
{
    Mip_Operand res_ope = empty_MIP_operand();
    res_ope->mip_ope_kind = FUNC_MIP;
    res_ope->u.func_name = func_name;
    return res_ope;
}

// 通过寄存器ID生成操作数，在初始化阶段用
Mip_Operand reg_MIP_operand_ID(int reg_ID)
{
    Mip_Operand res_ope = empty_MIP_operand();
    res_ope->mip_ope_kind = REG;
    res_ope->u.reg.ID = reg_ID;
    switch(reg_ID)
    {
        case 0:
            res_ope->u.reg.alias.kind = zero_REG;
            break;
        case 1:
            res_ope->u.reg.alias.kind = at_REG;
            break;
        case 2:
        case 3:
            res_ope->u.reg.alias.kind = v_REG;
            res_ope->u.reg.alias.no = reg_ID - 2;
            break;
        case 4:
        case 5:
        case 6:
        case 7:
            res_ope->u.reg.alias.kind = a_REG;
            res_ope->u.reg.alias.no = reg_ID - 4;
            break;
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
            res_ope->u.reg.alias.kind = t_REG;
            res_ope->u.reg.alias.no = reg_ID - 8;
            break;
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
        case 21:
        case 22:
        case 23:
            res_ope->u.reg.alias.kind = s_REG;
            res_ope->u.reg.alias.no = reg_ID - 16;
            break;
        case 24:
        case 25:
            res_ope->u.reg.alias.kind = t_REG;
            res_ope->u.reg.alias.no = reg_ID - 16;
            break;
        case 26:
        case 27:
            res_ope->u.reg.alias.kind = k_REG;
            res_ope->u.reg.alias.no = reg_ID - 26;
            break;
        case 28:
            res_ope->u.reg.alias.kind = gp_REG;
            break;
        case 29:
            res_ope->u.reg.alias.kind = sp_REG;
            break;
        case 30:
            res_ope->u.reg.alias.kind = fp_REG;
            // tips：这里是使用了fp的方法
            break;
        case 31:
            res_ope->u.reg.alias.kind = ra_REG;
            break;
    }

    return res_ope;
}

// 初始化所有寄存器操作数
void init_all_reg_operand()
{
    int i;
    for(i = 0; i < 32;i++)
    {
        reg_operands[i] = reg_MIP_operand_ID(i);
    }
}

// 根据寄存器ID获取寄存器操作数，打表之后用
Mip_Operand get_reg_ope_by_ID(int reg_ID)
{
    return reg_operands[reg_ID];
}

// 数字还要看是否表示标签
char* MIP_ope2str(Mip_Operand mip_ope)
{
    // printf("debug: MIP_ope2str 入口\n");
    // printf("debug: %d\n",mip_ope==NULL);
    int ope_kind = mip_ope->mip_ope_kind;
    // printf("debug: mip_ope_kind=%d\n",ope_kind);
    char* res_str;
    switch(ope_kind)
    {
        case CONSTANT_MIP:
            {
                
                // printf("debug: MIP_ope2str CONSTANT\n");
                int if_label = mip_ope->u.const_.if_label;
                int const_val = mip_ope->u.const_.value;
                int num_len = get_num_len(const_val)+(if_label?5:0);
                res_str = (char*) malloc(num_len+1);
                sprintf(res_str,"%s%d",if_label?"label":"",const_val);
            }
            break;
        case FUNC_MIP:
            {
                // printf("debug: MIP_ope2str FUNC\n");
                res_str = mip_ope->u.func_name;
            }
            break;
        case REG:
            {
                // printf("debug: MIP_ope2str REG\n");
                int reg_ID = mip_ope->u.reg.ID;
                int reg_kind = mip_ope->u.reg.alias.kind;
                int reg_no = mip_ope->u.reg.alias.no;
                res_str = (char*) malloc(reg_ID==0?5:2 +1);
                switch(reg_kind)
                {
                    case zero_REG:
                        sprintf(res_str,"$zero");
                        break;
                    case at_REG:
                        sprintf(res_str,"$at");
                        break;
                    case v_REG:
                        sprintf(res_str,"$v%d",reg_no);
                        break;
                    case a_REG:
                        sprintf(res_str,"$a%d",reg_no);
                        break;
                    case t_REG:
                        sprintf(res_str,"$t%d",reg_no);
                        break;
                    case s_REG:
                        sprintf(res_str,"$s%d",reg_no);
                        break;
                    case k_REG:
                        sprintf(res_str,"$k%d",reg_no);
                        break;
                    case gp_REG:
                        sprintf(res_str,"$gp");
                        break;
                    case sp_REG:
                        sprintf(res_str,"$sp");
                        break;
                    case fp_REG:
                        sprintf(res_str,"$fp");
                        break;
                    case ra_REG:
                        sprintf(res_str,"$ra");
                        break;
                    default:
                        sprintf(res_str,"err");
                        break;
                }
                
            }
            break;
        default:
            // printf("debug: MIP_ope2str default\n");
            res_str = "error";
            
            break;
    }    
    return res_str;
    // printf("debug: MIP_ope2str 出口\n");
}