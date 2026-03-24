#pragma once
/*
* 用例编号：TTMS_UC_11
* File name: SalesAnalysis.h
* Description : 统计票房逻辑层头文件
*/

#include "../Common/common.h"
#include "Ticket.h"
#include "Sale.h"

typedef struct {
	int play_id;    // 剧目编号
	char name[31];  // 剧目名称
	char area[9];    // 剧目区域
	int duration;   // 剧目播放时长
	long totaltickets;  // 剧目上座数量
	long sales;          // 剧目票房（销售额统计
	int price;         // 剧目票价
	user_date_t start_date;   // 剧目上映日期
	user_date_t end_date;     // 剧目下映日期
}salesanalysis_t;


typedef struct salesanalysis_node {
	salesanalysis_t data;
	struct salesanalysis_node* next;
	struct salesanalysis_node* prev;
}salesanalysis_node_t, * salesanalysis_list_t;

//获取剧目票房
int SalesAnalysis_Srv_StaticSale(salesanalysis_list_t list);

//剧目票房排行
void SalesAnalysis_Srv_SortBySale(salesanalysis_list_t list);

//根据剧目ID获取票房
int Schedule_Srv_StatRevByPlay(int play_id, int* soldCount);

//根据演出计划id获取票房
int Ticket_Srv_StatRevBySchID(int schedule_id, int* soldCount);

//根据演出计划id获取票的数据
int Ticket_Srv_FetchBySchID(ticket_list_t list, int schedule_id);

//根据票id获取销售记录
int Sale_Srv_FetchByTicketID(int ticket_id, sale_t* sale);
