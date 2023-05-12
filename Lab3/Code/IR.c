#include "all.h"
#include <stdarg.h>
#include <math.h>


int cur_label;
int cur_tem_no;
int cur_line;
InterCodes head;// 存储IR链表，head不存放
InterCodes tail;// IR链表的最后一个元素
Operand_List ope_list_head;// 存储变量操作数链表，head不存放，用来记录一个作用域已经声明的地址
int field_layer;// 结构体取域的层数
int array_layer;// 数组取域的层数

// code只用一次，用完就可以直接释放

void init_IR()
{
    cur_label = 1;

    cur_tem_no = 1;
    cur_line = 0;
    head = (InterCodes) malloc(sizeof(struct InterCodes_)); 
    ope_list_head = (Operand_List) malloc(sizeof(struct Operand_List_));
    ope_list_head->nxt = NULL;
    tail = NULL;
    field_layer = 0;
    array_layer = 0;
}

void insert_InterCodes(InterCodes new_ir)
{
    // 链表为空
    if(tail==NULL)
    {
        head->nxt = new_ir;
        new_ir->prev = head;
        
    }
    // 链表非空
    else
    {
        tail->nxt = new_ir;
        new_ir->prev = tail;
    }
    // 更新尾部元素
    tail = new_ir;
}



void insert_Operand_List(Operand_List new_ope_list)
{
    new_ope_list->nxt = ope_list_head->nxt;
    ope_list_head->nxt = new_ope_list;
}

void add_Operand(Operand ope)
{
    // printf("debug: add_Operand %d\n",ope->u.no);
    Operand_List new_ope_list = (Operand_List) malloc(sizeof(struct Operand_List_));
    new_ope_list->ope = ope;
    insert_Operand_List(new_ope_list);
}

Operand search_Operand_List(int kind,int no)
{
    // printf("debug: search_Operand_List开始\n");
    Operand_List cur_ope_list = ope_list_head->nxt;
    Operand res_ope = NULL;
    while(cur_ope_list!=NULL)
    {
        // printf("debug: search_Operand_List 进入循环\n");
        // printf("debug: search_Operand_List cur_ope is null %p\n",cur_ope_list->ope);
        
        int cur_kind = cur_ope_list->ope->kind;
        // printf("debug: search_Operand_List 得到kind %d\n",cur_kind);
        int cur_no = cur_ope_list->ope->u.no;
        // printf("debug: search_Operand_List 得到no %d\n",cur_no);
        if(cur_kind==kind && cur_no==no)
        {
            // printf("debug: search_Operand_List 进入IF\n");
            res_ope = cur_ope_list->ope;
            
            break;
        }
        cur_ope_list = cur_ope_list->nxt;
        
    }
    return res_ope;
    // printf("debug: search_Operand_List结束\n");
}

// 不是深度释放内存，只是释放链表
void clear_Operand_List()
{
    Operand_List cur_ope_list = ope_list_head->nxt;
    Operand_List tem_ope_list;
    while(cur_ope_list!=NULL)
    {
        tem_ope_list = cur_ope_list;
        cur_ope_list = cur_ope_list->nxt;
        free(tem_ope_list);
    }
    ope_list_head->nxt = NULL;
}

void clear_InterCodes()
{
    InterCodes cur_ir = head->nxt;
    InterCodes next_ir = cur_ir->nxt;
    while(next_ir!=NULL)
    {
        free(cur_ir);
        cur_ir = next_ir;
        next_ir = next_ir->nxt;
    }
}



// 根据num返回对应的位数
int get_num_len(int num)
{
    int num_len = 0;
    do
    {
        num_len += 1;
        num /= 10;
    } while (num);
    return num_len;
}

// 根据Operand返回对应place的字符串，一般只会对VARIABLE和ADDRESS进行操作
// 返回一个指向新分配空间的地址
char* ope2str(Operand op)
{
    char* res_str;
    int no = op->u.no;
    int val = op->u.value;
    int num_len = op->kind==CONSTANT?get_num_len(val):get_num_len(no); 
    res_str = (char*) malloc(num_len+2+(op->if_take_addr||op->if_deref));
    char* cur_pos = res_str;
    if(op->if_take_addr)
    {
        sprintf(cur_pos,"&");
        cur_pos += 1;
    }
    else if(op->if_deref)
    {
        sprintf(cur_pos,"*");
        cur_pos += 1;
    }

    if(op->kind==VARIABLE)
    {
        sprintf(cur_pos,"v%d",no);   
    }
    else if(op->kind==TEMP)
    {
        sprintf(cur_pos,"t%d",no); 
    }
    else if(op->kind==CONSTANT)
    {
        sprintf(cur_pos,"#%d",val);
    }
    else
    {
        printf("note: ope2str error\n");
        // printf("debug: ope_kind:%p\n",op);
    }
    return res_str;
}


// 获取域的相对位置
int get_field_pos(Type type,char* field_name)
{
    if(type->kind!=STRUCTURE)
    {
        printf("note: get_field_pos error\n");
        return 0;
    }
    int cur_pos = 0;
    FieldList cur_field = type->u.structure;
    while(cur_field!=NULL)
    {
        if(strcmp(cur_field->name,field_name)==0)
        {
            return cur_pos;
        }
        cur_pos += cur_field->type->size;
        cur_field = cur_field->tail;
    }
    return 0;
}

void copy_operand(Operand dst,Operand src)
{
    memcpy(dst,src,sizeof(struct Operand_));
}

// 获取数组元素的相对位置
int get_item_pos(Type type,int idx)
{
    if(type->kind!=ARRAY)
    {
        printf("note: get_item_pos error\n");
        return 0;
    }
    return type->u.array.elem->size*idx;
}

// 用于输出全部IR
void output_IRs(InterCodes head,char* file_name)
{
    FILE* IR_file;
    IR_file = fopen(file_name,"w");
    printf("debug: %s\n",file_name);
    InterCodes cur_ir = head->nxt;
    while(cur_ir!=NULL)
    {
        // printf("debug: output_IRs IR_type:%d\n",cur_ir->code.ir_kind);
        switch (cur_ir->code.ir_kind)
        {
            case ASSIGN_IR:

                fprintf(IR_file,"%s := ",ope2str(cur_ir->code.u.assign.left));

                fprintf(IR_file,"%s",ope2str(cur_ir->code.u.assign.right));
                break; 
            case ADD_IR:
                fprintf(IR_file,"%s := %s + %s",ope2str(cur_ir->code.u.binary_op.result),ope2str(cur_ir->code.u.binary_op.op1),ope2str(cur_ir->code.u.binary_op.op2));
                break;
            case SUB_IR:
                fprintf(IR_file,"%s := %s - %s",ope2str(cur_ir->code.u.binary_op.result),ope2str(cur_ir->code.u.binary_op.op1),ope2str(cur_ir->code.u.binary_op.op2));
                break;
            case MUL_IR:
                fprintf(IR_file,"%s := %s * %s",ope2str(cur_ir->code.u.binary_op.result),ope2str(cur_ir->code.u.binary_op.op1),ope2str(cur_ir->code.u.binary_op.op2));
                break;
            case DIV_IR:
                fprintf(IR_file,"%s := %s / %s",ope2str(cur_ir->code.u.binary_op.result),ope2str(cur_ir->code.u.binary_op.op1),ope2str(cur_ir->code.u.binary_op.op2));
                break;
            case LABEL_IR:
                fprintf(IR_file,"LABEL label%d :",cur_ir->code.u.if_op.label_no);
                break;
            case GOTO_IR:
                fprintf(IR_file,"GOTO label%d",cur_ir->code.u.if_op.label_no);
                break;
            case IF_IR:
                fprintf(IR_file,"IF %s %s %s GOTO label%d",ope2str(cur_ir->code.u.if_op.left),cur_ir->code.u.if_op.relop,ope2str(cur_ir->code.u.if_op.right),cur_ir->code.u.if_op.label_no);
                break;
            case RETURN_IR:
                fprintf(IR_file,"RETURN %s",ope2str(cur_ir->code.u.return_op.return_operand));
                break;
            case DEC_IR:
                fprintf(IR_file,"DEC %s %d",ope2str(cur_ir->code.u.dec.place),cur_ir->code.u.dec.size);
                break;
            case ARG_IR:
                fprintf(IR_file,"ARG %s",ope2str(cur_ir->code.u.read_write_arg_param_return.place));
                break;
            case CALL_IR:
                fprintf(IR_file,"%s := CALL %s",ope2str(cur_ir->code.u.func_call.left),cur_ir->code.u.func_call.func);
                break;
            case FUNC_IR:
                fprintf(IR_file,"FUNCTION %s :",cur_ir->code.u.func_call.func);
                break;
            case PARAM_IR:
                fprintf(IR_file,"PARAM %s",ope2str(cur_ir->code.u.read_write_arg_param_return.place));
                break;
            case READ_IR:
                fprintf(IR_file,"READ %s",ope2str(cur_ir->code.u.read_write_arg_param_return.place));
                break;
            case WRITE_IR:
                fprintf(IR_file,"WRITE %s",ope2str(cur_ir->code.u.read_write_arg_param_return.place));
                break;
            default:
                break;
        }
        fprintf(IR_file,"\n");
        cur_ir = cur_ir->nxt;
    }
    fclose(IR_file);
}

// 创建ASSIGN_IR，并插入IR链表中
void generate_ASSIGN_IR(Operand left,Operand right)
{
    if(left==NULL || right==NULL)
    {
        if(left==NULL)
        {
            printf("note: generate_ASSIGN_IR left NULL\n");
        }
        else
        {
            printf("note: generate_ASSIGN_IR right NULL\n");
        }
        // printf("note: generate_ASSIGN_IR NULL\n");
        return;
    }
    InterCodes new_ir = (InterCodes) malloc(sizeof(struct InterCodes_));
    new_ir->code.ir_kind = ASSIGN_IR;
    new_ir->code.u.assign.left = left;
    new_ir->code.u.assign.right = right;
    insert_InterCodes(new_ir);
}

// 创建ADD_IR，并插入IR链表中
// if_load: 是否是存到result代表的地址中，此时result代表一个地址
void generate_binary_IR(int IR_kind,Operand result,Operand op1,Operand op2)
{
    // printf("debug: gen_binary_IR\n");
    if(result==NULL || op1==NULL || op2==NULL)
    {
        printf("note: generate_binary_IR NULL\n");
        return;
    
    }

    // 不允许*t_1 = #1 + #2的指令
    if(result->if_deref)
    {
        InterCodes new_ir = (InterCodes) malloc(sizeof(struct InterCodes_));

        new_ir->code.ir_kind = IR_kind;
        new_ir->code.u.binary_op.result = result;
        new_ir->code.u.binary_op.op1 = op1;
        new_ir->code.u.binary_op.op2 = op2;
        
        insert_InterCodes(new_ir);
    }
    else
    {
        InterCodes binary_ir = (InterCodes) malloc(sizeof(struct InterCodes_));

        binary_ir->code.ir_kind = IR_kind;
        binary_ir->code.u.binary_op.result = result;
        binary_ir->code.u.binary_op.op1 = op1;
        binary_ir->code.u.binary_op.op2 = op2;
        insert_InterCodes(binary_ir);
    }
    
}

// 创建LABEL_IR和GOTO_IR，并插入IR链表中: 
void generate_uncondition_IR(int ir_kind,int label)
{
    InterCodes new_ir = (InterCodes) malloc(sizeof(struct InterCodes_));
    new_ir->code.ir_kind = ir_kind;
    new_ir->code.u.if_op.label_no = label;

    insert_InterCodes(new_ir);
}

// 创建IF_IR，并插入IR链表中
void generate_condition_IR(Operand left,Operand right,int label,char* relop)
{
    // printf("debug: generate_condition_IR\n");
    InterCodes new_ir = (InterCodes) malloc(sizeof(struct InterCodes_));
    new_ir->code.ir_kind = IF_IR;
    new_ir->code.u.if_op.left = left;
    new_ir->code.u.if_op.right = right;
    new_ir->code.u.if_op.label_no = label;
    new_ir->code.u.if_op.relop = relop;

    insert_InterCodes(new_ir);
}

// 创建RETURN_IR，并插入IR链表中
void generate_return_IR(Operand return_operand)
{
    InterCodes new_ir = (InterCodes) malloc(sizeof(struct InterCodes_));
    new_ir->code.ir_kind = RETURN_IR;
    new_ir->code.u.return_op.return_operand = return_operand;
    insert_InterCodes(new_ir);
}

// 创建一个新的tem，返回一个指向新开辟空间的指针
Operand create_tem()
{
    Operand res = (Operand) malloc(sizeof(struct Operand_));
    res->kind = TEMP;
    res->u.no = cur_tem_no++;
    res->if_addr = 0;
    res->if_deref = 0;
    res->if_take_addr = 0;
    return res;
}

// 创建新label，只需返回对应的label序号
int create_label()
{
    return cur_label++;
}

// 创建常数操作数，返回一个指向新开辟空间的指针
Operand create_const(int val)
{
    // 创建常数操作数
    Operand CONST_place = (Operand) malloc(sizeof(struct Operand_));
    CONST_place->kind = CONSTANT;
    CONST_place->u.value = val;
    CONST_place->if_addr = 0;
    CONST_place->if_deref = 0;
    CONST_place->if_take_addr = 0;
    return CONST_place;
}



// 用以合成代码
// 返回一个指向新分配空间的地址
char* syn_codes(int num,...) {
    // step 1 
    va_list valist; 
    // step 2 
     
    va_start(valist, num); // 为num个参数初始化valist
    struct Node* cur_kid;
    char* res_code;
    char* cur_code;
    int res_len = 0;

    int i;
    // 计算代码总长度
    for(i = 0; i < num;i++)
    {
        cur_code = va_arg(valist, char*);
        if(cur_code!=NULL && strcmp(cur_code,"")!=0)
        {
            // 为什么要加1，因为我想每个命令换行
            res_len += strlen(cur_code)+1;
        }
        
    }
    // 但是最后一个code后不要加换行，所以最后一个循环多出来的空间放置'\0'

    res_code = (char*) malloc(res_len);
    strcpy(res_code,"");
    for (i = 0; i < num; i++) 
    {
        cur_code = va_arg(valist, char*);
        if(cur_code!=NULL && strcmp(cur_code,"")!=0)
        {
            strcat(res_code,cur_code);
            // 最后一个code不加"\n"
            if(i!=num-2)
            {
                strcat(res_code,"\n");
            }
            
        }
        
    } 
    va_end(valist); // 清除为varlist保存的内存
    return res_code;
} 

char* empty_code()
{
    char* tem;
    tem = (char*) malloc(1);
    strcpy(tem,"");
    return tem;
}


void translate_Args(struct Node* nd,Arg_List arg_list,FieldList arg_type,int arg_idx)
{
    // printf("debug: translate_Args begin\n");
    FieldList cur_arg_type = arg_type;
    int i = 0;
    while(i < arg_idx)
    {
        cur_arg_type = cur_arg_type->tail;
        i++;
    }
    int if_addr = cur_arg_type->type->kind==ARRAY || cur_arg_type->type->kind==STRUCTURE;

    int kid_num = nd->kid_num;
    // Args -> Exp
    if(kid_num==1)
    {
        // printf("debug: Args断点1-1\n");
        Operand t_1 = create_tem();
        // TODO: 根据参数类型判断是否保留地址
        translate_Exp(nd->kids[0],t_1,if_addr);
        // printf("debug: Args断点1-2\n");
        Arg_List new_arg = (Arg_List) malloc(sizeof(struct Arg_List_));
        new_arg->arg = t_1;
        new_arg->nxt = arg_list->nxt;
        arg_list->nxt = new_arg;
        // printf("debug: Args断点1-3\n");
    }
    // Args -> Exp COMMA Args 
    else if(kid_num==3)
    {
        // printf("debug: Args断点2-1\n");
        Operand t_1 = create_tem();
        translate_Exp(nd->kids[0],t_1,if_addr);
        Arg_List new_arg = (Arg_List) malloc(sizeof(struct Arg_List_));
        new_arg->arg = t_1;
        new_arg->nxt = arg_list->nxt;
        arg_list->nxt = new_arg;
        // 这里需要注意，新arg是添加到头部的，因为要倒序存储
        translate_Args(nd->kids[2],arg_list,arg_type,arg_idx+1);
        // printf("debug: Args断点2-2\n");
    }
    else
    {
        printf("note: translate_Args error\n");
    }
    

    // printf("debug: translate_Args end\n");
}


// 尽可能地把地址place往下传，知道不能传为止
// remain_addr——在某些情况下，结构体取域和数组取元素只是获取到某个地址(在层数不足的情况下)，
// 这个remian_addr只需根据被赋值的对象的类型判断，一般只在传参过程中使用
void translate_Exp(struct Node* nd, Operand place,int remain_addr)
{
    // printf("debug: translate_Exp begin\n");
    
    int kid_num = nd->kid_num;

    char* res_code;

    if(kid_num==1)
    {
        struct Node* kid_0 = nd->kids[0];
        // Exp -> INT
        if(kid_0->tag==INT)
        {
            int val = kid_0->value.type_int;

            place->kind = CONSTANT;
            place->u.value = val;

        }
        // Exp -> ID
        else if(kid_0->tag==ID)
        {            
            Var var = search_var(kid_0->name,1);
            int var_no = var->var_no;
            Operand var_place = search_Operand_List(VARIABLE,var_no);
            copy_operand(place,var_place);
            // 是否取地址只取决于这个变量当前是否代表一个地址与其是否为非基础类型
            place->if_take_addr = (!var_place->if_addr)&&var->type->kind!=BASIC;
        }
    }
    else if(kid_num==2)
    {
        struct Node* kid_0 = nd->kids[0];
        struct Node* kid_1 = nd->kids[1];
        // Exp -> MINUS Exp
        if(kid_0->tag==MINUS)
        {
            Operand t_1 = create_tem();
            translate_Exp(kid_1,t_1,remain_addr);
            generate_binary_IR(SUB_IR,place,create_const(0),t_1);
        }
        // Exp -> NOT Exp
        else if(kid_0->tag==NOT)
        {
            int label_1 = create_label();
            int label_2 = create_label();
            generate_ASSIGN_IR(place,create_const(0));
            translate_Cond(nd,label_1,label_2);
            generate_uncondition_IR(LABEL_IR,label_1);
            generate_ASSIGN_IR(place,create_const(1));
            generate_uncondition_IR(LABEL_IR,label_2);
        }
    }
    else if(kid_num==3)
    {
        struct Node* kid_0 = nd->kids[0];
        struct Node* kid_1 = nd->kids[1];
        struct Node* kid_2 = nd->kids[2];

        // Exp -> Exp ASSIGNOP Exp
        // 先得到代表左边的变量/地址，然后再放到右边去计算
        if(kid_1->tag==ASSIGNOP)
        {
            // Exp_1 -> ID
            if(kid_0->generation==Exp3)
            {
                Var var = search_var(kid_0->kids[0]->name,1);
                Operand var_place = search_Operand_List(VARIABLE,var->var_no);
                Operand t_1 = create_tem();
                translate_Exp(kid_2,t_1,0);// 因为不存在结构体赋值，所以remain_addr是0
                generate_ASSIGN_IR(var_place,t_1);
            }
            // Exp_1 -> Exp DOT ID
            else if(kid_0->generation==Exp2)
            {
                Operand ope = create_tem();
                translate_Exp(kid_0,ope,1);// 得到的ope是一个地址
                ope->if_addr = 1;
                Operand t_1 = create_tem();
                translate_Exp(kid_2,t_1,0);// 因为不存在结构体赋值，所以remain_addr是0
                // 为什么要复制？因为后面ope_copy解引用,而ope不需要，复用会相互干扰
                Operand ope_copy = (Operand) malloc(sizeof(struct Operand_));
                copy_operand(ope_copy,ope);
                ope_copy->if_deref = 1;
                generate_ASSIGN_IR(ope_copy,t_1);
            }
            // Exp_1 -> Exp LB Exp RB
            else if(kid_0->kid_num==4&&kid_0->kids[1]->tag==LB)
            {
                Operand ope = create_tem();
                translate_Exp(kid_0,ope,1);// 得到的ope是一个地址
                ope->if_addr = 1;
                Operand t_1 = create_tem();
                translate_Exp(kid_2,t_1,0);// 因为不存在结构体赋值，所以remain_addr是0
                // 为什么要复制？因为后面ope_copy解引用,而ope不需要，复用会相互干扰
                Operand ope_copy = (Operand) malloc(sizeof(struct Operand_));
                copy_operand(ope_copy,ope);
                ope_copy->if_deref = 1;
                generate_ASSIGN_IR(ope_copy,t_1);
            }
            else
            {

            }
        }
        // Exp -> Exp PLUS Exp
        else if(kid_1->tag==PLUS)
        {
            Operand t_1 = create_tem();
            Operand t_2 = create_tem();
            translate_Exp(kid_0,t_1,remain_addr);
            translate_Exp(kid_2,t_2,remain_addr);
            generate_binary_IR(ADD_IR,place,t_1,t_2);
        }
        // Exp -> Exp MINUS Exp
        else if(kid_1->tag==MINUS)
        {
            Operand t_1 = create_tem();
            Operand t_2 = create_tem();
            translate_Exp(kid_0,t_1,remain_addr);
            translate_Exp(kid_2,t_2,remain_addr);
            generate_binary_IR(SUB_IR,place,t_1,t_2);
            
        }
        // Exp -> Exp STAR Exp
        else if(kid_1->tag==STAR)
        {
            Operand t_1 = create_tem();
            Operand t_2 = create_tem();
            translate_Exp(kid_0,t_1,remain_addr);
            translate_Exp(kid_2,t_2,remain_addr);
            generate_binary_IR(MUL_IR,place,t_1,t_2);
        }
        // Exp -> Exp DIV Exp
        else if(kid_1->tag==DIV)
        {
            Operand t_1 = create_tem();
            Operand t_2 = create_tem();
            translate_Exp(kid_0,t_1,remain_addr);
            translate_Exp(kid_2,t_2,remain_addr);
            generate_binary_IR(DIV_IR,place,t_1,t_2); 
        }
        // Exp -> Exp RELOP Exp | Exp AND Exp | Exp OR Exp
        else if(kid_1->tag==RELOP || kid_1->tag == AND || kid_1->tag==OR)
        {
            // printf("debug: translate_Exp 断点开始\n");
            int label_1 = create_label();
            int label_2 = create_label();
            generate_ASSIGN_IR(place,create_const(0));
            translate_Cond(nd,label_1,label_2);
            // printf("debug: translate_Exp 断点1\n");
            generate_uncondition_IR(LABEL_IR,label_1);
            
            generate_ASSIGN_IR(place,create_const(1));
            generate_uncondition_IR(LABEL_IR,label_2);
            // printf("debug: translate_Exp 断点结束\n");
        }
        // Exp -> ID LP RP
        else if(kid_0->tag==ID)
        {
            // printf("debug: translate_Exp 断点2\n");
            Common func = search_common(kid_0->name,FUNC);
            // printf("debug: translate_Exp func_name: func_is_null: %d\n",func==NULL);
            char* func_name = func->name;
            // printf("debug: translate_Exp 断点2-1\n");
            // C--定义的特殊函数read()
            if(strcmp(func_name,"read")==0)
            {
                InterCodes new_ir = (InterCodes) malloc(sizeof(struct InterCodes_));
                new_ir->code.ir_kind = READ_IR;
                new_ir->code.u.read_write_arg_param_return.place = place;
                insert_InterCodes(new_ir);
            }
            else
            {
                Operand t_1 = create_tem();
                InterCodes new_ir = (InterCodes) malloc(sizeof(struct InterCodes_));
                new_ir->code.ir_kind = CALL_IR;
                new_ir->code.u.func_call.func = func_name;
                new_ir->code.u.func_call.left = place;
                insert_InterCodes(new_ir);    
            }
            
        }
        // Exp -> LP Exp RP
        else if(kid_0->tag==LP)
        {
            translate_Exp(kid_1,place,remain_addr);
        }
        // Exp -> Exp DOT ID
        else if(kid_1->tag==DOT)
        {
            // TODO: 结构体取域
            // 默认head_addr被赋值了结构体的起始地址
            field_layer++;

            // 计算出头地址
            Operand head_addr = create_tem();
            translate_Exp(kid_0,head_addr,1);

            // 计算出加上偏移的地址
            int rel_position = get_field_pos(kid_0->type,kid_2->name);
            Operand actual_addr = create_tem();
            generate_binary_IR(ADD_IR,actual_addr,head_addr,create_const(rel_position));


            // 判断是否为最外层，最外层并且不需要保留为地址就需要deref，存到空间中
            int outer = (field_layer==1&&array_layer==0) || (field_layer==0&&array_layer==1);
            copy_operand(place,actual_addr);
            place->if_deref = outer&&!remain_addr;

            field_layer--;
        }
        else
        {
            printf("note: translate_Exp error 1\n");
        } 
    }
    else if(kid_num==4)
    {
        struct Node* kid_0 = nd->kids[0];
        // Exp -> ID LP Args RP
        if(kid_0->tag==ID)
        {
            Common func = search_common(nd->kids[0]->name,FUNC);
            char* func_name = func->name;
            // 用来存储参数的链表，第一个元素没有用，需要以倒序方式存储
            Arg_List arg_list = (Arg_List) malloc(sizeof(struct Arg_List_));
            translate_Args(nd->kids[2],arg_list,func->type->u.structure,1);
            // C--定义的特殊函数write()
            if(strcmp(func_name,"write")==0)
            {   
                InterCodes new_ir = (InterCodes) malloc(sizeof(struct InterCodes_));
                new_ir->code.ir_kind = WRITE_IR;
                new_ir->code.u.read_write_arg_param_return.place = arg_list->nxt->arg;
                insert_InterCodes(new_ir);
            }
            else
            {
                InterCodes arg_ir;
                Arg_List cur_Arg = arg_list->nxt;
                while(cur_Arg!=NULL)
                {
                    arg_ir = (InterCodes) malloc(sizeof(struct InterCodes_));
                    arg_ir->code.ir_kind = ARG_IR;
                    arg_ir->code.u.read_write_arg_param_return.place = cur_Arg->arg;

                    insert_InterCodes(arg_ir);
                    cur_Arg = cur_Arg->nxt;
                }

                // 不允许*t1 = CALL func
                if(place->if_take_addr)
                {
                    Operand t_1 = create_tem();
                    InterCodes cur_ir = (InterCodes) malloc(sizeof(struct InterCodes_));
                    cur_ir->code.ir_kind = CALL_IR;
                    cur_ir->code.u.func_call.func = func_name;
                    cur_ir->code.u.func_call.left = t_1;

                    insert_InterCodes(cur_ir);

                    generate_ASSIGN_IR(place,t_1);
                }
                else
                {
                    Operand t_1 = create_tem();
                    InterCodes cur_ir = (InterCodes) malloc(sizeof(struct InterCodes_));
                    cur_ir->code.ir_kind = CALL_IR;
                    cur_ir->code.u.func_call.func = func_name;
                    cur_ir->code.u.func_call.left = place;
                    insert_InterCodes(cur_ir);
                }
                
            }

        }
        // Exp -> Exp LB Exp RB
        else if(nd->kids[1]->tag==LB)
        {
            array_layer++;

            // 计算首地址
            Operand head_addr = create_tem();
            translate_Exp(kid_0,head_addr,1);
            // 计算下标
            Operand idx = create_tem();
            translate_Exp(nd->kids[2],idx,0);
            
            // 计算偏移量
            Operand bia = create_tem();
            generate_binary_IR(MUL_IR,bia,idx,create_const(kid_0->type->u.array.elem->size));
            // 计算准确地址
            Operand actual_addr = create_tem();
            generate_binary_IR(ADD_IR,actual_addr,head_addr,bia);
            // 同结构体取域
            int outer = (field_layer==1&&array_layer==0) || (field_layer==0&&array_layer==1);
            copy_operand(place,actual_addr);
            place->if_deref = outer&&!remain_addr;
            array_layer--;
        }
        else
        {
            printf("note: translate_Exp error 2\n");
        }
        
        
    }

    // printf("debug: translate_Exp end\n");
}

void translate_Cond(struct Node* nd,int label_true,int label_false)
{

    int kid_num = nd->kid_num;
    
    // Exp -> NOT Exp
    if(kid_num==2 && nd->kids[0]->tag==NOT)
    {
        translate_Cond(nd->kids[1],label_false,label_true);
    }
    // Exp -> Exp RELOP Exp
    else if(kid_num==3 && nd->kids[1]->tag==RELOP)
    {

        Operand t_1 = create_tem();
        Operand t_2 = create_tem();
        translate_Exp(nd->kids[0],t_1,0);
        translate_Exp(nd->kids[2],t_2,0);
        generate_condition_IR(t_1,t_2,label_true,nd->kids[1]->name);
        generate_uncondition_IR(GOTO_IR,label_false);

    }
    // Exp -> Exp AND Exp
    else if(kid_num==3 && nd->kids[1]->tag==AND)
    {

        int label_1 = create_label();
        translate_Cond(nd->kids[0],label_1,label_false);
        generate_uncondition_IR(LABEL_IR,label_1);
        translate_Cond(nd->kids[2],label_true,label_false);
    }
    // Exp -> Exp OR Exp
    else if(kid_num==3 && nd->kids[1]->tag==OR)
    {
        int label_1 = create_label();
        translate_Cond(nd->kids[0],label_true,label_1);
        generate_uncondition_IR(LABEL_IR,label_1);
        translate_Cond(nd->kids[2],label_true,label_false);
    }
    else
    {
        Operand t_1 = create_tem();
        translate_Exp(nd,t_1,0);
        generate_condition_IR(t_1,create_const(0),label_true,"!=");
        generate_uncondition_IR(GOTO_IR,label_false);
    }

    // printf("debug: translate_Cond end\n");
}

void translate_Stmt(struct Node* nd)
{
    // printf("debug: translate_Stmt begin\n");

    int kid_num = nd->kid_num;
    // Stmt -> Compst
    if(kid_num==1)
    {
        translate_CompSt(nd->kids[0]);
    }
    // Stmt -> Exp SEMI
    else if(kid_num==2)
    {
        Operand t_1 = create_tem();
        translate_Exp(nd->kids[0],t_1,0);
    }
    // Stmt ->  RETURN Exp SEMI
    else if (kid_num==3)
    {
        Operand t_1 = create_tem();
        // printf("debug: translate_Stmt断点：处理RETURN语句\n");
        
        // 根据假设，只会返回基本类型
        translate_Exp(nd->kids[1],t_1,0);
        generate_return_IR(t_1);
    }
    else if(kid_num==5)
    {
        // Stmt -> IF LP Exp RP Stmt
        if(nd->kids[0]->tag==IF)
        {
            int label_1 = create_label();
            int label_2 = create_label();
            translate_Cond(nd->kids[2],label_1,label_2);
            generate_uncondition_IR(LABEL_IR,label_1);
            translate_Stmt(nd->kids[4]);
            generate_uncondition_IR(LABEL_IR,label_2);
        }
        // Stmt -> WHILE LP Exp RP Stmt
        else if(nd->kids[0]->tag==WHILE)
        {
            int label_1 = create_label();
            int label_2 = create_label();
            int label_3 = create_label();
            generate_uncondition_IR(LABEL_IR,label_1);
            translate_Cond(nd->kids[2],label_2,label_3);
            generate_uncondition_IR(LABEL_IR,label_2);
            translate_Stmt(nd->kids[4]);
            generate_uncondition_IR(GOTO_IR,label_1);
            generate_uncondition_IR(LABEL_IR,label_3);
        }
        else
        {
            printf("note: translate_Cond error 1\n");
        }
    }
    // Stmt-> IF LP Exp RP Stmt ELSE Stmt
    else if(kid_num==7)
    {
        int label_1 = create_label();
        int label_2 = create_label();
        int label_3 = create_label();
        translate_Cond(nd->kids[2],label_1,label_2);
        generate_uncondition_IR(LABEL_IR,label_1);
        translate_Stmt(nd->kids[4]);
        generate_uncondition_IR(GOTO_IR,label_3);
        generate_uncondition_IR(LABEL_IR,label_2);
        translate_Stmt(nd->kids[6]);
        generate_uncondition_IR(LABEL_IR,label_3);

    }
    else
    {
        printf("note: translate_Cond error 2\n");
    }

    // printf("debug: translate_Stmt end\n");
}

void translate_VarDec(struct Node* nd,int param)
{
    // printf("debug: translate_VarDec begin\n");

    int kid_num = nd->kid_num;
    // printf("debug: translate_VarDec var_name: |%s|\n",nd->kids[0]->name);
    Var var = search_var(nd->kids[0]->name,1);
    // printf("debug: translate_VarDec find var: %d\n",var==NULL);
    int var_no = var->var_no;

    // 创建ID操作数
    Operand VAR_place = (Operand) malloc(sizeof(struct Operand_));
    VAR_place->kind = VARIABLE;
    VAR_place->if_addr = param&&var->type->kind!=BASIC?1:0;
    VAR_place->u.no = var_no;
    VAR_place->if_take_addr = 0;
    VAR_place->if_deref = 0;
    add_Operand(VAR_place);
    

    // 参数只需声明为参数，不分配内存
    if(param)
    {
        InterCodes param_ir = (InterCodes) malloc(sizeof(struct InterCodes_));
        param_ir->code.ir_kind = PARAM_IR;
        param_ir->code.u.read_write_arg_param_return.place = VAR_place;
        insert_InterCodes(param_ir);
    }
    else 
    {
        // VarDec -> ID
        if(kid_num==1)
        {
            // printf("debug: VarDec -> ID\n");
            if(var->type->kind==STRUCTURE)
            {
                InterCodes dec_ir = (InterCodes) malloc(sizeof(struct InterCodes_));
                dec_ir->code.ir_kind = DEC_IR;
                dec_ir->code.u.dec.place = VAR_place;
                dec_ir->code.u.dec.size = var->type->size;
                insert_InterCodes(dec_ir);
            }
            else
            {

            }
        }
        // VarDec -> VarDec LB INT RB
        else if(kid_num==4)
        {
            // printf("debug: VarDec -> VarDec LB INT RB\n");
            InterCodes dec_ir = (InterCodes) malloc(sizeof(struct InterCodes_));
            dec_ir->code.ir_kind = DEC_IR;
            dec_ir->code.u.dec.place = VAR_place;
            dec_ir->code.u.dec.size = var->type->size;
            // printf("debug: %d\n",VAR_place->if_take_addr);
            insert_InterCodes(dec_ir);
        }
        else
        {
            printf("note: translate_VarDec error\n");
        }
        
    }
    
    // printf("debug: translate_VarDec end\n");
}

void translate_ParamDec(struct Node* nd)
{
    // printf("debug: translate_ParamDec begin\n");

    // ParamDec -> Specifier VarDec 
    translate_VarDec(nd->kids[1],1);

    // printf("debug: translate_ParamDec end\n");
}

void translate_VarList(struct Node* nd)
{
    // printf("debug: translate_VarList begin\n");

    int kid_num = nd->kid_num;
    // VarList -> ParamDec COMMA VarList
    if(kid_num==3)
    {
        translate_ParamDec(nd->kids[0]);
        translate_VarList(nd->kids[2]);
    }
    // VarList -> ParamDec
    else if(kid_num==1)
    {
        translate_ParamDec(nd->kids[0]);
    }
    else
    {
        printf("note: translate_VarList error\n");
    }

    // printf("debug: translate_VarList end\n");
}

void translate_FunDec(struct Node* nd)
{
    // printf("debug: translate_FunDec begin\n");

    int kid_num = nd->kid_num;
    // FunDec -> ID LP VarList RP
    if(kid_num==4)
    {
        InterCodes func_IR = (InterCodes) malloc(sizeof(struct InterCodes_));
        func_IR->code.ir_kind = FUNC_IR;
        func_IR->code.u.func_call.func = nd->kids[0]->name;
        insert_InterCodes(func_IR);

        translate_VarList(nd->kids[2]);       
    }
    // FunDec -> ID LP RP
    else if(kid_num==3)
    {
        InterCodes func_IR = (InterCodes) malloc(sizeof(struct InterCodes_));
        func_IR->code.ir_kind = FUNC_IR;
        func_IR->code.u.func_call.func = nd->kids[0]->name;

        insert_InterCodes(func_IR);
    }

    // printf("debug: translate_FunDec end\n");
}

void translate_Dec(struct Node* nd)
{
    // printf("debug: translate_Dec begin\n");

    int kid_num = nd->kid_num;
    // Dec -> VarDec
    if(kid_num==1)
    {
        translate_VarDec(nd->kids[0],0);
    }
    // Dec -> VarDec ASSIGNOP Exp
    else if(kid_num==3)
    {
        Operand t_1 = create_tem();
        translate_Exp(nd->kids[2],t_1,0);
        translate_VarDec(nd->kids[0],0);
        Var var = search_var(nd->kids[0]->kids[0]->name,1);
        int var_no = var->var_no;
        // 创建ID操作数
        Operand VAR_place = (Operand) malloc(sizeof(struct Operand_));
        VAR_place->kind = VARIABLE;
        VAR_place->u.no = var_no;
        VAR_place->if_addr = 0;
        VAR_place->if_deref = 0;
        VAR_place->if_take_addr = 0;
        generate_ASSIGN_IR(VAR_place,t_1);
    }
    else
    {
        printf("note: translate_Dec error\n");
    }

    // printf("debug: translate_Dec end\n");
}

void translate_DecList(struct Node* nd)
{
    // printf("debug: translate_DecList begin\n");

    int kid_num = nd->kid_num;
    // DecList -> Dec
    if(kid_num==1)
    {
        translate_Dec(nd->kids[0]);
    }
    // DecList -> Dec COMMA DecList
    else if(kid_num==3)
    {
        translate_Dec(nd->kids[0]);
        translate_DecList(nd->kids[2]);
    }
    else
    {
        printf("note: translate_DecList error\n");
    }

    // printf("debug: translate_DecList end\n");
}

void translate_Def(struct Node* nd)
{
    // printf("debug: translate_Def begin\n");

    // Def -> Specifier DecList SEMI
    translate_DecList(nd->kids[1]);

    // printf("debug: translate_Def end\n");
}

void translate_DefList(struct Node* nd)
{
    // printf("debug: translate_DefList begin\n");

    int kid_num = nd->kid_num;
    // DefList -> 
    if(nd->if_empty)
    {
        // return empty_code();
    }
    // DefList -> Def DefList
    else if(nd->kid_num==2)
    {
        translate_Def(nd->kids[0]);
        translate_DefList(nd->kids[1]);
    }
    else
    {
        printf("note: translate_DefList error\n");
    }

    // printf("debug: translate_DefList end\n");
}

void translate_StmtList(struct Node* nd)
{
    // printf("debug: translate_StmtList begin\n");

    int kid_num = nd->kid_num;
    // StmtList -> 
    if(kid_num==0)
    {
        // return empty_code();
    }
    // StmtList -> Stmt StmtList
    else if(nd->kid_num==2)
    {
        translate_Stmt(nd->kids[0]);
        translate_StmtList(nd->kids[1]);
    }
    else
    {
        printf("note: translate_StmtList error\n");
    }

    // printf("debug: translate_StmtList end\n");
}

void translate_CompSt(struct Node* nd)
{
    // printf("debug: translate_CompSt begin\n");

    push_domain();

    // CompSt -> LC DefList StmtList RC
    translate_DefList(nd->kids[1]);
    translate_StmtList(nd->kids[2]);


    pop_domain();

    // printf("debug: translate_CompSt end\n");
}

void translate_ExtDef(struct Node* nd)
{
    // printf("debug: translate_ExtDef begin\n");

    // ExtDef -> Specifier ExtDecList SEMI
    // 这个产生式不产生中间代码
    if(nd->kids[1]->tag==ExtDecList)
    { 
        // return empty_code();
    }
    // ExtDef -> Specifier SEMI
    // 这个产生式不产生中间代码
    else if(nd->kids[1]->tag==SEMI)
    {
        // return empty_code();
    }
    // ExtDef -> Specifier FunDec CompSt
    else if(nd->kids[1]->tag==FunDec)
    {
        translate_FunDec(nd->kids[1]);
        translate_CompSt(nd->kids[2]);
    }
    else
    {
        printf("note: translate_ExtDef error\n");
    }
    clear_Operand_List();
    // printf("debug: translate_ExtDef end\n");
}

void translate_ExtDefList(struct Node* nd)
{
    // printf("debug: translate_ExtDefList begin\n");

    int kid_num = nd->kid_num;
    // ExtDefList -> 
    if(kid_num==0)
    {
        // return empty_code();
    }
    // ExtDefList -> ExtDef ExtDefList
    else if(nd->kid_num==2)
    {
        translate_ExtDef(nd->kids[0]);
        translate_ExtDefList(nd->kids[1]);
    }
    else
    {
        printf("note: translate_ExtDefList error\n");
    }

    // printf("debug: translate_ExtDefList end\n");
}

void translate_Program(struct Node* nd)
{
    // printf("debug: translate_DProgram begin\n");
    translate_ExtDefList(nd->kids[0]);
    // printf("debug: translate_Program end\n");
}

// IR代码生成的入口
void generate_IR(struct Node* nd,char* ir_file)
{

    if(nd==NULL)
    {
        fprintf(stderr, "Fail building syntax tree\n");
        // printf("poniter is null\n");
        return;
    }
    init_IR();
    printf("debug: 开始生成IR\n");
    translate_Program(nd);
    printf("debug: 完成生成IR\n");
    output_IRs(head,ir_file);
}
