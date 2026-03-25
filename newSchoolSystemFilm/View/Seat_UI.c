#define _CRT_SECURE_NO_WARNINGS 1

/*
* Copyright(C), 2007-2008, XUPT Univ.
* 用例编号：TTMS_UC_02
* File name: Seat_UI.c
* Description : 设置座位用例界面层
* Author:   XUPT
* Version:  v.1
* Date: 	2015年4月22日
*/

#include "Seat_UI.h"
#include "../Service/Seat.h"

#include "../Service/Studio.h"
#include "../Common/List.h"
#include <stdio.h>

static const int SEAT_PAGE_SIZE = 10;


static void Seat_Status_To_Str(seat_status_t status, char* str) {
    switch (status) {
    case SEAT_NONE:
        strcpy(str, "Unconfigured");
        break;
    case SEAT_GOOD:
        strcpy(str, "Available");
        break;
    case SEAT_BROKEN:
        strcpy(str, "Broken");
        break;
    default:
        strcpy(str, "Unknown");
    }
}
/*
表识符：TTMS_SCU_Seat_UI_S2C
函数功能：根据座位状态获取界面显示符号。
参数说明：status为seat_status_t类型，表示座位状态。
返 回 值：字符型，表示座位的界面显示符号。
*/
inline char Seat_UI_Status2Char(seat_status_t status) {
    if (status == SEAT_GOOD)
        return "#";
    else if (status == SEAT_BROKEN)
        return "~";
    else if (status == SEAT_NONE)
        return " ";
}

/*
标识符：TTMS_SCU_Seat_UI_C2S
函数功能：根据输入符号获取座位状态。
参数说明：statusChar为字符型，表示设置座位的输入符号。
返 回 值：seat_status_t类型，表示座位的状态。
*/
inline seat_status_t Seat_UI_Char2Status(char statusChar) {
    if (statusChar == "#")
        return SEAT_NONE;
    else if (statusChar == "~")
        return SEAT_BROKEN;
    else if (statusChar == "''")
        return SEAT_NONE;
}

/*
标识符：TTMS_SCU_Seat_UI_MgtEnt
函数功能：界面层管理座位的入口函数，显示当前的座位数据，并提供座位数据添加、修改、删除功能操作的入口。
参数说明：roomID为整型，是需要设置座位的演出厅ID。
返 回 值：无。
*/
void Seat_UI_MgtEntry(int roomID) {
    int row, col;
    char choice;

    // 初始化链表和分页结构
    seat_list_t head;
    seat_node_t* pos;
    Pagination_t paging;

    List_Init(head, seat_node_t);
    paging.offset = 0;
    paging.pageSize = SEAT_PAGE_SIZE;

    // 加载指定演出厅的座位数据
    paging.totalRecords = Seat_Srv_FetchByRoomID(head, roomID);
    Paging_Locate_FirstPage(head, paging);

    do {
        system("cls"); // Windows清屏
        printf("\n==================================================================\n");
        printf("********************** Seat Management (Room:%d) **********************\n", roomID);
        printf("%8s  %8s  %8s  %8s  %12s\n", "SeatID", "RoomID", "Row", "Column", "Status");
        printf("------------------------------------------------------------------\n");

        // 分页显示座位数据
        pos = head->next;
        // 跳过当前页之前的记录
        for (int i = 0; pos != head && i < paging.offset; i++) {
            pos = pos->next;
        }
        // 显示当前页数据
        int showCnt = 0;
        while (pos != head && showCnt < paging.pageSize) {
            char statusStr[20];
            Seat_Status_To_Str(pos->data.status, statusStr);
            printf("%8d  %8d  %8d  %8d  %12s\n",
                pos->data.id, pos->data.roomID, pos->data.row,
                pos->data.column, statusStr);
            pos = pos->next;
            showCnt++;
        }

        // 计算分页信息
        int totalPages = (paging.totalRecords % paging.pageSize == 0) ? (paging.totalRecords / paging.pageSize) : (paging.totalRecords / paging.pageSize + 1);
        int curPage = (paging.offset / paging.pageSize) + 1;

        // 打印分页和操作提示
        printf("------- Total Seats:%2d ----------------------- Page %2d/%2d ----\n",
            paging.totalRecords, curPage, totalPages);
        printf("******************************************************************\n");
        printf("[A]dd Seat  [M]odify Seat  [D]elete Seat  [P]rev Page  [N]ext Page  [R]eturn\n");
        printf("==================================================================\n");
        printf("Please input your choice: ");
        while (getchar() != '\n');

        scanf(" %c", &choice);

        // 处理用户操作
        switch (choice) {
            // 添加座位
        case 'a':
        case 'A':
            printf("\nInput Seat Row: ");
            scanf("%d", &row);
            printf("Input Seat Column: ");
            scanf("%d", &col);
            Seat_UI_Add(head, roomID, row, col);
            // 刷新数据并定位到最后一页
            paging.totalRecords = Seat_Srv_FetchByRoomID(head, roomID);
            totalPages = (paging.totalRecords % paging.pageSize == 0) ? (paging.totalRecords / paging.pageSize) : (paging.totalRecords / paging.pageSize + 1);
            paging.offset = (totalPages - 1) * paging.pageSize;
            printf("\nPress Enter to continue...");
            while (getchar() != '\n');
            break;

            // 修改座位
        case 'm':
        case 'M':
            printf("\nInput Seat Row to Modify: ");
            scanf("%d", &row);
            printf("Input Seat Column to Modify: ");
            scanf("%d", &col);
            Seat_UI_Modify(head, row, col);
            paging.totalRecords = Seat_Srv_FetchByRoomID(head, roomID);
            printf("\nPress Enter to continue...");
            while (getchar() != '\n');
            break;

            // 删除座位
        case 'd':
        case 'D':
            printf("\nInput Seat Row to Delete: ");
            scanf("%d", &row);
            printf("Input Seat Column to Delete: ");
            scanf("%d", &col);
            Seat_UI_Delete(head, row, col);
            // 刷新数据并调整分页,避免越界
            paging.totalRecords = Seat_Srv_FetchByRoomID(head, roomID);
            totalPages = (paging.totalRecords % paging.pageSize == 0) ? (paging.totalRecords / paging.pageSize) : (paging.totalRecords / paging.pageSize + 1);
            if (curPage > totalPages && totalPages > 0) {
                paging.offset = (totalPages - 1) * paging.pageSize;
            }
            else if (totalPages == 0) {
                paging.offset = 0;
            }
            printf("\nPress Enter to continue...");
            while (getchar() != '\n');
            break;

            // 上一页
        case 'p':
        case 'P':
            if (curPage > 1) {
                paging.offset -= paging.pageSize;
            }
            else {
                printf("\nAlready at the first page!");
                while (getchar() != '\n');
            }
            break;

            // 下一页
        case 'n':
        case 'N':
            if (curPage < totalPages) {
                paging.offset += paging.pageSize;
            }
            else {
                printf("\nAlready at the last page!");
                while (getchar() != '\n');
            }
            break;

            // 返回（退出）
        case 'r':
        case 'R':
            printf("\nExit Seat Management...");
            break;

            // 无效选择
        default:
            printf("\nInvalid choice! Please try again.");
            while (getchar() != '\n');
            break;
        }
    } while (choice != 'r' && choice != 'R');

    // 释放链表内存，避免内存泄漏
    List_Destroy(head, seat_node_t);
}

/*
识符：TTMS_SCU_Seat_UI_Add
函数功能：用于添加一个新的座位数据。
参数说明：第一个参数list为seat_list_t类型指针，指向座位链表头指针，
         第二个参数rowsCount为整型，表示座位所在行号，第三个参数colsCount为整型，表示座位所在列号。
返 回 值：整型，表示是否成功添加了座位的标志。
*/
int Seat_UI_Add(seat_list_t list, int roomID, int row, int column) {  //输入一个座位
    if (list == NULL) {
        return 0;
    }
    if (roomID <= 0 || row <= 0 || column <= 0) {
        return 0;
    }

    //检查指定行列的座位是否已存在
    seat_node_t* existNode = Seat_Srv_FindByRowCol(list, row, column);
    if (existNode != NULL) {
        printf("Error : This seat already exists and cannot be added again!");
        return 0;
    }

    //输入座位状态
    seat_t newSeat = { 0 };  // 初始化座位结构体
    int inputStatus;
    printf("\n=======================================================\n");
    printf("****************  Add New Seat  ****************\n");
    printf("-------------------------------------------------------\n");
    printf("Seat Status (0-Unconfigured,1-Available,9-Broken): ");
    scanf("%d", &inputStatus);
    while (getchar() != '\n');

    //检测座位状态
    if (inputStatus != SEAT_NONE && inputStatus != SEAT_GOOD && inputStatus != SEAT_BROKEN) {
        return 0;
    }

    newSeat.roomID = roomID;  //演出厅ID
    newSeat.row = row;  //行
    newSeat.column = column;  //列
    newSeat.status = (seat_status_t)inputStatus;  //座位状态
    seat_node_t* newNode = (seat_node_t*)malloc(sizeof(seat_node_t));
    newNode->data = newSeat;
    if (Seat_Srv_Add(&newSeat)) {
        //新增成功：同步到链表
        Seat_Srv_AddToSoftedList(list, newNode);
        printf("Success: Seat added\n");
        return 1;
    }
    else {
        printf("Failed: Add seat failed!\n");
        return 0;
    }
}

/*
标识符：TTMS_SCU_Seat_UI_Mod
函数功能：用于修改一个座位数据。
参数说明：第一个参数list为seat_list_t类型指针，指向座位链表头指针，第二个参数rowsCount为整型，表示座位所在行号，第三个参数colsCount为整型，表示座位所在列号。
返 回 值：整型，表示是否成功修改了座位的标志。
*/
int Seat_UI_Modify(seat_list_t list, int row, int column) {
    if (list == NULL || row <= 0 || column <= 0) {
        return 0;
    }

    //查找目标座位
    seat_node_t* targetSeat = Seat_Srv_FindByRowCol(list, row, column);
    if (targetSeat == NULL) {
        printf("Error: Seat not found!\n");
        return 0;
    }

    char curStatus[20];
    Seat_Status_To_Str(targetSeat->data.status, curStatus);
    printf("\n=======================================================\n");
    printf("****************  Modify Seat  ****************\n");
    printf("-------------------------------------------------------\n");
    printf("Current Seat Info:\n");
    printf("ID: %d, RoomID: %d, Row:%d, Column:%d\n", targetSeat->data.id, targetSeat->data.roomID, row, column);
    printf("Current Status: %s\n", curStatus);

    int newStatus;
    printf("New Status (0-Unconfigured,1-Available,9-Broken): ");
    scanf("%d", &newStatus);
    while (getchar() != '\n');

    // 检查状态
    if (newStatus != SEAT_NONE && newStatus != SEAT_GOOD && newStatus != SEAT_BROKEN) {
        return 0;
    }

    // 状态未变化 直接返回
    if (newStatus == targetSeat->data.status) {
        printf("Seat status not changed!\n");
        return 1;
    }

    //修改状态
    seat_t modSeat = targetSeat->data;
    modSeat.status = (seat_status_t)newStatus;
    if (Seat_Srv_Modify(&modSeat)) {
        targetSeat->data.status = (seat_status_t)newStatus; // 同步链表
        char newStatusStr[20];
        Seat_Status_To_Str((seat_status_t)newStatus, newStatusStr);
        printf("Success: Seat status updated to %s!\n", newStatusStr);
        return 1;
    }
    else {
        printf("Failed: Modify seat failed!\n");
        return 0;
    }

}

/*
标识符：TTMS_SCU_Seat_UI_Del
函数功能：用于删除一个座位的数据。
参数说明：第一个参数list为seat_list_t类型指针，指向座位链表头指针，第二个参数rowsCount为整型，表示座位所在行号，第三个参数colsCount为整型，表示座位所在列号。
返 回 值：整型，表示是否成功删除了座位的标志。
*/
int Seat_UI_Delete(seat_list_t list, int row, int column) {
    if (list == NULL || row <= 0 || column <= 0) {
        return 0;
    }

    //查找座位
    seat_node_t* targetSeat = Seat_Srv_FindByRowCol(list, row, column);
    if (targetSeat == NULL) {
        printf("Error: Seat not found!\n");
        return 0;
    }

    //二次确认删除
    char confirm;
    printf("\n=======================================================\n");
    printf("****************  Delete Seat  ****************\n");
    printf("-------------------------------------------------------\n");
    printf("Confirm to delete this seat? (Y/N):\n");
    printf("Seat ID: %d, RoomID: %d, Row:%d, Column:%d\n", targetSeat->data.id, targetSeat->data.roomID, row, column);
    printf("Your choice (Y/N): ");
    while (getchar() != '\n');

    scanf("%c", &confirm);
    if (confirm != 'y' && confirm != 'Y') {
        printf("Hint: Delete operation canceled!\n");
        return 0;
    }

    //调用业务层删除接口（按ID删除，保证数据一致性）
    if (Seat_Srv_DeleteByID(targetSeat->data.id)) {
        //删除成功
        printf("Success: Seat deleted!\n");
        return 1;
    }
    else {
        //删除失败
        printf("Failed: Delete seat failed !\n");
        return 0;
    }

}
