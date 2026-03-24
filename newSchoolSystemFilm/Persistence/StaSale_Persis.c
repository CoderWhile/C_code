#define _CRT_SECURE_NO_WARNINGS 1
#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <stdlib.h>
#include"../Service/Sale.h"
#include "StaSale_Persis.h"
#include "../common/List.h"

int Sale_Perst_SelByID(sale_list_t list, int usrID) {
    FILE* fp = fopen("Sale.dat", "rb");
    if (fp == NULL) {
        return 0;
    }

    sale_t temp;
    int count = 0;

    while (fread(&temp, sizeof(sale_t), 1, fp) == 1) {
        if (temp.user_id == usrID) {
            // 创建新节点
            sale_node_t* new_node = (sale_node_t*)malloc(sizeof(sale_node_t));
            new_node->data = temp;
            new_node->next = NULL;
            new_node->prev = NULL;

            // 用项目自带的尾插法宏插入链表
            List_AddTail(list, new_node);
            count++;
        }
    }

    fclose(fp);
    return count;
}