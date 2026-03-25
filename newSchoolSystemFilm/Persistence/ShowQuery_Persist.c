#define _CRT_SECURE_NO_WARNINGS

#include "../Service/ShowQuery.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// //修改参数为指针的指针，让外部能拿到链表头
//int Schedule_Perst_SelectAll(query_schedule_list_t* list) {
//    int recCount = 0;
//    FILE* fp = NULL;
//    schedule_t schedule;
//    query_schedule_list_t newNode = NULL;
//    query_schedule_list_t tail = NULL;
//
//    // 步骤a：检查文件是否存在
//    fp = fopen("Schedule.dat", "rb");
//    if (fp == NULL) {
//        return 0; // 文件不存在，返回0
//    }
//
//    // 步骤b：初始化链表为空（修改外部指针）
//    *list = NULL;
//
//    // 步骤c：循环读取文件直到末尾
//    while (fread(&schedule, sizeof(schedule_t), 1, fp) == 1) {
//        // 步骤d：构造新节点
//        newNode = (query_schedule_list_t)malloc(sizeof(struct schedule_node));
//        if (newNode == NULL) {
//            fclose(fp);
//            return recCount; // 内存分配失败，返回已读取数量
//        }
//        // 复制演出计划数据到节点
//        memcpy(&newNode->data, &schedule, sizeof(schedule_t));
//        newNode->next = NULL;
//
//        // 添加到链表尾部
//        if (*list == NULL) {
//            *list = newNode;  // 链表为空，新节点为头节点
//            tail = newNode;
//        } else {
//            tail->next = newNode; // 链表不为空，追加到尾部
//            tail = newNode;
//        }
//
//        recCount++; // 记录数+1
//    }
//
//    // 步骤e：关闭文件，返回记录数量
//    fclose(fp);
//    return recCount;
//}
// //根据名称载入剧目（与头文件类型匹配）
//int Play_Perst_SelectByName(const char* condt, play_t* play) {
//    int recCount = 0;
//    FILE* fp = NULL;
//    play_t data;
//    // 注意：这里使用头文件中的play_t类型，不再使用play_list_t链表
//
//    // a) 检查文件是否存在
//    fp = fopen("Play.dat", "rb");
//    if (fp == NULL) {
//        return 0;
//    }
//
//    // b) 读取文件并匹配关键词
//    while (fread(&data, sizeof(play_t), 1, fp) == 1) {
//        // d) 判断剧目名称是否包含关键词
//        if (strstr(data.name, condt) != NULL) {
//            // 将匹配的剧目信息存入输出参数
//            memcpy(play, &data, sizeof(play_t));
//            recCount++;
//            // 若只需要第一个匹配结果，可直接break
//            // break;
//        }
//    }
//
//    // e) 关闭文件并返回结果
//    fclose(fp);
//    return recCount;
//}