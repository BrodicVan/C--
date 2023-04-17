#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "syntax.tab.h"
#include "tree_semantics_raw.h"

Signal signal_table[SIGNAL_TABLE_SIZE+1];// 符号表，数组中元素均为头指针，本身没有含义
Domain_Item domains[DOMAIN_LAYER];// 作用域栈，数组中元素均为头指针，本身没有含义; 栈顶为最内部作用域
int domain_top = 0;

// 初始化语义分析
void init_semantics()
{
    domain_top = 0;
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
        signal_table[i] = (Signal) malloc(sizeof(struct Signal_));
        signal_table[i]->nxt_signal = NULL;
    }
    
}

// 求哈希值
unsigned hash_pjw(char* name)
{
    unsigned val = 0, i;
    for (; *name; ++name)
    {
        val = (val << 2) + *name;
        if (i = val & ~0x3fff) val = (val ^ (i >> 12)) & SIGNAL_TABLE_SIZE;
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
    Signal tmp_signal = NULL;
    Domain_Item tmp_item = NULL;
    while(cur_item!=NULL)
    {
        unsigned hash = cur_item->signal->hash;
        /*
            此处需要注意的是，处于作用域链表头的Signal肯定会处于哈希表链表头；每次新加Single都是加载链表头
        */
        if(signal_table[hash]->nxt_signal==cur_item->signal)// 该变量在最内层作用域； 此处似乎无需判断
        {
            // 清除作用域内的变量
            signal_table[hash]->nxt_signal = signal_table[hash]->nxt_signal->nxt_signal;
            tmp_signal = cur_item->signal;
            free(tmp_signal);
        }
        tmp_item = cur_item;
        cur_item = cur_item->nxt_item;
        free(tmp_item);
    }

    domain_top--;
}

// 根据符号名称查询Single
Signal search_signal(char* name)
{
    int layer;
    Domain_Item cur_item;
    // 一层一层作用域搜
    for(layer = domain_top-1;layer>=0;layer--)
    {
        cur_item = domains[layer]->nxt_item;// 首先获取头
        while(cur_item!=NULL)
        {
            if(strcmp(cur_item->signal->name,name)==0)
            {
                // 搜到即停止
                return cur_item->signal;
            }
        }
    }
    // 找不到返回NULL
    return NULL;
}

// 插入符号
void insert_signal(Signal signal)
{
    // 插入作用域栈
    Domain_Item cur_item = (Domain_Item) malloc(sizeof(struct Domain_Item_));
    cur_item->signal = signal;
    cur_item->nxt_item =  domains[domain_top-1]->nxt_item;
    domains[domain_top-1]->nxt_item = cur_item;

    // 插入变量/函数符号表
    unsigned hash = signal->hash;
    signal->nxt_signal = signal_table[hash]->nxt_signal;
    signal_table[hash]->nxt_signal = signal;
}

void inherited()
{

}

void synthesized()
{

}

void r_s(struct Node* nd)
{
    inherited();
    if(nd->if_empty==1)
    {
        return;
    }
    int i; 
    int kid_num = nd->kid_num;
    int nd_tag = nd->tag;
    switch(nd_tag)
    {
        // 类型标号
        case TYPE:
            if(strcmp(nd->name,"int")==0)
            {
                nd->type = (Type) malloc(sizeof(struct Type_));
                nd->type->kind = BASIC;
                nd->type->u.basic = INT;
            }
            if(strcmp(nd->name,"float")==0)
            {
                nd->type = (Type) malloc(sizeof(struct Type_));
                nd->type->kind = BASIC;
                nd->type->u.basic = FLOAT;
            }
            break;
        // 结构体标识符标号
        case StructSpecifier:
            // StructSpecifier -> STRUCT Tag
            if(nd->kid_num==2)
            {
                nd->type = (Type) malloc(sizeof(struct Type_));
                nd->type->kind = STRUCTURE;
                Signal s = search_signal(nd->kids[1]->name);
                // 使用了未声明的结构体
                if(s==NULL)
                {
                    // 此处要进行报错
                }
                else
                {

                }
                
            }
            if(nd->kid_num==5)
            {
                nd->type = (Type) malloc(sizeof(struct Type_));
                nd->type->kind = STRUCTURE;
                
            }
            break;
        default:
            break;
    }
    
    synthesized();
}

void recursive_semantic(struct Node* nd)
{
    if(nd==NULL)
    {
        fprintf(stderr, "Fail building syntax tree\n");
        // printf("poniter is null\n");
        return;
    }
    nd->level = 0;
    r_s(nd);
}

