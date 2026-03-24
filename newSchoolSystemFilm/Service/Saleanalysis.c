#define _CRT_SECURE_NO_WARNINGS 1
#define _CRT_SECURE_NO_WARNINGS
/*
* 用例编号：TTMS_UC_11
* File name: SalesAnalysis.h
* Description : 统计票房逻辑层
*/

#include"../Persistence/Sale_Persist.h"
#include "Saleanalysis.h"
#include "Play.h"
#include "Sale.h"
#include "Ticket.h"
#include "../Service/ShowPlan.h"
#include "../Common/List.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include"../Persistence/Saleanalysis_Persis.h"


#define SALE_DATA_FILE "Sale.dat"

int List_Count(salesanalysis_list_t list) {
    

    // 方式B：若未维护size字段，遍历统计（兼容旧版链表）
    
    int count = 0;
    salesanalysis_node_t* pos = list->next;
    while (pos != list) {
        count++;
        pos = pos->next;
    }
    return count;
    
}

//获取剧目票房
int SalesAnalysis_Srv_StaticSale(salesanalysis_list_t list) {
    //断言链表是否存在
    if (list == NULL) {
        printf("错误：统计链表不存在！\n");
        return 0;
    }

    //定义剧目链表相关变量
    play_list_t playList;
    play_node_t* pos;

    //定义统计链表新节点指针
    salesanalysis_node_t* newNode;

    //定义已售有效票数变量
    int sold;

    //释放统计链表中原有数据节点
    List_Free(list, salesanalysis_node_t);

    //初始化剧目链表
    List_Init(playList, play_node_t);

    //从文件载入所有剧目信息构建playList链表
    Play_Srv_FetchAll(playList);

    //遍历剧目链表，构建统计票房链表
    List_ForEach(playList, pos) {
        // 初始化当前剧目的票房和售票数
        long totalSales = 0;
        sold = 0;

        // 打开销售记录文件
        FILE* fp = fopen(SALE_DATA_FILE, "rb");
        if (fp == NULL) {
            continue;
        }

        sale_t sale;
        ticket_t ticket;
        schedule_t schedule;

        //遍历所有销售记录
        while (fread(&sale, sizeof(sale_t), 1, fp) == 1) {
            //跳过退票记录
            if (sale.type == SALE_REFOUND) {
                continue;
            }

            //通过ticket_id获取票信息
            if (!Ticket_Srv_FetchByID(sale.ticket_id, &ticket)) {
                continue;
            }

            //通过schedul_id获取演出计划
            if (!Schedule_Srv_FetchByID(ticket.schedule_id, &schedule)) {
                continue;
            }

            //匹配当前剧目ID
            if (schedule.play_id == pos->data.id) {
                totalSales += sale.value;
                sold++;
            }
        }
        fclose(fp);

        //创建新的统计节点
        newNode = (salesanalysis_node_t*)malloc(sizeof(salesanalysis_node_t));
        if (newNode == NULL) {
            continue;
        }

        //填充统计数据
        newNode->data.play_id = pos->data.id;
        strcpy(newNode->data.name, pos->data.name);
        strcpy(newNode->data.area, pos->data.area);
        newNode->data.duration = pos->data.duration;
        newNode->data.price = pos->data.price;
        newNode->data.start_date.day = pos->data.start_date.day;
        newNode->data.start_date.month = pos->data.start_date.month;
        newNode->data.start_date.year = pos->data.start_date.year;
        newNode->data.end_date.day = pos->data.end_date.day;
        newNode->data.end_date.month = pos->data.end_date.month;
        newNode->data.end_date.year = pos->data.end_date.year;
        newNode->data.sales = totalSales;
        newNode->data.totaltickets = sold;

        //将新节点插入统计链表
        List_AddTail(list, newNode);
    }

    //销毁剧目链表，释放内存
    List_Destroy(playList, play_node_t);

    //返回统计链表的节点个数
    return List_Count(list);
}

//剧目票房排行
void SalesAnalysis_Srv_SortBySale(salesanalysis_list_t list) {
    salesanalysis_node_t* listLeft = NULL;
    salesanalysis_node_t* p = NULL;
    salesanalysis_node_t* curr = NULL;
    salesanalysis_node_t* prev = NULL;

    //若链表为空，直接返回
    if (list == NULL || list->next == list) {
        return;
    }

    //断开循环链表，转为普通链表
    list->prev->next = NULL;
    list->prev = list;

    //listLeft 指向第一个数据节点
    listLeft = list->next;

    //将原链表置为空（仅保留头节点）
    list->next = list;
    list->prev = list;

    //遍历所有未排序节点，逐个插入到已排序链表
    while (listLeft != NULL) {
        //取出当前待排序节点
        p = listLeft;
        listLeft = listLeft->next;

        //直接在主函数中实现插入逻辑（无辅助函数）
        prev = list;
        curr = list->next;
        //按票房降序找插入位置
        while (curr != list) {
            if (curr->data.sales < p->data.sales) {
                break;
            }
            prev = curr;
            curr = curr->next;
        }
        //插入当前节点p到prev和curr之间
        p->next = curr;
        p->prev = prev;
        prev->next = p;
        curr->prev = p;
    }
}

//根据剧目ID获取票房
int Schedule_Srv_StatRevByPlay(int play_id, int* soldCount) {
    //定义票房变量
    int value = 0;
    //定义有效售票数量临时变量
    int sold = 0;
    //定义演出计划链表与遍历指针
    schedule_list_t list;
    schedule_node_t* p;

    //初始化有效售票数量
    if (soldCount != NULL) {
        *soldCount = 0;
    }

    //初始化演出计划链表
    List_Init(list, schedule_node_t);

    //载入该剧目对应的所有演出计划
    Schedule_Perst_SelectByPlay(list, play_id);

    //遍历所有演出计划
    List_ForEach(list, p) {
        int schSold = 0;
        //统计该演出计划的票房和售票数
        int schRev = Ticket_Srv_StatRevBySchID(p->data.id, &schSold);

        //累加票房和有效售票数
        value += schRev;
        sold += schSold;
    }

    //更新输出参数：有效售票数量
    if (soldCount != NULL) {
        *soldCount = sold;
    }

    //销毁演出计划链表，释放内存
    List_Destroy(list, schedule_node_t);

    //返回剧目总票房
    return value;
}

//根据演出计划id获取票房
int Ticket_Srv_StatRevBySchID(int schedule_id, int* soldCount) {
    //定义票房变量
    int value = 0;
    //定义链表和遍历指针、销售记录变量
    ticket_list_t list;
    ticket_node_t* p;
    sale_t sale;

    //初始化票链表
    List_Init(list, ticket_node_t);

    //初始化有效售票数量
    if (soldCount != NULL) {
        *soldCount = 0;
    }

    //根据演出计划ID获取所有票
    Ticket_Srv_FetchBySchID(list, schedule_id);

    //遍历票链表
    List_ForEach(list, p) {
        //根据票ID获取销售记录
        Sale_Srv_FetchByTicketID(p->data.id, &sale);

        //判断：销售类型为「买票」且票状态为「已售」
        if (sale.type == SALE_SELL && p->data.status == TICKET_SOLD) {
            // i) 有效售票数量累加
            if (soldCount != NULL) {
                (*soldCount)++;
            }
            // j) 票房累加
            value += p->data.price;
        }
    }

    //销毁链表，释放内存
    List_Destroy(list, ticket_node_t);

    //返回总票房
    return value;
}

//根据演出计划id获取票的数据
int Ticket_Srv_FetchBySchID(ticket_list_t list, int schedule_id) {
    //定义计数变量
    int count = 0;

    //清除原始数据
    List_Free(list, ticket_node_t);

    //定义临时票链表
    ticket_list_t tickList;

    //初始化临时链表
    List_Init(tickList, ticket_node_t);

    //从文件中载入匹配的票信息
    count = Ticket_Srv_SelBySchID( schedule_id, tickList);

    //判断返回值是否小于等于0
    if (count <= 0) {
        //销毁临时链表
        List_Destroy(tickList, ticket_node_t);
        //返回0
        return 0;
    }

    List_Destroy(tickList, ticket_node_t);

    //返回结果
    return count;
}

//根据票id获取销售记录
int Sale_Srv_FetchByTicketID(int ticket_id, sale_t* sale) {
    if (sale == NULL || ticket_id <= 0) {
        return 0;
    }
    return Sale_Perst_SelByTicketID(ticket_id, sale);
}