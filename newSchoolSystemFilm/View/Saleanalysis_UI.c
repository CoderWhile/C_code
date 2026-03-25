#define _CRT_SECURE_NO_WARNINGS 1
#define _CRT_SECURE_NO_WARNINGS
/*
* 用例编号：TTMS_UC_11
* File name: SalesAnalysis_UI.h
* Description : 统计票房界面层
*/

#include "../Service/Saleanalysis.h"
#include "../Common/List.h"
#include <stdio.h>

#define SALESANALYSIS_PAGE_SIZE 5

//统计票房界面
void SalesAnalysis_UI_MgtEntry() {
    //定义链表与分页变量
    salesanalysis_list_t head;
    int totalRecords;
    int currentPage = 1;
    char choice;
    salesanalysis_node_t* p;
    int i, start, end, maxPage;

    //初始化统计票房链表
    List_Init(head, salesanalysis_node_t);

    //调用业务层函数统计票房
    totalRecords = SalesAnalysis_Srv_StaticSale(head);

    //按票房降序排序
    SalesAnalysis_Srv_SortBySale(head);

    //分页交互循环
    while (1) {
        //分页显示剧目票房排行信息
        maxPage = (totalRecords + SALESANALYSIS_PAGE_SIZE - 1) / SALESANALYSIS_PAGE_SIZE;
        start = (currentPage - 1) * SALESANALYSIS_PAGE_SIZE + 1;
        end = currentPage * SALESANALYSIS_PAGE_SIZE;
        i = 0;

        printf("\n=====================================\n");
        printf("********Repertoire Box Office Ranking(Page %d/%d)********\n", currentPage, maxPage);
        printf("=====================================\n");
        printf("Title of the play\tarea\ttotaltickets,\tsales\tstart_date\tend_date\n");
        printf("---------------------------------------------------------------------\n");

        p = head->next;
        while (p != head) {
            i++;
            if (i >= start && i <= end) {
                printf("%s\t%s\t%ld\t%ld\t%d-%02d-%02d\t%d-%02d-%02d\n",
                    p->data.name, p->data.area, p->data.totaltickets, p->data.sales,
                    p->data.start_date.year, p->data.start_date.month, p->data.start_date.day,
                    p->data.end_date.year, p->data.end_date.month, p->data.end_date.day);
            }
            p = p->next;
        }
        if (totalRecords == 0) {
            printf("*************Repertoire Box Office Ranking (Page %d/%d)************\n");
        }

        //接收用户输入的按键字符
        printf("\n p/P-last page | n/N-next page | r/R-return\nplease input: ");
        scanf(" %c", &choice);
        while (getchar() != '\n'); // 清空输入缓冲区

        //处理上一页 ('p'/'P')
        if (choice == 'p' || choice == 'P') {
            if (currentPage > 1) {
                currentPage--;
            }
            else {
                printf("\n It's already the first page.！\n");
            }
        }
        //处理下一页 ('n'/'N')
        else if (choice == 'n' || choice == 'N') {
            if (currentPage < maxPage) {
                currentPage++;
            }
            else {
                printf("\n 已It's already the last page.！\n");
            }
        }
        //处理退出 ('r'/'R')
        else if (choice == 'r' || choice == 'R') {
            printf("\nExiting...\n");
            break;
        }
        //无效输入
        else {
            printf("\nInvalid input, please select again.！\n");
        }
    }

    // 销毁链表，释放内存
    List_Destroy(head, salesanalysis_node_t);
}