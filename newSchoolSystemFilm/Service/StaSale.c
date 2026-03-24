#define _CRT_SECURE_NO_WARNINGS 1
#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include"Account.h"
#include <stdlib.h>
#include <string.h>
#include<time.h>
#include "StaSales.h"
#include "../Persistence/StaSale_Persis.h"
#include "../common/List.h"
#include"Sale.h"
//实现持久化层函数


// 实现用户查找函数



// 辅助函数：日期比较
static int DateCmp(ttms_date_t d1, ttms_date_t d2) {
    if (d1.year != d2.year)
        return d1.year > d2.year ? 1 : -1;
    if (d1.month != d2.month)
        return d1.month > d2.month ? 1 : -1;
    if (d1.day != d2.day)
        return d1.day > d2.day ? 1 : -1;
    return 0;
}

// 根据ID获取销售额统计
int StaSales_Srv_CompSaleVal(int usrID, ttms_date_t stDate, ttms_date_t endDate) {
    int amount = 0;

    // 定义链表变量
    sale_list_t saleList;

    // 定义链表节点指针，用于遍历
    sale_node_t* pSale;

    // 初始化链表（适配项目宏，传节点类型）
    List_Init(saleList, sale_node_t);

    // 调用持久化层函数，加载该售票员的所有销售记录到链表
    Sale_Perst_SelByID(saleList, usrID);

    // 遍历链表，累加符合日期区间的销售金额（用项目自带的 List_ForEach 宏）
    List_ForEach(saleList, pSale) {
        // 判断当前销售记录的日期是否在 [stDate, endDate] 区间内
        if (DateCmp(pSale->data.date, stDate) >= 0 &&
            DateCmp(pSale->data.date, endDate) <= 0) {
            amount += pSale->data.value;  // 结构体成员是 price，不是 value
        }
    }

    // 销毁链表，释放内存（适配项目宏，传节点类型）
    List_Destroy(saleList, sale_node_t);

    // 返回总销售额
    return amount;
}