#define _CRT_SECURE_NO_WARNINGS 1

/*
* Copyright(C), 2007-2008, XUPT Univ.	
* 用例编号：TTMS_UC_02 
* File name: Seat.c		  
* Description : 设置座位用例业务逻辑层	
* Author:   XUPT  		 
* Version:  v.1 	 
* Date: 	2015年4月22日	
*/

#include <stdlib.h>
#include "../Common/List.h"
#include "Seat.h"
#include"../Persistence/Seat_Persist.h"

/*
函数功能：用于添加一个新座位数据。
参数说明：data为seat_t类型指针，表示需要添加的座位数据结点。
返 回 值：整型，表示是否成功添加了座位的标志。
*/
int Seat_Srv_Add(const seat_t *data){
    return Seat_Perst_Insert(data);
}

/*
函数功能：批量添加座位数据。
参数说明：list为seat_list_t类型指针，表示需要添加的批量座位数据形成的链表。
返 回 值：整型，表示是否成功添加了批量座位的标志。
*/
int Seat_Srv_AddBatch(seat_list_t list){
	// 请补充完整
    return Seat_Perst_InsertBatch(list);

}

/*
函数功能：用于修改一个座位数据。
参数说明：data为seat_t类型指针，表示需要修改的座位数据结点。
返 回 值：整型，表示是否成功修改了座位的标志。
*/
int Seat_Srv_Modify(const seat_t *data){
	// 请补充完整
    return Seat_Perst_Update(data);

}

/*
函数功能：根据座位ID删除一个座位。
参数说明：ID为整型，表示需要删除的座位数据结点。
返 回 值：整型，表示是否成功删除了座位的标志。
*/
int Seat_Srv_DeleteByID(int ID){
	// 请补充完整
    return Seat_Perst_DeleteByID(ID);

}

/*
函数功能：根据座位ID获取座位数据。
参数说明：第一个参数ID为整型，表示座位ID，第二个参数buf为seat_t指针，指向待获取的座位数据结点。
返 回 值：整型，表示是否成功获取了座位的标志。
*/
int Seat_Srv_FetchByID(int ID, seat_t *buf){
	// 请补充完整
    return Seat_Perst_SelectByID(ID, buf);

}

/*
函数功能：根据演出厅ID删除所有座位。
参数说明：roomID为整型，表示需要删除所有座位的演出厅ID。
返 回 值：整型，表示是否成功删除了演出厅所有座位的标志。
*/
/*inline*/ int Seat_Srv_DeleteAllByRoomID(int roomID) {
	// 请补充完整
    return Seat_Perst_DeleteAllByRoomID(roomID);

}

/*
函数功能：根据给定演出厅的行、列数初始化演出厅的所有座位数据，并将每个座位结点按行插入座位链表。
参数说明：第一个参数list为seat_list_t类型指针，指向座位链表头指针，第二个参数rowsCount为整型，表示座位所在行号，第三个参数colsCount为整型，表示座位所在列号。
返 回 值：整型，表示是否成功初始化了演出厅的所有座位。
*/
int Seat_Srv_FetchByRoomID(seat_list_t list, int roomID){
       // 请补充完整
    return Seat_Perst_SelectByRoomID(list, roomID);

}

/*
函数功能：根据演出厅ID获得该演出厅的有效座位。
参数说明：第一个参数list为seat_list_t类型，表示获取到的有效座位链表头指针，第二个参数roomID为整型，表示需要提取有效座位的演出厅ID。
返 回 值：整型，表示演出厅的有效座位个数。
*/
int Seat_Srv_FetchValidByRoomID(seat_list_t list, int roomID)
{
    if (list == NULL || roomID <= 0)
        return 0;

    int total = Seat_Srv_FetchByRoomID(list, roomID);
    if (total <= 0)
        return 0;

    int validCount = 0;
    seat_node_t* cur = list->next;
    seat_node_t* prev = list;

    while (cur != NULL) {
        seat_node_t* next = cur->next;
        if (cur->data.status == SEAT_GOOD) {
            validCount++;
            prev = cur;
        }
        else {
            // 移除无效节点
            prev->next = next;
            if (next != NULL) {
                next->prev = prev;
            }
            free(cur);
        }
        cur = next;
    }

    return validCount;
}

/*
函数功能：根据给定演出厅的行、列数初始化演出厅的所有座位数据，并将每个座位结点按行插入座位链表。
参数说明：第一个参数list为seat_list_t类型指针，指向座位链表头指针，第二个参数rowsCount为整型，表示座位所在行号，第三个参数colsCount为整型，表示座位所在列号。
返 回 值：整型，表示是否成功初始化了演出厅的所有座位。
*/
int Seat_Srv_RoomInit(seat_list_t list, int roomID, int rowsCount,
		int colsCount) {
    if (list == NULL || roomID <= 0 || rowsCount <= 0 || colsCount <= 0)
        return 0;

    // 先删除该演出厅旧数据
    Seat_Srv_DeleteAllByRoomID(roomID);

    int seatID = roomID * 10000;
    int count = 0;
    seat_node_t* cur = list;

    for (int i = 1; i <= rowsCount; i++) {
        for (int j = 1; j <= colsCount; j++) {
            seat_node_t* newNode = (seat_node_t*)malloc(sizeof(seat_node_t));
            if (newNode == NULL) continue;

            newNode->data.id = ++seatID;
            newNode->data.roomID = roomID;
            newNode->data.row = i;
            newNode->data.column = j;
            newNode->data.status = SEAT_GOOD;
            newNode->prev = cur;
            newNode->next = NULL;

            cur->next = newNode;
            cur = newNode;
            count++;
        }
    }
    return 0;
}

/*
函数功能：对座位链表list按座位行号、列号进行排序。
参数说明：list为seat_list_t类型，表示待排序座位链表头指针。
返 回 值：无。
*/
void Seat_Srv_SortSeatList(seat_list_t list) {
    // a) 链表为空直接返回
    if (list == NULL || list->next == list) {
        return;
    }

    // b) 将双向循环链表从最后一个结点断开
    list->prev->next = NULL;

    // c) listLeft指向第一个数据结点
    seat_node_t* listLeft = list->next;

    // d) 将原链表置空（头结点自环）
    list->next = list;
    list->prev = list;

    // e~h) 逐个取出结点，插入到已排序链表
    seat_node_t* p = NULL;
    while (listLeft != NULL) {
        // f) 取出第一个结点
        p = listLeft;
        // g) 移动listLeft到下一个结点
        listLeft = listLeft->next;
        // h) 将p插入到已排序链表list中
        Seat_Srv_AddToSoftedList(list, p);
    }
}

/*
函数功能：将一个座位结点加入到已排序的座位链表中。
参数说明：第一个参数list为seat_list_t类型，表示待插入结点的座位链表头指针，第二个参数node为seat_node_t指针，表示需要插入的座位数据结点。
返 回 值：无。
*/
void Seat_Srv_AddToSoftedList(seat_list_t list, seat_node_t *node) {
    // a) 链表为空（只有头结点），尾插法插入
    if (list->next == list) {
        node->next = list;
        node->prev = list;
        list->next = node;
        list->prev = node;
        return;
    }

    // b) 遍历找到插入位置
    seat_node_t* p = list->next;
    while (p != list &&
        (p->data.row < node->data.row ||
            (p->data.row == node->data.row && p->data.column < node->data.column))) {
        p = p->next;
    }

    // 将node插入到p的前面
    node->next = p;
    node->prev = p->prev;
    p->prev->next = node;
    p->prev = node;
}

/*
函数功能：根据座位的行、列号获取座位数据。
参数说明：第一个参数list为seat_list_t类型，表示座位链表头指针，
         第二个参数row为整型，表示待获取座位的行号，第三个参数column为整型，表示待获取座位的列号。
返 回 值：为seat_node_t指针，表示获取到的座位数据。
*/
seat_node_t * Seat_Srv_FindByRowCol(seat_list_t list, int row, int column) {
    // 请补充完整
    seat_node_t* cur;
    cur = list->next;
    while (cur != list)
    {
        if (cur->data.row == row && cur->data.column == column)
            return cur;
        cur = cur->next;
    }
    return NULL;
}

/*
函数功能：根据座位ID在链表中获取座位数据。
参数说明：第一个参数list为seat_list_t类型，指向座位数据链表，第二个参数ID为整型，表示座位ID。
返 回 值：seat_node_t类型，表示获取的座位数据。
*/
seat_node_t * Seat_Srv_FindByID(seat_list_t list, int rowID) {
       // 请补充完整
    if (list == NULL || rowID <= 0) return NULL;

    seat_node_t* cur = list;
    while (cur != NULL) {
        if (cur->data.id == rowID) {
            return cur;
        }
        cur = cur->next;
    }

    return NULL;
}
