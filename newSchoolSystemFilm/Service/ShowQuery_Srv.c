#include "ShowPlan.h"// 假设包含schedule_list_t和相关函数声明
#include"ShowQuery.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//int Schedule_Srv_FetchAll(query_schedule_list_t list) {
//    // 调用持久化层函数获取所有演出计划
//    int rtn = Schedule_Perst_SelectAll(&list);
//
//    // 初始化剧目信息链表和演出计划信息链表（如果需要后续处理）
//    schedule_list_t list_p = NULL;
//    schedule_list_t list_s = NULL;
//
//    // 返回获取到的演出计划数量
//    return rtn;
//}

// 业务层函数定义（在 .c 文件中）

//// 假设play_list_t和持久化层函数已定义
//typedef struct play_list_node {
//    // 假设的剧目信息结构体
//    char name[50];
//    struct play_list_node* next;
//} play_list_t;



// 目标函数实现
//int Play_Srv_FetchByName(const char*play_name,play_t* play) {
//    // 调用持久化层函数，获取匹配的剧目数量
//    int rtn = Play_Perst_SelectByName(play_name,play);
//    // 返回结果数量
//    return rtn;
//}
//schedule_list_t  Schedule_Srv_FetchByPlay(query_schedule_list_t list, int play_id) {
//    // 调用持久化层函数获取所有演出计划
//   return  Schedule_Perst_SelectByPlay(list, play_id );
//   }