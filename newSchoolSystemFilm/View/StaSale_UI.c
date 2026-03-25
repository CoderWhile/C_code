#define _CRT_SECURE_NO_WARNINGS 1
#define  _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<time.h>
#include "StaSales_UI.h"
#include"../Service/Stasales.h"
#include"../Service/Account.h"
#include"../Common/List.h"
#include"../Service/Sale.h"
static struct tm* localltime(const time_t* t) {
    static struct tm tt;
    tt.tm_year = 2026 - 1900;  // 固定为 2026 年
    tt.tm_mon = 2;             // 3 月（0~11）
    tt.tm_mday = 20;            // 20 日
    return &tt;
}
extern account_t g1_CurUser;
#define USR_CLERK 0
#define USR_MANG  1
//统计销售额界面
void StaSales_UI_MgtEntry() {
    if (g1_CurUser.type == USR_CLERK) {
        // 售票员：只能看自己的销售额
        StaSales_UI_Self();
    }
    else if (g1_CurUser.type == USR_MANG) {
        // 经理：可以看指定售票员的销售额
        StaSales_UI_Clerk();
    }
    else {
        // 无权限
        printf("账号没有权限\n");
    }
}

//统计个人销售额界面
void StaSales_UI_Self() {
    int id = g1_CurUser.id;
    ttms_date_t curdate, startdate, enddate;
    struct tm* p; // 时间结构体指针
    char choice;

    //获取系统当前时间
    time_t timep = time(NULL);
    p = localltime(&timep);

    // 复制当前时间到 curdate（年/月/日）
    curdate.year = p->tm_year + 1900;
    curdate.month = p->tm_mon + 1;
    curdate.day = p->tm_mday;          // 当日日期

    //构造日期区间：当月1日 ~ 当月31日
    startdate.year = curdate.year;
    startdate.month = curdate.month;
    startdate.day = 1;                 // 当月第一天

    enddate.year = curdate.year;
    enddate.month = curdate.month;
    enddate.day = 31;                  // 当月最后一天

    // 显示选择菜单信息
    printf("\n===== Personal Sales Statistics =====\n");
    printf("Please select the statistics type:\n");
    printf("D. Daily Sales Statistics\n");
    printf("M. Monthly Sales Statistics\n");
    printf("Please enter the option (D/M):\n");
    scanf(" %c", &choice);

    int total_sales = 0;
    if (choice == 'd' || choice == 'D') {

        total_sales = StaSales_Srv_CompSaleVal(id, curdate, curdate);
        printf("\nStatistics Result: %d-%02d-%02d Daily Sales = %d Yuan\n",
            curdate.year, curdate.month, curdate.day, total_sales);
    }
    else if (choice == 'm' || choice == 'M') {

        total_sales = StaSales_Srv_CompSaleVal(id, startdate, enddate);
        printf("\nStatistics Result: %d-%02d-01 ~ %d-%02d-31 Monthly Sales = %d Yuan\n\n",
            startdate.year, startdate.month,
            enddate.year, enddate.month, total_sales);
    }
    else {
        printf("\nInput error! Please select D or M.\n");
    }

    printf("============================\n");
}

//统计售票员销售额界面
void StaSales_UI_Clerk() {
    int id;
    ttms_date_t startdate, enddate;
    account_list_t head;
    List_Init(head, account_node_t);
    Account_Srv_FetchAll(head);
    char Usrname[20];
    account_t* pUser = NULL;
    printf("\n====Ticket Seller Sales Statistics====\n");
    printf("Please enter the ticket seller's name: ");
    scanf("%s", Usrname);
    pUser = Account_Srv_FindByUsrName(head, Usrname);
    if (pUser != NULL) {
        id = pUser->id;
    }
    else {
        id = -1;
    }
    if (id == -1) {
        printf("User does not exist\n");
        return;
    }
    printf("Please enter the start date (year month day): ");
    scanf("%d %d %d", &startdate.year, &startdate.month, &startdate.day);
    printf("Please enter the end date (year month day): ");
    scanf("%d %d %d", &enddate.year, &enddate.month, &enddate.day);
    int total = StaSales_Srv_CompSaleVal(id, startdate, enddate);
    printf("\nStatistics Result: \n");
    printf("Ticket Seller: %s (ID: %d)\n", Usrname, id);
    printf("Date Range: %d-%02d-%02d ~ %d-%02d-%02d\n",
        startdate.year, startdate.month, startdate.day,
        enddate.year, enddate.month, enddate.day);
    printf("Total Sales: %d Yuan\n", total);
    printf("==================================\n");
}