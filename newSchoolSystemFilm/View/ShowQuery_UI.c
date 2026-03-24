#define _CRT_SECURE_NO_WARNINGS
#include "../Service/ShowQuery.h"
#include "../Service/ShowPlan.h"
#include "../Common/List.h"
#include "../Service/Play.h"
#include "../Persistence/Play_Persist.h"
#include <stdio.h>
#include <string.h>
#include "../Service/Ticket.h"

// 打印单条演出信息
static void ShowSchedule(const schedule_t* pSch)
{
    play_t play;
    if (Play_Perst_SelByID(pSch->play_id, &play) == 1) {
        printf("演出ID：%d\t剧目：%s\t演出厅ID：%d\t时间：%s\n",
            pSch->id, play.name, pSch->studio_id, pSch->time);
    }
    else {
        printf("演出ID：%d\t剧目ID：%d\t演出厅ID：%d\t时间：%s\n",
            pSch->id, pSch->play_id, pSch->studio_id, pSch->time);
    }
}

// 查询所有演出（分页版）
void Schedule_UI_ListAll(void)
{
    struct schedule_node* list;
    Pagination_t paging;
    struct schedule_node* pos;
    int i;

    // 初始化链表
    List_Init(list, struct schedule_node);
    Schedule_Srv_FetchAll(list);

    // 统计总记录数
    paging.totalRecords = 0;
    List_ForEach(list, pos) {
        paging.totalRecords++;
    }

    // 初始化分页
    paging.pageSize = 5;
    Paging_Locate_FirstPage(list, paging);

    printf("\n========== 所有演出计划 ==========\n");
    printf("总记录：%d，总页数：%d\n",
		paging.totalRecords, Paging_TotalPages(paging));
    printf("=================================\n");

    if (paging.totalRecords == 0) {
        printf("暂无演出计划！\n");
        List_Destroy(list, struct schedule_node);
        return;
    }

    // 分页循环
    while (1) {
        printf("\n【第 %d 页】\n", Paging_CurPage(paging));
        Paging_ViewPage_ForEach(list, paging, struct schedule_node, pos, i) {
            ShowSchedule(&pos->data);
        }

        // 操作选择
        char op;
        printf("\n操作：n下一页 p上一页 q退出：");
        scanf(" %c", &op);

        switch (op) {
        case 'q':
        case 'Q':
            goto EXIT_LOOP;
        case 'n':
        case 'N':
            Paging_Locate_OffsetPage(list, paging, 1, struct schedule_node);
            break;
        case 'p':
        case 'P':
            Paging_Locate_OffsetPage(list, paging, -1, struct schedule_node);
            break;
        default:
             printf("输入错误！请输入 n/p/q\n");
             break;
        }
    }

    EXIT_LOOP:
    List_Destroy(list, struct schedule_node);
}

// 按剧目查询演出
int Schedule_UI_Query(void)
{
    char key[64];
    play_t play;
    struct schedule_node* list_s;
    struct schedule_node* pos;
    int count = 0;

    List_Init(list_s, struct schedule_node);

    printf("请输入剧目名称关键字：");
    scanf("%63s", key);

    if (Play_Srv_FetchByName(key, &play) <= 0) {
        printf("未找到该剧目！\n");
        List_Destroy(list_s, struct schedule_node);
        return 0;
    }

    Schedule_Srv_FetchByPlay(list_s, play.id);

    printf("\n===== 查询结果 =====\n");
    List_ForEach(list_s, pos) {
        printf("演出厅ID：%d，剧目名称：%s，座位数：%d\n",
            pos->data.studio_id, play.name, pos->data.seat_count);
        count++;
    }

    List_Destroy(list_s, struct schedule_node);
    return count;
}