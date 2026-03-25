#define _CRT_SECURE_NO_WARNINGS 1
/*
 * Copyright (C), 2007-2008, XUPT Univ.
 * File name: Ticket_Persist.c
 * Description : 演出票持久化层实现
 * Author:  李睿熙
 * Version:  v1.0
 * Date:  2026-03-18
 */
#include <stdio.h>
#include <stdlib.h>
#include"../Service/Seat.h"
#include"../Service/Ticket.h"
#include"../Service/play.h"
#include"Schedule.h"
#include <string.h>
#include"../Common/List.h"
#include"../Persistence/EntityKey_Persist.h"
#include"../Persistence/Play_Persist.h"

static const char TICKET_DATA_FILE[] = "Ticket.dat"; //演出厅文件名常量 
static const char TICKET_DATA_TEMP_FILE[] = "TicketTmp.dat"; //演出厅临时文件名常量 
#define SCHEDULE_FILE "Schedule.dat"
#define TICKET_FILE "Ticket.dat"
#define TMP_TICKET_FILE "TicketTmp.dat"
#define PLAY_FILE "Play.dat"
static const char TICKET_KEY_NAME[] = "Ticket"; //演出厅名常量 


 // --------------------------
 // 1. 根据ID载入演出计划
 // --------------------------
int Schedule_Perst_SelectByID(int id, schedule_t* buf) {//要查的场次id，buf用来存找到的数据
    int found = 0;//0是没找到，1是找到了
    FILE* fp = fopen(SCHEDULE_FILE, "rb");
    if (!fp) return 0;

    schedule_t data;//定义临时变量，存从文件读出的场次数据
    while (fread(&data, sizeof(schedule_t), 1, fp) == 1) {//读成功返1，失败0循环结束
        if (data.id == id) {
            *buf = data;
            found = 1;//已找到
            break;
        }
    }
    fclose(fp);
    return found;//返回1或者0
}

// --------------------------
// 2. 存储演出票（批量插入）
int Ticket_Perst_Insert(int schedule_id, seat_list_t list) {//根据场次id,座位链表，批量生成票写入文件
    int rtn = 0;//临变，存fwrite的返回值
    FILE* fp = fopen(TICKET_FILE, "ab");
    if (!fp) return 0;

    // 获取演出计划信息
    schedule_t sch;//定义变量存演出计划
    if (!Schedule_Perst_SelectByID(schedule_id, &sch)) {//根据schedule_id找场次
        fclose(fp);
        return -1;//找不到返回1
    }

    //  获取剧目信息
    play_t play;//定义剧目变量
    if (!Play_Perst_SelectByID(sch.play_id, &play)) {//根据剧目id，查找对应剧目
        fclose(fp);
        return -1;
    }

    // 统计座位数并获取主键，要生成多少张票
    int count = 0;
    seat_node_t* p = list->next;//让指针p指向链表第一个有效座位
    while (p != list) {//循环遍历整个座位链表
        count++;//每遍历一个座位，+1
        p = p->next;//指针后移
    }
    if (count == 0) {
        printf("座位链表为空，无需插入票数据！\n");
        fclose(fp);
        return 0;
    }
    long key = EntKey_Perst_GetNewKeys(TICKET_KEY_NAME, count); //

   

    //  遍历座位链表，构造并写入票数据
    p = list->next;//重新回到链表头
    ticket_t ticket_data;
    int key_idx = 0;//主键数组下标，从0开始
    while (p != list) {
        ticket_t data;
        long key = EntKey_Perst_GetNewKeys(TICKET_KEY_NAME, count); //

        data.id = key;//给票赋值id，下标后移
        data.schedule_id = schedule_id;//绑定场次id
        data.seat_id = p->data.id;//绑定座位id
        // 
        data.price = play.price;//票价来自场次价格
        data.status = TICKET_AVL; // 初始状态为待售

        rtn = fwrite(&data, sizeof(ticket_t), 1, fp);//把这张票写入文件
        p = p->next;//下一个座位
    }

    //free(keys);
    // 关闭文件并返回
    fclose(fp);
    return rtn >= 0 ? count : -1;
}
// --------------------------
// 3. 根据演出计划ID去除演出票（批量删除）
// --------------------------
int Ticket_Perst_Rem(int schedule_id) {
    int found = 0;
    // 重命名原文件
    if (rename(TICKET_FILE, TMP_TICKET_FILE) != 0) {//把原票文件改名为临时文件
        return -1;
    }

    FILE* rfp = fopen(TMP_TICKET_FILE, "rb");//读原
    FILE* wfp = fopen(TICKET_FILE, "wb");//写新
    if (!rfp || !wfp) {
        if (rfp) fclose(rfp);
        if (wfp) fclose(wfp);
        rename(TMP_TICKET_FILE, TICKET_FILE);//该回去，恢复数据
        return -1;
    }

    ticket_t buf;//临时存1每张票
    while (fread(&buf, sizeof(ticket_t), 1, rfp) == 1) {//循环读每一张
        if (buf.schedule_id == schedule_id) {//如果是要删除的场次
            found++; // 统计删除数量
        }
        else {
            fwrite(&buf, sizeof(ticket_t), 1, wfp);//写回新文件
        }
    }

    fclose(rfp);
    fclose(wfp);
    remove(TMP_TICKET_FILE); // 删除临时文件
    return found;
}

// --------------------------
// 4. 根据ID载入剧目
// --------------------------




// 根据ID载入演出票
int Ticket_Perst_SelByID(int id, ticket_t* buf) {
    // 定义局部变量found，赋初值0
    int found = 0;
    ticket_t data;

    // 以只读方式打开 Ticket.dat 文件
    FILE* fp = fopen("Ticket.dat", "rb");
    if (!fp) {
        // 打开失败
        return found;
    }

    // 循环判断是否到文件尾
    while (!feof(fp)) {
        // 从文件读出当前记录到局部变量data
        if (fread(&data, sizeof(ticket_t), 1, fp) != 1) {
            // 读操作失败，跳出循环
            break;
        }

        // 判断data.id是否等于参数id
        if (data.id == id) {
            found = 1;
            *buf = data;  // 将data的值赋给*buf
            break;        // 跳出循环
        }
    }

    // 关闭文件
    fclose(fp);

    // 返回found
    return found;
}