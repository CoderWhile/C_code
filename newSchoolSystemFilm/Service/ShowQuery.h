#pragma once
// 防止头文件重复包含（双重保护：兼容不同编译器）
#ifndef SHOW_QUERY_H
#define SHOW_QUERY_H

// 引入依赖的核心头文件（演出计划基础定义）
#include "../Service/ShowPlan.h"
// 引入链表通用定义（如果List.h中有基础链表操作，需确保路径正确）
#include "../Common/List.h"
#include "Play.h"

// ===================== 前置类型声明（避免编译依赖） =====================
// 剧目数据结构声明（需与项目中play.h的实际定义一致，此处为兼容声明）
// 
#ifndef PLAY_T_DEFINED
#define PLAY_T_DEFINED
//typedef struct {
//    int id;             // 剧目唯一ID（主键）
//    char name[64];      // 剧目名称（用于查询匹配）
//    // 可扩展字段：如时长、类型、票价等，根据实际需求添加
//} play_t;
#endif

// 统一链表类型别名（解决原代码中account_list_t与schedule_list_t的命名混乱）
// 核心：查询结果存储的是演出计划，应复用ShowPlan.h中定义的schedule_list_t
typedef schedule_list_t query_schedule_list_t;

// ===================== 界面层（UI层） - 面向用户交互 =====================

void Schedule_UI_ListAll(void);


int Schedule_UI_Query(void);

// ===================== 业务逻辑层（Service层） - 核心业务处理 =====================

//int Schedule_Srv_FetchAll(query_schedule_list_t list);


//int Play_Srv_FetchByName(const char* play_name, play_t* play);


int Schedule_Srv_FetchByPlay(query_schedule_list_t list, int play_id);

// ===================== 持久化层（Persistent层） - 数据读写 =====================

//int Schedule_Perst_SelectAll(query_schedule_list_t* list);


//int Play_Perst_SelectByName(const char* play_name, play_t* play);


int Schedule_Perst_SelectByPlay(int play_id, query_schedule_list_t list);

#endif // SHOW_QUERY_H