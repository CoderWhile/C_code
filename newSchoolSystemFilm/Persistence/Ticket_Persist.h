#pragma once
#include"Ticket.h"
#include"../Service/Seat.h"
#include"../Service/ShowPlan.h"

// 1. 根据ID载入演出计划
int Schedule_Perst_SelectByID(int id, schedule_t* buf);

// 2. 存储演出票（批量插入）
int Ticket_Perst_Insert(int schedule_id, seat_list_t list);

// 3. 根据演出计划ID去除演出票（批量删除）
int Ticket_Perst_Rem(int schedule_id);

// 4. 根据ID载入剧目
//int Play_Perst_SelectByID(int id, play_t* buf);

// 5. 获取实体主键（批量生成票ID）
//int EntKey_Perst_GetNewKeys(int count, int* key_list);

// 根据ID载入演出票
int Ticket_Perst_SelByID(int id, ticket_t* buf);
