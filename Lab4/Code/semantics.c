#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "syntax.tab.h"
#include "all.h"

Var var_table[SIGNAL_TABLE_SIZE+1];// 变量符号表，数组中元素均为头指针，本身没有含义
Domain_Item domains[DOMAIN_LAYER];// 作用域栈，数组中元素均为头指针，本身没有含义; 栈顶为最内部作用域
Common common_table[COMMON_TABLE_SIZE];// 其他符号表
Common struct_cache;// 用来缓存结构体内部的结构，头指针没有含义

int anon_stru = 0;
int domain_top = 0;
int cur_var_no;

extern int fail;

int count_filed_num(FieldList head)
{
    int count  = 0;
    
    FieldList cur_field = head;
    while(cur_field!=NULL)
    {
        count++;
        cur_field = cur_field->tail;
    }
    return count;
}

// 初始化语义分析
void init_semantics()
{
    int i;

    // 初始化作用域栈
    for(i = 0; i < DOMAIN_LAYER;i++)
    {
        domains[i] = (Domain_Item) malloc(sizeof(struct Domain_Item_));
        domains[i]->nxt_item = NULL;
    }
    // 初始化符号表
    for(i=0; i <= SIGNAL_TABLE_SIZE;i++)
    {
        var_table[i] = (Var) malloc(sizeof(struct Var_));
        var_table[i]->nxt_var = NULL;
    }

    // 初始化其他符号表
    for(i=0; i <= COMMON_TABLE_SIZE;i++)
    {
        common_table[i] = (Common) malloc(sizeof(struct Common_));
        common_table[i]->nxt_common = NULL;
    }

    // 初始化结构体缓存
    struct_cache = (Common) malloc(sizeof(struct Common_));
    struct_cache->nxt_common = NULL;

    domain_top = 1;
    cur_var_no = 1;

    // 插入read和write函数
    Common read_common = (Common) malloc(sizeof(struct Common_));
    read_common->kind = FUNC;
    read_common->name = (char*) malloc(strlen("read")+1);
    strcpy(read_common->name,"read");
    read_common->type = (Type) malloc(sizeof(struct Type_));
    read_common->type->kind = STRUCTURE;
    read_common->type->u.structure = (FieldList) malloc(sizeof(struct FieldList_));
    read_common->type->u.structure->type = (Type) malloc(sizeof(struct Type_));
    read_common->type->u.structure->type->kind = UNKNOWN;
    read_common->type->u.structure->tail = NULL;
    insert_common(read_common);

    Common write_common = (Common) malloc(sizeof(struct Common_));
    write_common->kind = FUNC;
    write_common->name = (char*) malloc(strlen("write")+1);
    strcpy(write_common->name,"write");
    write_common->type = (Type) malloc(sizeof(struct Type_));
    write_common->type->kind = STRUCTURE;
    write_common->type->u.structure = (FieldList) malloc(sizeof(struct FieldList_));
    write_common->type->u.structure->type = (Type) malloc(sizeof(struct Type_));
    write_common->type->u.structure->type->kind = UNKNOWN;
    write_common->type->u.structure->tail = (FieldList) malloc(sizeof(struct FieldList_));
    write_common->type->u.structure->tail->type = (Type) malloc(sizeof(struct Type_));
    write_common->type->u.structure->tail->type->kind = UNKNOWN;
    write_common->type->u.structure->tail->tail = NULL;
    insert_common(write_common);
}

int create_var()
{
    return cur_var_no++;
}

// 求哈希值
unsigned hash_pjw(char* name,unsigned size)
{
    unsigned val = 0, i;
    for (; *name; ++name)
    {
        val = (val << 2) + *name;
        if (i = val & ~0x3fff) val = (val ^ (i >> 12)) & size;
    }
    return val;
}

// 根据添加代码作用域
void push_domain()
{
    if(domain_top >= DOMAIN_LAYER)
    {
        fprintf(stderr, "Error: Domain stack overflow. Please extand the Domain stack in file \"semaintic.h\" by increasing the DOAMIN_LAYER\n");
        exit(1);
    }

    domain_top++;

}

// 弹出代码作用域
void pop_domain()
{
    if(domain_top<=0)
    {
        fprintf(stderr, "Error: Domain stack is empty. There are sonething wrong in the program.\n");
        exit(1);
    }

    // 清除变量作用域的内容
    Domain_Item cur_item = domains[domain_top-1]->nxt_item;// 获取该层作用域的第一个变量
    domains[domain_top-1]->nxt_item = NULL;// 并移除
    Var tmp_signal = NULL;
    Domain_Item tmp_item = NULL;
    while(cur_item!=NULL)
    {
        unsigned hash = cur_item->var->hash;
        /*
            此处需要注意的是，处于作用域链表头的Signal肯定会处于哈希表链表头；每次新加Single都是加载链表头
        */
        if(var_table[hash]->nxt_var==cur_item->var)// 该变量在最内层作用域； 此处似乎无需判断
        {
            // 清除作用域内的变量
            var_table[hash]->nxt_var = var_table[hash]->nxt_var->nxt_var;
            tmp_signal = cur_item->var;
            free(tmp_signal);
        }
        tmp_item = cur_item;
        cur_item = cur_item->nxt_item;
        free(tmp_item);
    }

    domain_top--;
}

// 根据符号名称查询Var
Var search_var(char* name,int if_global)
{
    Domain_Item cur_item;
    // 一层一层作用域搜
    if(if_global)
    {
        
        int layer;
        for(layer = domain_top-1;layer>=0;layer--)
        {
            cur_item = domains[layer]->nxt_item;// 首先获取头
            while(cur_item!=NULL)
            {
                if(strcmp(cur_item->var->name,name)==0)
                {
                    // 搜到即停止
                    return cur_item->var;
                }
                cur_item = cur_item->nxt_item;
            }
        }
    }
    else
    {
        
        cur_item = domains[domain_top-1]->nxt_item;// 首先获取头
        
        while(cur_item!=NULL)
        {
            if(strcmp(cur_item->var->name,name)==0)
            {
                // 搜到即停止
                return cur_item->var;
            }
            cur_item = cur_item->nxt_item;
        }
        
        
    }
    // 找不到返回NULL
    return NULL;
}

// 插入符号
void insert_var(Var var)
{
    // 插入作用域栈
    Domain_Item cur_item = (Domain_Item) malloc(sizeof(struct Domain_Item_));
    cur_item->var = var;
    cur_item->nxt_item =  domains[domain_top-1]->nxt_item;
    domains[domain_top-1]->nxt_item = cur_item;

    // 插入变量/函数符号表
    unsigned hash = var->hash;
    var->nxt_var = var_table[hash]->nxt_var;
    var_table[hash]->nxt_var = var;
}

// 根据名称查询Common
Common search_common(char* name,int kind)
{
    unsigned hash = hash_pjw(name,COMMON_TABLE_SIZE);

    Common cur_common = common_table[hash]->nxt_common;
    if(kind==ALL)
    {
        
        while(cur_common!=NULL)
        {
            
            if(strcmp(name,cur_common->name)==0)
            {
                return cur_common;
            }
            cur_common = cur_common->nxt_common;
        }
    }
    else
    {
        while(cur_common!=NULL)
        {
            if(strcmp(name,cur_common->name)==0 && cur_common->kind==kind)
            {
                return cur_common;
            }
            cur_common = cur_common->nxt_common;
        }
    }
    // 找不到返回NULL
    return NULL;
}

// 插入Common
void insert_common(Common common)
{
    unsigned hash = hash_pjw(common->name,COMMON_TABLE_SIZE);
    common->nxt_common = common_table[hash]->nxt_common;
    common_table[hash]->nxt_common = common;
}

void print_common()
{
    int i ;
    for(i = 0; i < COMMON_TABLE_SIZE;i++)
    {
        Common cur_commom = common_table[i]->nxt_common;
        while(cur_commom!=NULL)
        {
            // printf("%s | ",cur_commom->name);
            cur_commom = cur_commom->nxt_common;
        }
    }
    // printf("common end\n");
}

// 根据名称查询结构体缓存中的Common
Common search_cache_common(char* name)
{
    Common cur_common = struct_cache->nxt_common;
    while(cur_common!=NULL)
    {
        if(strcmp(name,cur_common->name)==0)
        {
            return cur_common;
        }
        cur_common = cur_common->nxt_common;
    }
    // 找不到返回NULL
    return NULL;
}

// 释放struct_cache，并将内容插入到Common表中
void free_cache()
{
    Common cur_commom = struct_cache->nxt_common;
    Common nxt_common;
    while(cur_commom!=NULL)
    {
        nxt_common = cur_commom->nxt_common;
        insert_common(cur_commom);
        cur_commom = nxt_common;
    }
    struct_cache->nxt_common = NULL;
}

void print_cache()
{
    Common cur_commom = struct_cache->nxt_common;
    while(cur_commom!=NULL)
    {
        // printf("%s | ",cur_commom->name);
        cur_commom = cur_commom->nxt_common;
    }
    // printf("cache end\n");
}

// 需要进行深拷贝,目标地址为已分配好的空间
void copy_type(Type dst,Type src)
{
    // printf("debug: 进来copy_type了\n");
    // printf("%d\n",dst==NULL);
    dst->kind = src->kind;
    dst->size = src->size;
    if(src->kind==BASIC)
    {      
        // printf("debug: 进来copy_type BASIC\n");
        dst->u.basic = src->u.basic;  
    }
    else if(src->kind==ARRAY)
    {
        // printf("debug: 进来copy_type ARRAY\n");
        dst->u.array.elem = (Type) malloc(sizeof(struct Type_));
        copy_type(dst->u.array.elem,src->u.array.elem);
        dst->u.array.size = src->u.array.size;
    }
    else if(src->kind==STRUCTURE)
    {
        // printf("debug: 进来copy_type STRUCTURE\n");
        // printf("debug: 进来STRUCTURE了\n");
        FieldList tem_head = (FieldList) malloc(sizeof(struct FieldList_));// 创建一个链表头，方便找
        tem_head->tail = NULL;
        FieldList src_field = src->u.structure;

        FieldList dst_field = tem_head;
        // printf("debug: 进来STRUCTURE-1了\n");
        while(src_field!=NULL)
        {
            // printf("debug: 进来STRUCTURE-1-1了\n");
            dst_field->tail = (FieldList) malloc(sizeof(struct FieldList_));
            dst_field = dst_field->tail;
            // printf("debug: 进来STRUCTURE-1-2了\n");
            
            dst_field->name = (char*) malloc(strlen(src_field->name)+1);
            
            strcpy(dst_field->name,src_field->name);
            dst_field->type = (Type) malloc(sizeof(struct Type_));
            copy_type(dst_field->type,src_field->type);
            dst_field->tail = NULL;

            src_field = src_field->tail;

        }
        // printf("debug: 进来STRUCTURE-2了\n");
        dst->u.structure = tem_head->tail;
        free(tem_head);
        
    }
    else if(src->kind==UNKNOWN)
    {
        dst->kind==UNKNOWN;
    }
    else
    {
        fprintf(stderr, "Error: copy_type_error\n");
        exit(0);
    }
    // printf("debug: 退出copy_type了\n");
}

// 比较两个类型是否相等
int compare_type(Type dst,Type src)
{
    /*
        指针为空可能会是递归判断时某个类型的维度不足
        只要其中一个为假类型，就直接不相等
    */
    if(dst==NULL || src==NULL)
    {
        return 0;
    }
    

    // UNKNOWN在本代码中看做万能类型
    if(dst->kind==UNKNOWN || src->kind==UNKNOWN)
    {
        return 1;
    }
    if(dst->kind!=src->kind)
    {
        return 0;
    }
    if(src->kind==BASIC)
    {
        // printf("debug: 基本类型比较：%d %d\n",dst->u.basic,src->u.basic);
        return dst->u.basic == src->u.basic;
    }
    else if(src->kind==ARRAY)
    {
        return compare_type(dst->u.array.elem,src->u.array.elem);
    }
    else if(src->kind==STRUCTURE)
    {
        FieldList src_field = src->u.structure;
        
        FieldList dst_field = dst->u.structure;
        
        while(src_field!=NULL)
        {
            if(dst_field==NULL || !compare_type(dst_field->type,src_field->type))
            {
                return 0;
            }
            src_field = src_field->tail;
            dst_field = dst_field->tail;
        }

        if(dst_field!=NULL)
        {
            return 0;
        }
    }
    else
    {
        fprintf(stderr, "Error: compare_type_error\n");
        exit(0);
    }
    return 1;
}

FieldList search_field(Type type,char* field)
{
    if(type==NULL)
    {
        // fprintf(stderr, "Error : search_field type is NULL\n");
        return NULL;
    }
    else if(type->kind!=STRUCTURE)
    {
        // fprintf(stderr, "Error : search_field type is not STRUCTURE\n");
        return NULL;
    }
    FieldList cur_field = type->u.structure;
    while(cur_field!=NULL)
    {
        if(strcmp(cur_field->name,field)==0)
        {
            return cur_field;
        }
        cur_field = cur_field->tail;
    }
    return NULL;
}

// 计算继承属性，还需要将父节点传入，才能确定兄弟节点的属性
void inherited(struct Node* nd,struct Node* parent)
{
    int kid_num = parent->kid_num;
    int parent_gen = parent->generation;
    int nd_tag = nd->tag;
    switch(nd_tag)
    {

        /*
            在获取继承属性的函数中对作用域栈进行处理，似乎不太符合一般的处理思维。
            但是对于本项目而言，并没有对程序的整体正确性有影响，
            所以我把这部分代码放在这里进行偷懒，使代码简洁一点
        */
        case LC:
            // Compst -> LC DefList StmtList RC
            if(parent_gen==CompSt0)
            {
                // 根据实验三的假设，不需要进行作用域的处理
                // push_domain();
            }
            break;
        case RC:
            // Compst -> LC DefList StmtList RC
            if(parent_gen==CompSt0)
            {
                // 根据实验三的假设，不需要进行作用域的处理
                // pop_domain();
            }
            break;
        case DefList:
            // StructSpecifier -> STRUCT OptTag LC DefList RC
            if(parent_gen==StructSpecifier0)
            {
                nd->id_type = FIELD;
            }
            // CompSt -> LC DefList StmtList RC
            else if(parent_gen==CompSt0)
            {
                nd->id_type == VAR;
            }
            // DefList -> Def DefList
            else if(parent_gen==DefList0)
            {
                nd->id_type = parent->id_type;
            }
            break;
        case Def:
            // DefList -> Def DefList
            if(parent_gen==DefList0)
            {
                nd->id_type = parent->id_type;
            }
            break;
        case DecList:
            // Def -> Specifier DecList
            if(parent_gen==Def0)
            {
                // 若Specifier是StructSpecifier
                if(parent->kids[0]->if_structspecifier)
                {
                    if(parent->kids[0]->struct_specifier==NULL)
                    {
                        // 此处不需要提醒，会在前面报错使用未定义结构体
                        Type fake_type = (Type) malloc(sizeof(struct Type_));
                        fake_type->kind = UNKNOWN;
                        nd->inh_type = fake_type;
                    }
                    else
                    {
                        copy_type(nd->inh_type,parent->kids[0]->struct_specifier->type);
                    }
                    
                }
                // 若Specifier是Type
                else
                {
                    copy_type(nd->inh_type,parent->kids[0]->type);
                }
                nd->id_type = parent->id_type;
            }
            // DecList -> Dec COMMA DecList
            else if(parent_gen==DecList1)
            {
                copy_type(nd->inh_type,parent->inh_type);
                nd->id_type = parent->id_type;
            }
            break;
        case Dec:
            // DecList -> Dec
            if(parent_gen==DecList0)
            {
                copy_type(nd->inh_type,parent->inh_type);
                nd->id_type = parent->id_type;
            }
            // DecList -> Dec COMMA DecList
            else if(parent_gen==DecList1)
            {
                copy_type(nd->inh_type,parent->inh_type);
                nd->id_type = parent->id_type;
            }
            break;
        // ExtDef -> Specifier ExtDecList SEMI
        case ExtDecList:
            if(parent->kids[0]->if_structspecifier)
            {
                if(parent->kids[0]->struct_specifier==NULL)
                {
                    // 此处不需要提醒，会在前面报错使用未定义结构体
                    Type fake_type = (Type) malloc(sizeof(struct Type_));
                    fake_type->kind = UNKNOWN;
                    nd->inh_type = fake_type;
                }
                else
                {
                    copy_type(nd->inh_type,parent->kids[0]->struct_specifier->type);
                }
                
            }
            // 若Specifier是Type
            else
            {
                copy_type(nd->inh_type,parent->kids[0]->type);
            }
            break;
        case VarDec:
            // ExtDecList -> VarDec | VarDec COMMA ExtDecList
            if(parent_gen==ExtDecList0 || parent_gen==ExtDecList1)
            {
                copy_type(nd->inh_type,parent->inh_type);
                // printf("debug:VarDec inh_type: %d %d\n",nd->inh_type->kind,nd->inh_type->u.basic);
                nd->id_type = parent->id_type;
            }
            // Dec -> VarDec | VarDec ASSIGNOP Exp
            else if(parent_gen==Dec0 || parent_gen==Dec1)
            {
                copy_type(nd->inh_type,parent->inh_type);
                nd->id_type = parent->id_type;
            }
            // VarDec -> VarDec LB INT RB
            else if (parent_gen==VarDec1)
            {
                Type elem_type = (Type) malloc(sizeof(struct Type_));
                copy_type(elem_type,parent->inh_type);
                nd->inh_type->u.array.elem = elem_type;
                
                nd->inh_type->u.array.size = parent->kids[2]->value.type_int;
                nd->inh_type->kind = ARRAY;
                nd->inh_type->size = nd->inh_type->u.array.size * nd->inh_type->u.array.elem->size;

                // printf("debug: var size:%d\n",nd->inh_type->u.array.elem->size);
            }
            
            // ParamDec -> Specifier VarDec
            else if(parent_gen==ParamDec0)
            {
                // 此处把类型复制下去
                // 若Specifier是StructSpecifier
                if(parent->kids[0]->if_structspecifier)
                {
                    // printf("debug: ParamDec VarDec StructSpecifier\n");
                    if(parent->kids[0]->struct_specifier==NULL)
                    {
                        nd->inh_type->kind = UNKNOWN;
                    }
                    else
                    {
                        copy_type(nd->inh_type,parent->kids[0]->struct_specifier->type);
                    }
                    
                }
                // 若Specifier是Type
                else
                {
                    // printf("debug: ParamDec VarDec Type\n");
                    copy_type(nd->inh_type,parent->kids[0]->type);
                }

                nd->id_type = VAR;
                nd->id_usage = DEF;
            }
            break;
        case FunDec:
            // ExtDef -> Specifier FunDec CompSt
            if(parent_gen==ExtDef0)
            {
                // 若Specifier是StructSpecifier
                if(parent->kids[0]->if_structspecifier)
                {
                    if(parent->kids[0]->struct_specifier==NULL)
                    {
                        
                        nd->inh_type->kind = UNKNOWN;
                    }
                    else
                    {
                        copy_type(nd->inh_type,parent->kids[0]->struct_specifier->type);
                    }
                    
                }
                // 若Specifier是Type
                else
                {
                    copy_type(nd->inh_type,parent->kids[0]->type);
                }
            }
            break;
        case CompSt:
            // ExtDef -> Specifier FunDec CompSt
            if(parent_gen==ExtDef0)
            {
                // 若Specifier是StructSpecifier
                if(parent->kids[0]->if_structspecifier)
                {
                    if(parent->kids[0]->struct_specifier==NULL)
                    {
                        
                        nd->return_type->kind = UNKNOWN;
                    }
                    else
                    {
                        
                        copy_type(nd->return_type,parent->kids[0]->struct_specifier->type);
                    }
                    
                }
                // 若Specifier是Type
                else
                {
                    
                    copy_type(nd->return_type,parent->kids[0]->type);
                    // printf("debug: CompSt return type %d\n",nd->return_type->u.basic);
                }
            }
            // Stmt -> Compst
            else if(parent_gen==Stmt0)
            {
                copy_type(nd->return_type,parent->return_type);
                // printf("debug: CompSt return type %d\n",nd->return_type->u.basic);
            }
            break;
        case StmtList:
            // CompSt -> LC DefList StmtList RC
            if(parent_gen==CompSt0)
            {
                copy_type(nd->return_type,parent->return_type);
                // printf("debug: StmtList return type %d\n",nd->return_type->u.basic);
                
            }
            // StmtList -> Stmt StmtList
            else if(parent_gen==StmtList0)
            {
                copy_type(nd->return_type,parent->return_type);
                // printf("debug: StmtList return type %d\n",nd->return_type->u.basic);
            }
            break;
        case Stmt:
            copy_type(nd->return_type,parent->return_type);
            // // StmtList -> Stmt StmtList
            // if(parent_gen==StmtList0)
            // {
                
            // }
            break;
        case ID:
            // VarDec -> ID
            if(parent_gen==VarDec0)
            {
                copy_type(nd->inh_type,parent->inh_type);
                nd->id_type = parent->id_type;
            }
            // FunDec-> ID LP VarList RP | ID LP RP
            else if(parent_gen==FunDec0 || parent_gen==FunDec1)
            {
                nd->id_type = FUNC;
                nd->id_usage = DEF;
            }
            // OptTag -> ID
            else if(parent_gen==OptTag0)
            {
                nd->id_type = STRU;
                nd->id_usage = DEF;
            }
            // Tag -> ID
            else if(parent_gen==Tag0)
            {
                nd->id_type = STRU;
                nd->id_usage = USE;
            }
            // Exp -> ID LP Args RP | ID LP RP | Exp DOT ID | ID 
            else if(parent->tag==Exp)
            {
                switch (parent_gen)
                {
                    // Exp -> ID LP Args RP | ID LP RP
                    case Exp0:
                    case Exp1:
                        nd->id_type = FUNC;
                        break;
                    // Exp -> Exp DOT ID
                    case Exp2:
                        nd->id_type = FIELD;
                        break;
                    // Exp -> ID
                    case Exp3:
                        break;
                }
                nd->id_usage = USE;
            }
            else
            {
                printf("inh ID 错误:%d\n",parent_gen);
            }
            break;
        case Exp:
            
            break;
    }

}

void syn_Exp(struct Node* nd)
{
    // Exp -> ID | INT | FLOAT
    if(nd->kid_num==1)
    {
        // Exp -> ID
        if(nd->kids[0]->tag==ID)
        {
            // 不改动的话，已经算完成了
            copy_type(nd->type,nd->kids[0]->type);
            // if(nd->kids[0]->type->kind!=STRUCTURE)
            // {
            //     copy_type(nd->type,nd->kids[0]->type);
            // }
            // else
            // {
            //     // 如果是结构体，知道是哪一个Common条目即可
            //     nd->struct_specifier = nd->kids[0]->struct_specifier;
            // }
            nd->l_r = LEFT;
        }
        // Exp -> INT | FLOAT
        else
        {
            copy_type(nd->type,nd->kids[0]->type);    
        }
    }
    // Exp -> MINUS Exp | NOT Exp
    else if(nd->kid_num==2)
    {
        struct Node* kid_0 = nd->kids[0];
        struct Node* kid_1 = nd->kids[1];
        int check_r;
        switch(kid_0->tag)
        {
            case MINUS:
                check_r = kid_1->type->kind==UNKNOWN || kid_1->type->kind==BASIC;
                
                // 右类型需要是基础类型
                if(!check_r)
                {
                    fprintf(stderr, "<语义错误> 第%d行，错误类型7: 只有基本类型才能进行求负运算\n",kid_1->row);fail=1;
                    nd->type->kind == UNKNOWN;
                }
                else
                {
                    copy_type(nd->type,kid_1->type);
                }
                
                break;
            case NOT:
                check_r = kid_1->type->kind==UNKNOWN || (kid_1->type->kind==BASIC && kid_1->type->u.basic==INT);
                // 右类型需要是INT
                if(!check_r)
                {
                    fprintf(stderr, "<语义错误> 第%d行，错误类型7: 只有INT类型才能进行逻辑运算\n",kid_1->row);fail=1;
                }
                nd->type->kind == BASIC;
                // 默认给予正确的类型
                nd->type->u.basic = INT;
                break;
        }
    }
    // 各项运算+结构体引用: Exp -> Exp op Exp | Exp DOT ID
    else if(nd->kid_num==3)
    {
        struct Node* kid_0 = nd->kids[0];
        struct Node* kid_1 = nd->kids[1];
        struct Node* kid_2 = nd->kids[2];
        int check_l,check_r,check_ok;
        switch(kid_1->tag)
        {
            // 赋值
            case ASSIGNOP:
                // 判断左右值
                check_l = kid_0->l_r == LEFT;
                // 判断左右两边类型是否匹配
                check_ok = compare_type(kid_0->type,kid_2->type);

                // 赋值符号左边不是左值
                if(!check_l)
                {
                    fprintf(stderr, "<语义错误> 第%d行，错误类型6: 赋值号左边出现一个只有右值的表达式\n",kid_1->row);fail=1;
                    nd->type->kind == UNKNOWN;
                }
                // 左右两边类型不匹配
                else if(!check_ok)
                {
                    fprintf(stderr, "<语义错误> 第%d行，错误类型5: 赋值号两边的表达式类型不匹配\n",kid_1->row);fail=1;
                    nd->type->kind == UNKNOWN;
                }
                else
                {
                    int normal = kid_0->type!=NULL && kid_0->type->kind!=UNKNOWN 
                                && kid_1->type!=NULL && kid_1->type->kind!=UNKNOWN ;
                
                    if(normal)
                    {
                        copy_type(nd->type,kid_0->type);
                    }
                    else
                    {
                        nd->type->kind == UNKNOWN;
                    }
                }
                
                break;
            // 逻辑运算
            case AND:
            case OR:
                check_l = kid_0->type->kind==UNKNOWN || (kid_0->type->kind==BASIC && kid_0->type->u.basic==INT);
                check_r = kid_2->type->kind==UNKNOWN || (kid_2->type->kind==BASIC && kid_2->type->u.basic==INT);
                // 左右类型都需要是INT
                if(!(check_l && check_r))
                {
                    fprintf(stderr, "<语义错误> 第%d行，错误类型7: 只有INT类型才能进行逻辑运算\n",kid_1->row);fail=1;
                }
                nd->type->kind == BASIC;
                // 默认给予正确的类型
                nd->type->u.basic = INT;
                
                break;
            // 关系运算
            case RELOP:
                check_l = kid_0->type->kind==UNKNOWN || kid_0->type->kind==BASIC;
                check_r = kid_2->type->kind==UNKNOWN || kid_2->type->kind==BASIC;
                check_ok = kid_0->type->kind==UNKNOWN || kid_0->type->kind==UNKNOWN || kid_0->type->u.basic==kid_2->type->u.basic;

                // 只有基本类型可以进行关系运算
                if(!(check_l && check_r))
                {
                    fprintf(stderr, "<语义错误> 第%d行，错误类型7: 只有基本类型才能进行关系运算\n",kid_1->row);fail=1;
                }
                // 左右两边类型不匹配
                else if(!check_ok)
                {
                    fprintf(stderr, "<语义错误> 第%d行，错误类型7: 关系运算的操作数类型不匹配\n",kid_1->row);fail=1;
                }
                
                nd->type->kind == BASIC;
                // 默认给予正确的类型
                nd->type->u.basic = INT;
                
                break;
            
            // 结构体取域
            case DOT:
                int unknown_l = kid_0->type->kind == UNKNOWN;
                // 左边不是UNKNOWN
                if(!unknown_l)
                {
                    check_l =  kid_0->type->kind==STRUCTURE;
                    FieldList field = search_field(kid_0->type,kid_2->name);
                    check_ok = field!=NULL;
                    // 非结构体不能取域
                    if(!check_l)
                    {
                        fprintf(stderr, "<语义错误> 第%d行，错误类型13: 对非结构体型变量使用“.”操作符\n",kid_1->row);fail=1;
                        nd->type->kind = UNKNOWN;
                    }
                    // 结构体没有对应的域
                    else if(!check_ok)
                    {
                        fprintf(stderr, "<语义错误> 第%d行，错误类型14: 访问结构体中未定义过的域(%s)\n",kid_1->row,kid_2->name);fail=1;
                        nd->type->kind = UNKNOWN;
                    }
                    else
                    {
                        copy_type(nd->type,field->type);
                    }
                    
                    nd->l_r = LEFT;
                }
                else
                {
                    nd->l_r = LEFT;
                    nd->type->kind = UNKNOWN;
                }
                break;
            // 加一个括号
            case Exp:
                copy_type(nd->type,kid_1->type);
                break;
            // 无参函数调用
            case LP:
                Common func_common = search_common(kid_0->name,FUNC);
                // 函数未定义
                if(func_common==NULL)
                {
                    fprintf(stderr, "<语义错误> 第%d行，错误类型11: 函数(%s)在调用时未经定义\n",kid_0->row,kid_0->name);fail=1;
                    nd->type->kind = UNKNOWN;
                }
                // 函数已定义
                else 
                {
                    // 参数不匹配
                    if(func_common->type->u.structure->tail!=NULL)
                    {
                        fprintf(stderr, "<语义错误> 第%d行，错误类型9: 函数(%s)调用时实参与形参的数目不匹配\n",kid_0->row,kid_0->name);fail=1;
                    }
                    copy_type(nd->type,func_common->type->u.structure->type);
                }
 
                break;
            // 算术运算：
            default:
                check_l = kid_0->type->kind==UNKNOWN || kid_0->type->kind==BASIC;
                check_r = kid_2->type->kind==UNKNOWN || kid_2->type->kind==BASIC;
                check_ok =  kid_0->type->kind==UNKNOWN || kid_2->type->kind==UNKNOWN || 
                                kid_0->type->u.basic==kid_2->type->u.basic;
                
                // 只有基本类型可以进行算数运算
                if(!(check_l && check_r))
                {
                    fprintf(stderr, "<语义错误> 第%d行，错误类型7: 只有基本类型可以进行算术运算\n",kid_1->row);fail=1;
                    nd->type->kind = UNKNOWN;
                }
                // 左右两边类型不匹配
                else if(!check_ok)
                {
                    fprintf(stderr, "<语义错误> 第%d行，错误类型7: 算术运算中的操作数类型不匹配\n",kid_1->row);fail=1;
                    nd->type->kind = UNKNOWN;
                    
                }
                else
                {
                    copy_type(nd->type,kid_0->type);
                }
                nd->l_r = RIGHT;
                break;
        }
    }
    // Exp -> Exp LB Exp RB | ID LP Args RP
    else if(nd->kid_num==4)
    {
        struct Node* kid_0 = nd->kids[0];
        struct Node* kid_1 = nd->kids[1];
        struct Node* kid_2 = nd->kids[2];
        struct Node* kid_3 = nd->kids[3];
        int check_l,check_r;
        // 数组
        // Exp -> Exp LB Exp RB
        if(kid_2->tag==Exp)
        {
            check_l =  kid_0->type->kind==ARRAY;
            check_r = kid_2->type->kind==UNKNOWN | (kid_2->type->kind==BASIC && kid_2->type->u.basic==INT);
            if(!(kid_0->type->kind==UNKNOWN ||check_l))
            {
                // 此处在某些情况下无法获取变量名：(a+b)[2]
                fprintf(stderr, "<语义错误> 第%d行，错误类型10: 对非数组型变量使用“[…]”（数组访问）操作符\n",kid_0->row);fail=1;
                nd->type->kind = UNKNOWN;
            }
            if(!check_r)
            {
                fprintf(stderr, "<语义错误> 第%d行，错误类型12: 数组访问操作符“[…]”中出现非整数\n",kid_2->row);fail=1;
            }

            if(!check_l)
            {
                nd->type->kind = UNKNOWN;
            }
            else
            {
                copy_type(nd->type,kid_0->type->u.array.elem);
            }
            nd->l_r = LEFT;
        }
        // 有参函数调用
        // Exp -> ID LP Args RP
        else
        {
            Common func_common = search_common(kid_0->name,FUNC);
            // 函数未定义
            if(func_common==NULL)
            {
                fprintf(stderr, "<语义错误> 第%d行，错误类型2: 函数(%s)在调用时未经定义\n",kid_0->row,kid_0->name);fail=1;
                nd->type->kind = UNKNOWN;
            }
            // 函数已定义
            else 
            {

                // 比较参数
                FieldList paras =  func_common->type->u.structure->tail;
                FieldList args = kid_2->type->u.structure;
                int count = 0;
                while(paras!=NULL)
                {
                    if(args==NULL)
                    {
                        fprintf(stderr, "<语义错误> 第%d行，错误类型9: 函数(%s)调用时实参与形参的数目不匹配\n",kid_0->row,kid_0->name);fail=1;
                        break;
                    }
                    if(!compare_type(paras->type,args->type))
                    {
                        fprintf(stderr, "<语义错误> 第%d行，错误类型9: 函数(%s)调用时实参与形参的类型不匹配\n",kid_0->row,kid_0->name);fail=1;
                    }
                    count += 1;
                    paras = paras->tail;
                    args = args->tail;
                }
                if(args!=NULL)
                {
                    fprintf(stderr, "<语义错误> 第%d行，错误类型9: 函数(%s)调用时实参与形参的数目不匹配\n",kid_0->row,kid_0->name);fail=1;
                }
                copy_type(nd->type,func_common->type->u.structure->type);
            }
        }
    }
            
}

void syn_StructSpecifier(struct Node* nd)
{
    // StructSpecifier -> STRUCT Tag
    if(nd->kid_num==2)
    {
        nd->type = (Type) malloc(sizeof(struct Type_));
        nd->type->kind = STRUCTURE;
        Common common = search_common(nd->kids[1]->name,STRU);
        // 使用了未声明的结构体
        if(common==NULL)
        {
            fprintf(stderr, "<语义错误> 第%d行，错误类型17: 直接使用未定义过的结构体(%s)来定义变量\n",nd->kids[1]->row,nd->kids[1]->name);fail=1;
            nd->struct_specifier = NULL;
        }
        else
        {
            // 从Common表中获取结构体信息
            nd->struct_specifier = common;
        }
        
    }
    // StructSpecifier -> STRUCT OptTag LC DefList RC
    if(nd->kid_num==5)
    {
        
        struct Node* kid_1 = nd->kids[1];
        struct Node* kid_3 = nd->kids[3];
        // 注意: 这里只为匿名结构体分配了20字节的名字空间，一般情况下已经够用了
        char* struct_name = (char*) malloc(20);
        // 匿名结构体
        if(kid_1->if_empty)
        {
            sprintf(struct_name,"%d",anon_stru++);
        }
        // 非匿名结构体
        else
        {
            struct_name = malloc(strlen(kid_1->name)+1);
            strcpy(struct_name,kid_1->name);
        }

        // print_cache();

        // 释放struct_cache
        free_cache();
        // printf("free_cache\n");
        // print_common();

        // 查询符号表
        Var old_var = search_var(struct_name,1);
        
        Common old_common = search_common(struct_name,ALL);

        // 该ID已经被变量占用
        if(old_var!=NULL)
        {
            fprintf(stderr, "<语义错误> 第%d行，错误类型16：结构体名(%s)与前面定义过的结构体或变量的名字重复\n",kid_1->row,struct_name);fail=1;
        }
        // 该名称已被使用
        if(old_common!=NULL)
        {
            switch (old_common->kind)
            {
                case STRU:
                    fprintf(stderr, "<语义错误> 第%d行，错误类型16：结构体名(%s)与前面定义过的结构体或变量的名字重复\n",kid_1->row,struct_name);fail=1;
                    break;
                case FUNC:
                    // fprintf(stderr, "<语义错误> 第%d行，错误类型3：变量(%s)与前面定义/声明过的函数名字重复\n",kid_1->row,struct_name);
                    break;
                case FIELD:
                    fprintf(stderr, "<语义错误> 第%d行，错误类型16：结构体名(%s)与前面定义过的结构体中的域名/自身内部域名重复\n",kid_1->row,struct_name);fail=1;
                    break;
                default:
                    break;
            }
        }
        /*
            为什么重名了还是要插入Common表呢？
            这是为了让语义分析可以进行下去，不然如果确定不了一个struct的结构，后面的分析可能很难进行
        */
        if(old_common==NULL || (old_common!=NULL && old_common->kind!=STRU))
        {
            // 构建要插入的Common条目
            Common struct_common = (Common) malloc(sizeof(struct Common_));
            struct_common->kind = STRU;
            struct_common->name = (char*) malloc(strlen(struct_name)+1);
            strcpy(struct_common->name,struct_name);
            struct_common->type = (Type) malloc(sizeof(struct Type_));
            
            copy_type(struct_common->type,kid_3->type);

            struct_common->nxt_common = NULL;
            insert_common(struct_common);
            

            nd->struct_specifier = struct_common;
        }
    }
}


void synthesized(struct Node* nd)
{
    int kid_num = nd->kid_num;
    int nd_tag = nd->tag;

    switch(nd_tag)
    {
        // 类型标号
        case TYPE:
            // 在词法分析部分已经完成
            if(strcmp(nd->name,"int")==0)
            {
                nd->type = (Type) malloc(sizeof(struct Type_));
                nd->type->kind = BASIC;
                nd->type->u.basic = INT;
                nd->type->size = 4;
            }
            else if(strcmp(nd->name,"float")==0)
            {
                nd->type = (Type) malloc(sizeof(struct Type_));
                nd->type->kind = BASIC;
                nd->type->u.basic = FLOAT;
                nd->type->size = 4;
            }
            break;
        // 结构体标识符标号
        case StructSpecifier:
            syn_StructSpecifier(nd);
            break;
        case Tag:
            // Tag -> ID
            nd->name = (char*) malloc(strlen(nd->kids[0]->name)+1);
            strcpy(nd->name,nd->kids[0]->name);
            break;
        case OptTag:
            // OptTag -> ID
            if(!nd->if_empty)
            {
                nd->name = (char*) malloc(strlen(nd->kids[0]->name)+1);
                strcpy(nd->name,nd->kids[0]->name);

            }
            // OptTag -> 不处理
            else
            {
                nd->name = NULL;
            }            
            break;
        case Stmt:
            if(kid_num==3)
            {
                // Stmt -> RETURN Exp SEMI
                if(nd->kids[0]->tag==RETURN)
                {
                    if(!compare_type(nd->return_type,nd->kids[1]->type))
                    {                        
                        fprintf(stderr, "<语义错误> 第%d行，错误类型8: 函数返回值类型不正确\n",nd->row);fail=1;
                    }
                }
            }
            // Stmt -> IF LP Exp RP Stmt | IF LP Exp RP Stmt ELSE Stmt | WHILE LP Exp RP Stmt
            else if(kid_num>=5)
            {
                struct Node* kid_2 = nd->kids[2];
                int check_ok  = kid_2->type->kind==UNKNOWN || 
                                    (kid_2->type->kind==BASIC && kid_2->type->u.basic==INT);
                if(!check_ok)
                {
                    fprintf(stderr, "<语义错误> 第%d行，错误类型21: 只有INT类型才能作为if和while语句的条件\n",nd->row);fail=1;
                }
            }
            break;
        case Exp:
            syn_Exp(nd);
            break;
        case Specifier:
            struct Node* kid_0 = nd->kids[0];
            // Specifier -> TYPE | StructSpecifier
            switch(kid_0->tag)
            {
                case TYPE:
                    copy_type(nd->type,kid_0->type);
                    break;
                case StructSpecifier:
                    nd->struct_specifier = kid_0->struct_specifier;
                    nd->if_structspecifier = 1;
                    break;
            }
            break;

        case DefList:
            // DefList -> Def DefList
            if(nd->kid_num==2)
            {
                // printf("debug: DefList syn 入口\n");
                struct Node* kid_0 = nd->kids[0];
                struct Node* kid_1 = nd->kids[1];

                copy_type(nd->type,kid_0->type);
                // 这里是为了计算结构体各个DefList的大小
                int cur_size = nd->type->size;
                
                // printf("debug: DefList syn 1\n");
                if(!kid_1->if_empty)
                {
                    Type kid_1_type = (Type) malloc(sizeof(struct Type_));
                    copy_type(kid_1_type,kid_1->type);
                    cur_size += kid_1_type->size;
                    FieldList nd_type_tail = nd->type->u.structure;
                    
                    while(nd_type_tail->tail!=NULL)
                    {
                        
                        nd_type_tail = nd_type_tail->tail;
                    }
                    nd_type_tail->tail = kid_1_type->u.structure;
                    
                    free(kid_1_type);
                }
                // printf("debug: DefList syn 2\n");
                nd->type->kind = STRUCTURE;
                nd->type->size = cur_size; 
                // printf("debug: DefList_size:%d\n",nd->type->size);
                // printf("debug: DefList syn 出口\n");
            }
            // DefList -> 不处理
            break;
        case Def:
            /*
                此处为什么要将DecList的类型继续往上传呢？
                原因是要确定结构体的结构: 
                StructSpecifier -> ... DefList ...
                DefList -> | Def DefList
                其他地方不再赘述
            */
            // Def -> Specifier DecList SEMI
            struct Node* kid_1 = nd->kids[1];
            copy_type(nd->type,kid_1->type);

            break;
        case DecList:
            // DecList -> Dec
            if(kid_num==1)
            {
                struct Node* kid_0 = nd->kids[0];

                nd->type->kind = STRUCTURE;
                FieldList tem_structure = (FieldList) malloc(sizeof(struct FieldList_));
                
                tem_structure->name = (char*) malloc(strlen(kid_0->name)+1);
                strcpy(tem_structure->name,kid_0->name);
                tem_structure->type = (Type) malloc(sizeof(struct Type_));
                copy_type(tem_structure->type,kid_0->type);
                tem_structure->tail = NULL;
                
                nd->type->u.structure = tem_structure;
                nd->type->size = tem_structure->type->size;
            }
            // DecList -> Dec COMMA DecList
            else if(kid_num==3)
            {
                struct Node* kid_0 = nd->kids[0];
                struct Node* kid_2 = nd->kids[2];

                // 将DecList中的元素添加至Type中
                copy_type(nd->type,kid_2->type);

                FieldList tem_structure = (FieldList) malloc(sizeof(struct FieldList_));
                
                tem_structure->name = (char*) malloc(strlen(kid_0->name)+1);
                strcpy(tem_structure->name,kid_0->name);
                tem_structure->type = (Type) malloc(sizeof(struct Type_));
                copy_type(tem_structure->type,kid_0->type);
                
                // 将Dec中的元素添加至Type中
                tem_structure->tail = nd->type->u.structure;
                nd->type->u.structure = tem_structure;

                nd->type->size += tem_structure->type->size;
            }
            break;
        case Dec:
            // Dec -> VarDec
            if(kid_num==1)
            {
                struct Node* kid_0 = nd->kids[0];
                nd->name = (char*) malloc(strlen(kid_0->name)+1);

                strcpy(nd->name,kid_0->name);
                copy_type(nd->type,kid_0->VarDec_type);
            }
            // Dec -> VarDec ASSIGNOP Exp
            else if(kid_num==3)
            {
                struct Node* kid_0 = nd->kids[0];
                struct Node* kid_2 = nd->kids[2];
                nd->name = (char*) malloc(strlen(kid_0->name)+1);
                strcpy(nd->name,kid_0->name);
                copy_type(nd->type,kid_0->VarDec_type);
                
                // 结构体的域不允许初始化
                if(kid_0->id_type==FIELD)
                {
                    fprintf(stderr, "<语义错误> 第%d行，错误类型15: 在定义时对域进行初始化\n",kid_1->row);fail=1;
                }
                // 判断ASSIGNOP是否合法
                if(!compare_type(kid_0->VarDec_type,kid_2->type))
                {
                    fprintf(stderr, "<语义错误> 第%d行，错误类型5: 赋值号两边的表达式类型不匹配\n",kid_1->row);fail=1;
                }

                
            }

        case VarDec:
                        
            // VarDec -> ID
            if(kid_num==1)
            {
                // printf("debug: syn VarDec 断点1\n");
                struct Node* kid_0 = nd->kids[0];
                nd->name = (char*) malloc(strlen(kid_0->name)+1);
                strcpy(nd->name,kid_0->name);
                // printf("debug: syn VarDec 断点2\n");
                copy_type(nd->VarDec_type,kid_0->type);
                // printf("debug: syn VarDec 断点3\n");

            }
            // VarDec -> VarDec LB INT RB
            else if(kid_num==4)
            {
                // printf("debug: VarDec syn 入口\n");
                struct Node* kid_0 = nd->kids[0];
                struct Node* kid_2 = nd->kids[2];
                nd->type = nd->inh_type;
                copy_type(nd->VarDec_type,kid_0->VarDec_type);
                // printf("debug: VarDec syn 1\n");
                nd->VarDec_type->u.array.size = kid_2->value.type_int;
                nd->name = (char*) malloc(strlen(kid_0->name)+1);
                strcpy(nd->name,kid_0->name);
                // printf("debug: VarDec syn 2\n");
                // TODO: 此处存疑
                // nd->type->size = nd->type->u.array.size * nd->type->u.array.elem->size;
                // printf("debug: VarDec syn 出口\n");
                
            }
            
            break;
        case FunDec:
            // 只需判断该函数是否已经被定义即可，没要求不能与结构体、变量重名
            kid_0 = nd->kids[0];
            Common old_func = search_common(kid_0->name,FUNC);
            // 已被定义则报错
            if(old_func!=NULL)
            {
                fprintf(stderr, "<语义错误> 第%d行，错误类型4: 函数出现重复定义\n",nd->row);fail=1;
            }
            // 没被定义则插入
            else
            {
                // FunDec -> ID LP RP
                if(kid_num==3)
                {
                    Common func_common = (Common) malloc(sizeof(struct Common_));
                    
                    func_common->kind = FUNC;
                    func_common->name = (char*) malloc(strlen(kid_0->name));
                    strcpy(func_common->name,kid_0->name);
                    // 记录函数结构
                    Type new_type = (Type) malloc(sizeof(struct Type_));
                    new_type->kind = STRUCTURE;
                    new_type->u.structure = (FieldList) malloc(sizeof(struct FieldList_));
                    new_type->u.structure->type = (Type) malloc(sizeof(struct Type_));
                    copy_type(new_type->u.structure->type,nd->inh_type);
                    // TODO: 此处考虑一下要不要将FieldList断尾
                    new_type->u.structure->tail = NULL;

                    func_common->type = new_type;
                    func_common->nxt_common = NULL;
                    
                    insert_common(func_common);
                }
                // FunDec -> ID LP VarList RP  
                else if(kid_num==4)
                {
                    
                    struct Node* kid_2 = nd->kids[2];
                    Common func_common = (Common) malloc(sizeof(struct Common_));
                    
                    func_common->kind = FUNC;
                    func_common->name = (char*) malloc(strlen(kid_0->name));
                    strcpy(func_common->name,kid_0->name);
                    
                    // 记录函数结构
                    // 记录返回值类型
                    Type new_type = (Type) malloc(sizeof(struct Type_));
                    new_type->kind = STRUCTURE;
                    new_type->u.structure = (FieldList) malloc(sizeof(struct FieldList_));
                    new_type->u.structure->type = (Type) malloc(sizeof(struct Type_));
                    copy_type(new_type->u.structure->type,nd->inh_type);

                    
                    // 记录参数类型
                    Type args_type = (Type) malloc(sizeof(struct Type_));
                    copy_type(args_type,kid_2->type);
                    
                    // 拼接结构体
                    new_type->u.structure->tail = args_type->u.structure;
                    free(args_type);
                    func_common->type = new_type;
                    func_common->nxt_common = NULL;

                    
                    insert_common(func_common);
                }
            }
            break;
        case VarList:
            
            // VarList -> ParamDec
            if(nd->kid_num==1)
            {
                
                nd->type->kind = STRUCTURE;
                nd->type->u.structure = (FieldList) malloc(sizeof(struct FieldList_));
                nd->type->u.structure->type = (Type) malloc(sizeof(struct Type_));

                nd->type->u.structure->name = (char*) malloc(strlen(nd->kids[0]->name)+1);
                strcpy(nd->type->u.structure->name,nd->kids[0]->name);
                
                copy_type(nd->type->u.structure->type,nd->kids[0]->type);
                nd->type->u.structure->tail = NULL;
            }
            // VarList -> ParamDec COMMA VarList
            else
            {
                copy_type(nd->type,nd->kids[2]->type);
                FieldList new_field = (FieldList) malloc(sizeof(struct FieldList_));
                new_field->name = (char*) malloc(strlen(nd->kids[0]->name)+1);
                strcpy(new_field->name,nd->kids[0]->name);
                new_field->type = (Type) malloc(sizeof(struct Type_));
                copy_type(new_field->type,nd->kids[0]->type);

                new_field->tail =  nd->type->u.structure->tail;
                nd->type->u.structure->tail = new_field;
            }
            break;
        // ParamDec -> Specifier VarDec
        case ParamDec:
            nd->name = (char*) malloc(strlen(nd->kids[1]->name)+1);
            strcpy(nd->name,nd->kids[1]->name);
            copy_type(nd->type,nd->kids[1]->VarDec_type);

            // // 查询符号表
            // Var old_var = search_var(nd->name,0);
            // Common old_common = search_common(nd->name,ALL);
            
            // // 该ID未被占用
            // if(old_var==NULL && old_common==NULL)
            // {
            //     // 更新VAR符号表
            //     Var new_var = (Var) malloc(sizeof(struct Var_));
                
            //     new_var->name = (char*) malloc(strlen(nd->name)+1);
                
            //     strcpy(new_var->name,nd->name);
                
            //     new_var->type = (Type) malloc(sizeof(struct Type_));
            //     copy_type(new_var->type,nd->type);
            //     new_var->hash = hash_pjw(new_var->name,SIGNAL_TABLE_SIZE);
                
            //     new_var->nxt_var = NULL;
                
                
            //     insert_var(new_var);
            // }
            // // 变量重复定义
            // if(old_var!=NULL)
            // {
    
            //     fprintf(stderr, "<语义错误> 第%d行,错误类型3：变量(%s)出现重复定义\n",kid_1->row,kid_1->name);fail=1;
            // }
            // // 该名称已被使用
            // if(old_common!=NULL)
            // {
            //     switch (old_common->kind)
            //     {
            //         case STRU:
            //             fprintf(stderr, "<语义错误> 第%d行,错误类型3：变量(%s)与前面定义过的结构体名字重复\n",kid_1->row,kid_1->name);fail=1;
            //             break;
            //         case FUNC:
            //             // fprintf(stderr, "<语义错误> 第%d行错误类型3：变量(%s)与前面定义/声明过的函数名字重复\n",kid_1->row,kid_1->name);fail=1;
            //             break;
            //         case FIELD:
            //             fprintf(stderr, "<语义错误> 第%d行,错误类型3：变量(%s)与前面定义过的结构体中的域名字重复\n",kid_1->row,kid_1->name);fail=1;
            //             break;
            //         default:
            //             break;
            //     }
            // }
            break;
        case Args:
            // Args -> Exp
            if(nd->kid_num==1)
            {
                
                nd->type->kind = STRUCTURE;
                nd->type->u.structure = (FieldList) malloc(sizeof(struct FieldList_));
                nd->type->u.structure->type = (Type) malloc(sizeof(struct Type_));

                nd->type->u.structure->name = (char*) malloc(5);
                strcpy(nd->type->u.structure->name,"anon");
                
                copy_type(nd->type->u.structure->type,nd->kids[0]->type);
                nd->type->u.structure->tail = NULL;
            }
            // Args -> Exp COMMA Args
            else
            {
                
                copy_type(nd->type,nd->kids[2]->type);
                FieldList new_field = (FieldList) malloc(sizeof(struct FieldList_));
                new_field->name = (char*) malloc(5);
                strcpy(new_field->name,"anon");
                new_field->type = (Type) malloc(sizeof(struct Type_));
                copy_type(new_field->type,nd->kids[0]->type);

                new_field->tail =  nd->type->u.structure->tail;
                nd->type->u.structure->tail = new_field;
                // printf("debug: 进来2了\n");
            }
            break;
        case ID:
            // ID用于定义
            if(nd->id_usage==DEF)
            {
                // printf("debug: ID定义 入口\n");
                switch(nd->id_type)
                {
                    // VAR和FIELD可能是数组类型，不能直接在底层插入
                    case VAR:
                        // printf("debug: ID 断点1\n");
                        copy_type(nd->type,nd->inh_type);
                        // 查询符号表
                        Var old_var = search_var(nd->name,0);
                        Common old_common = search_common(nd->name,ALL);
                        
                        // 该ID未被占用
                        if(old_var==NULL && old_common==NULL)
                        {
                            // 更新VAR符号表
                            Var new_var = (Var) malloc(sizeof(struct Var_));
                            
                            new_var->name = (char*) malloc(strlen(nd->name)+1);
                            
                            strcpy(new_var->name,nd->name);
                            
                            new_var->type = (Type) malloc(sizeof(struct Type_));
                            copy_type(new_var->type,nd->type);
                            new_var->hash = hash_pjw(new_var->name,SIGNAL_TABLE_SIZE);
                            
                            new_var->nxt_var = NULL;
                            new_var->var_no = create_var();
                            

                            insert_var(new_var);
                        }
                        // printf("debug: ID 断点2\n");
                        // 变量重复定义
                        if(old_var!=NULL)
                        {
                            fprintf(stderr, "<语义错误> 第%d行,错误类型3：变量(%s)出现重复定义\n",nd->row,nd->name);fail=1;
                        }
                        // printf("debug: ID 断点3\n");
                        // 该名称已被使用
                        if(old_common!=NULL)
                        {
                            
                            switch (old_common->kind)
                            {
                                case STRU:
                                    fprintf(stderr, "<语义错误> 第%d行,错误类型3：变量(%s)与前面定义过的结构体名字重复\n",nd->row,nd->name);fail=1;
                                    break;
                                case FUNC:
                                    // fprintf(stderr, "<语义错误> 第%d行错误类型3：变量(%s)与前面定义/声明过的函数名字重复\n",nd->row,nd->name);fail=1;
                                    break;
                                case FIELD:
                                    fprintf(stderr, "<语义错误> 第%d行,错误类型3：变量(%s)与前面定义过的结构体中的域名字重复\n",nd->row,nd->name);fail=1;
                                    break;
                                default:
                                    break;
                            }
                        }
                        break;
                    case FIELD:
                        copy_type(nd->type,nd->inh_type);
                        struct Node* kid_0 = nd->kids[0];
                        // 查询符号表
                        old_var = search_var(nd->name,0);
                        old_common = search_common(nd->name,ALL);
                        Common cache_common = search_cache_common(nd->name);

                        // 该ID未被占用
                        if(old_var==NULL && old_common==NULL && cache_common==NULL)
                        {
                            // 更新struct_cache
                            Common new_common = (Common) malloc(sizeof(struct Common_));

                            new_common->kind = FIELD;
                            new_common->name = (char*) malloc(strlen(nd->name)+1);
                            strcpy(new_common->name,nd->name);
                            new_common->type = (Type) malloc(sizeof(struct Type_));
                            copy_type(new_common->type,nd->type);
                            new_common->nxt_common = NULL;
                            
                            new_common->nxt_common = struct_cache->nxt_common;
                            struct_cache->nxt_common = new_common;
                            // print_cache();
                        }
                        // 该ID已经被变量占用
                        if(old_var!=NULL)
                        {
                            fprintf(stderr, "<语义错误> 第%d行，错误类型20：域名(%s)与前面定义过的变量的名字重复\n",nd->row,nd->name);fail=1;
                        }
                        // 该名称已被使用
                        if(old_common!=NULL)
                        {
                            switch (old_common->kind)
                            {
                                case STRU:
                                    fprintf(stderr, "<语义错误> 第%d行，错误类型20：域名(%s)与前面定义的结构体名字重复\n",nd->row,nd->name);fail=1;
                                    break;
                                case FUNC:
                                    // fprintf(stderr, "<语义错误> 第%d行，错误类型3：变量(%s)与前面定义/声明过的函数名字重复\n",nd->row,nd->name);
                                    break;
                                case FIELD:
                                    fprintf(stderr, "<语义错误> 第%d行，错误类型20：不同结构体中域名(%s)重复\n",nd->row,nd->name);fail=1;
                                    break;
                                default:
                                    break;
                            }
                        }
                        // 该结构体中的域名重复
                        if(cache_common!=NULL)
                        {
                            fprintf(stderr, "<语义错误> 第%d行，错误类型15：结构体中域名(%s)重复定义\n",nd->row,nd->name);fail=1;
                        }
                        break;
                    // FUNC和STRU不可以一开始就确定具体结构，所有要留到上一级的综合属性中计算
                    case FUNC:
                    // StructSpecifier -> STRUCT OptTag LC DefList RC -> STRUCT ID LC DefList RC
                    case STRU:
                        break;
                
                }
                // printf("debug: ID定义 出口\n");
            }
            // ID用于使用
            else
            {
                switch(nd->id_type)
                {
                    case VAR:
                        // 查询符号表
                        Var old_var = search_var(nd->name,1);

                        // 该变量未定义
                        if(old_var==NULL)
                        {
                            fprintf(stderr,"<语义错误> 第%d行，错误类型1：变量(%s)在使用时未经定义\n",nd->row,nd->name);fail=1;
                            nd->type->kind = UNKNOWN;
                        }
                        else
                        {
                            copy_type(nd->type,old_var->type);
                        }
                        
                        break;
                    /*
                        FUNC、STRU、FIELD中的ID可以留到再上一级处理，因为直路树
                        
                        StructSpecifier -> STRUCT Tag -> STRUCT ID

                        FunDec -> ID LP VarList RP | ID LP RP

                        Exp -> Exp DOT ID
                    */
                    case FUNC:
                    case STRU:
                    case FIELD:
                        break;
                }
            }
            
            break;
        default:
            break;
    }
}


void r_s(struct Node* nd)
{
    // treePrintLevel(nd);
    // printf("    inh\n");

    if(nd->if_empty==1)
    {
        return;
    }
    
    int i; 
    int kid_num = nd->kid_num;
    
    for(i = 0; i < kid_num;i++)
    {
        // 处理继承属性
        inherited(nd->kids[i],nd);
        r_s(nd->kids[i]);
    }

    // 处理综合属性
    synthesized(nd);

    // treePrintLevel(nd);
    // printf("    syn\n");
}

void recursive_semantic(struct Node* nd)
{
    if(nd==NULL)
    {
        fprintf(stderr, "Fail building syntax tree\n");
        // printf("poniter is null\n");
        return;
    }
    init_semantics();
    nd->level = 0;
    r_s(nd);
}

