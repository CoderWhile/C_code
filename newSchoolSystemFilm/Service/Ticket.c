#define _CRT_SECURE_NO_WARNINGS 1
#include"Ticket.h"
#include"Schedule.h"
#include "../View/Seat_UI.h"
#include"../Service/Seat.h"
#include "../Persistence/Ticket_persist.h"
#include <stdio.h>

//  1. 根据ID获取演出计划 
int Schedule_Srv_FetchByID(int id, schedule_t* buf) {//buf输出参数，存储查到的演出数据
    // 调用持久化层函数获取演出计划
    int rtn = Schedule_Perst_SelectByID(id, buf);

    return rtn;//返回
}

//  2. 批量生成演出票 
void Ticket_Srv_GenBatch(int schedule_id, int stuID) {

    seat_list_t seat_head = NULL;//初始化链表，存储有效座位的
    int count = 0;

    // 获取有效座位链表
    if (Seat_Srv_FetchValidByRoomID(&seat_head, stuID) != 1) {
        printf("获取有效座位失败！\n");
        return -1;
    }

    // 统计有效座位数（可选，用于验证）
    seat_list_t p = seat_head;
    while (p != NULL) {
        count++;
        p = p->next;
    }

    // 步调用持久化层批量插入票务
    int rtn = Ticket_Perst_Insert(schedule_id, seat_head);

    // 返回生成的票数
    return rtn;
}

// 3. 批量删除演出票
int Ticket_Srv_DeleteBatch(int schedule_id) {
    // 调用持久化层删除票务
    int found = Ticket_Perst_Rem(schedule_id);
    // 返回删除的票数
    return found;
}


// --------------------------
 // 4. 根据ID获取剧目
 // --------------------------

// --------------------------
// 5. 根据演出厅ID获取有效座位
// --------------------------
//int Seat_Srv_FetchValidByRoomID(int room_id, seat_list_t list) {
    // 调用座位持久化层获取有效座位
    //return Seat_Perst_FetchValidByRoomID(room_id, list);
//}

// --------------------------
// 根据ID获取演出票
// 对应教材：TTMS_SCU_Ticket_Srv_FetchByID
// --------------------------
int Ticket_Srv_FetchByID(int id, ticket_t* buf) {
    // 步骤a：将参数 id 和 buf 作为实参，调用持久化层查询函数 Ticket_Perst_SelByID
    int rtn = Ticket_Perst_SelByID(id, buf);

    // 步骤b：返回持久化层函数的返回值
    return rtn;
}