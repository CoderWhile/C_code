#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <stdlib.h>
#include "ticket_ui.h"
#include"../Service/play.h"
#include"Schedule.h"
#include"../Service/Seat.h"
#include"../Service/Ticket.h"
#include"../Service/ShowPlan.h"
#include"../common/List.h"

void Ticket_UI_MgtEntry(int schedule_id) {
    schedule_t sch;
    play_t play;
    int choice;

    // 调用业务逻辑层获取演出计划信息
    if (!Schedule_Srv_FetchByID(schedule_id, &sch)) {
        printf("Failed to obtain the performance schedule!\n");
        return;
    }

    // 用剧目ID获取剧目信息
    if (!Play_Srv_FetchByID(sch.play_id, &play)) {
        printf("Failed to obtain the play information!\n");
        return;
    }

    // 在界面显示剧目名称、演出厅编号、演出日期、演出时间
    printf("===============================================\n");
    printf("        Details of the performance plan        \n");
    printf("===============================================\n");
    printf("PlayID:%d\n", play.id);
    printf("PlayName:%5s\n", play.name);

    printf("RoomID:%5d\n", sch.studio_id);
    printf("Release date:%4d-%2d-%2d\n", sch.date.year, sch.date.month, sch.date.day);
    printf("Show time:%2d:%2d:%2d\n", sch.time.hour, sch.time.minute, sch.time.second);
    printf("=========================================\n");
    printf("1. Generate the tickets\n");
    printf("2. Regenerate the ticket\n");
    printf("0. return\n");
    printf("Please enter your selection:\n");
    scanf_s("%d", &choice);

    // 处理用户输入
    switch (choice) {
    case 1:
        // 选择“生成演出票”：调用业务逻辑层生成票
        Ticket_Srv_GenBatch(schedule_id, sch.studio_id);
       
        break;
    case 2:
        // 选择“重新生成票”：先删除旧票，再生成新票
        if (Ticket_Srv_DeleteBatch(schedule_id) >= 0) {
            Ticket_Srv_GenBatch(schedule_id, sch.studio_id);
        }
        else {
            printf("Failed to delete the old ticket, unable to regenerate!\n");
        }
        break;
    case 0:
        // e退出函数
        printf("Exit the performance ticket management interface\n");
        return;
    default:
        printf("Input error, please select again!\n");
    }

    // 执行完后跳转到步骤e（退出）
}

// --------------------------
// (1) 查询演出票入口界面
// TTMS_SCU_Ticket_UI_Qry
// --------------------------
void Ticket_UI_Query(void) {
    int ticket_id;
    ticket_list_t head=NULL;
    List_Init(head, ticket_node_t);     // 重新初始化
    if (head == NULL || head->next == head) {
        printf("无票务记录！\n");
        return;
    }
    Play_Srv_FetchAll(head);
    printf("=========================================\n");
    printf("        查询演出票\n");
    printf("=========================================\n");
    printf("请输入要查询的演出票ID：\n");
    scanf_s("%d", &ticket_id);
    // 调用显示函数展示查询结果
    Ticket_UI_ShowTicket(ticket_id);
}

// --------------------------
// (2) 显示演出票界面
// TTMS_SCU_Ticket_UI_ShowTicket
// --------------------------
int Ticket_UI_ShowTicket(int ticket_id) {
    ticket_t buf;
    int rtn = 0;

    // 步骤a。根据输入的票ID调用业务逻辑层查询函数
    rtn = Ticket_Srv_FetchByID(ticket_id, &buf);

    // 步骤b。根据返回值提示结果
    if (rtn == 1) {
        printf("=========================================\n");
        printf("        演出票信息\n");
        printf("=========================================\n");
        printf("票ID：%d\n", buf.id);
        printf("演出计划ID：%d\n", buf.schedule_id);
        printf("座位ID：%d\n", buf.seat_id);
        printf("票价：%d\n", buf.price);
        printf("状态：");
        switch (buf.status) {
        case TICKET_AVL: printf("待售\n"); break;
        case TICKET_SOLD: printf("已售\n"); break;
        case TICKET_RESV: printf("预留\n"); break;
        default: printf("未知\n"); break;
        }
        printf("=========================================\n");
    }
    else {
        printf("未找到ID为 %d 的演出票！\n", ticket_id);
    }

    // 步骤c。返回 rtn 的值
    return rtn;
}