#define _CRT_SECURE_NO_WARNINGS 1
#include "../View/Studio_UI.h"

#include "../Common/List.h"
#include"../Service/Schedule.h"
#include"../Service/Studio.h"
#include<stdio.h>
#include<stdlib.h>
#include"../Service/play.h"
#include"Ticket_UI.h"
static const int SCHEDULE_PAGE_SIZE = 5;



void Schedule_UI_MgtEntry(int play_id)
{
    int i, sch_id;
    char choice;

    schedule_list_t head;
    schedule_node_t* pos;
    Pagination_t paging;

    // 1. 校验剧目是否存在 (根据你提供的play_t结构体)
    play_t target_play;
    // 修正：传入结构体指针接收数据，而非直接赋值
    if (!Play_Srv_FetchByID(play_id, &target_play)) {
        printf("\n错误：ID为 %d 的剧目不存在！\n", play_id);
        return;
    }

    // 2. 初始化链表与分页
    List_Init(head, schedule_node_t); // 使用双向链表初始化宏
    paging.offset = 0;
    paging.pageSize = SCHEDULE_PAGE_SIZE;

    // 3. 加载该剧目下的所有演出计划
    paging.totalRecords = Schedule_Srv_FetchByPlay(head, play_id);
    Paging_Locate_FirstPage(head, paging);

    do {
        // 4. 显示界面标题与剧目信息
        printf("\n==================================================================\n");
        printf("演出计划管理 (剧目: %s)\n", target_play.name);
        printf("------------------------------------------------------------------\n");
        printf("%5s  %10s  %10s  %12s  %12s  %10s\n",
            "ID", "剧目ID", "放映厅ID", "放映日期", "放映时间", "总座位数");
        printf("------------------------------------------------------------------\n");

        // 5. 分页遍历显示当前页数据
        Paging_ViewPage_ForEach(head, paging, schedule_node_t, pos, i) {
            printf("%5d  %10d  %10d  %04d-%02d-%02d  %02d:%02d  %10d\n",
                pos->data.id,
                pos->data.play_id,
                pos->data.studio_id,
                pos->data.date.year, pos->data.date.month, pos->data.date.day,
                pos->data.time.hour, pos->data.time.minute,
                pos->data.seat_count);
        }

        // 6. 分页信息与操作菜单
        printf("------------------------------------------------------------------\n");
        printf("总记录数: %2d | 当前页: %2d/%2d | [P]上一页 [N]下一页\n",
            paging.totalRecords,
            Pageing_CurPage(paging),
            Pageing_TotalPages(paging));
        printf("==================================================================\n");
        printf("[T]售票 | [A]添加 | [U]修改 | [D]删除 | [R]返回主菜单\n");
        printf("==================================================================\n");
        printf("请输入操作: ");
        fflush(stdin);
        scanf(" %c", &choice);
        fflush(stdin);

        // 7. 业务逻辑处理
        switch (choice) {
        case 't':
        case 'T':
            printf("请输入演出计划ID: ");
            scanf("%d", &sch_id);
            // 校验演出计划是否存在
            schedule_t temp_sch;
            if (Schedule_Srv_FetchByID(sch_id, &temp_sch)) {
                // 进入售票界面
                Ticket_UI_MgtEntry(sch_id);
                // 售票后刷新座位数
                paging.totalRecords = Schedule_Srv_FetchByPlay(head, play_id);
                List_Paging(head, paging, schedule_node_t);
            }
            else {
                printf(" 错误：未找到ID为 %d 的演出计划！\n", sch_id);
            }
            break;

        case 'a':
        case 'A':
            if (Schedule_UI_Add(play_id))
            {
                printf("添加成功\n");
            }
            else {
                printf(" 演出计划添加失败！\n");
            }
            break;


        case 'u':
        case 'U':
            printf("请输入要修改的演出计划ID: ");
            scanf("%d", &sch_id);
            if (Schedule_UI_Modify(sch_id)) {
                paging.totalRecords = Schedule_Srv_FetchByPlay(head, play_id);
                List_Paging(head, paging, schedule_node_t);
            }
            break;

        case 'd':
        case 'D':
            printf("请输入要删除的演出计划ID: ");
            scanf("%d", &sch_id);
            if (Schedule_UI_Delete(sch_id)) {
                paging.totalRecords = Schedule_Srv_FetchByPlay(head, play_id);
                List_Paging(head, paging, schedule_node_t);
            }
            break;

        case 'p':
        case 'P':
            if (!Pageing_IsFirstPage(paging)) {
                Paging_Locate_OffsetPage(head, paging, -1, schedule_node_t);
            }
            break;

        case 'n':
        case 'N':
            if (!Pageing_IsLastPage(paging)) {
                Paging_Locate_OffsetPage(head, paging, 1, schedule_node_t);
            }
            break;
        }
    } while (choice != 'r' && choice != 'R');

    // 8. 释放链表内存
    List_Destroy(head, schedule_node_t);
}
int Schedule_UI_Add(int play_id)
{
    schedule_t new_sch;
    studio_t temp_studio;
    int ret;

    // 初始化新演出计划的剧目ID
    new_sch.play_id = play_id;

    printf("\n========== 添加新演出计划 ==========\n");
    printf("请输入放映厅ID: ");
    scanf("%d", &new_sch.studio_id);

    // 校验放映厅是否存在，并获取座位数
    if (!Studio_Srv_FetchByID(new_sch.studio_id, &temp_studio)) {
        printf("错误：未找到ID为 %d 的放映厅！\n", new_sch.studio_id);
        return 0;
    }
    new_sch.seat_count = temp_studio.seatsCount;

    // 输入放映日期
    printf("请输入放映日期（年 月 日，空格分隔）: ");
    scanf("%d %d %d",
        &new_sch.date.year,
        &new_sch.date.month,
        &new_sch.date.day);

    // 输入放映时间
    printf("请输入放映时间（时 分，空格分隔）: ");
    scanf("%d %d",
        &new_sch.time.hour,
        &new_sch.time.minute);

    // 调用业务逻辑层添加
    ret = Schedule_Srv_Add(&new_sch);
    if (ret == 1) {
        printf("演出计划添加成功！\n");
    }
    else {
        printf("演出计划添加失败！\n");
    }

    return ret;
}
int Schedule_UI_Modify(int id)
{
    schedule_t sch;
    studio_t temp_studio;
    int ret;

    // 先查询原演出计划
    if (!Schedule_Srv_FetchByID(id, &sch)) {
        printf("错误：未找到ID为 %d 的演出计划！\n", id);
        return 0;
    }

    printf("\n========== 修改演出计划 ==========\n");
    printf("当前放映厅ID: %d，输入新放映厅ID: ", sch.studio_id);
    scanf("%d", &sch.studio_id);

    // 校验新放映厅是否存在
    if (!Studio_Srv_FetchByID(sch.studio_id, &temp_studio)) {
        printf("错误：未找到ID为 %d 的放映厅！\n", sch.studio_id);
        return 0;
    }
    sch.seat_count = temp_studio.seatsCount;

    // 修改放映日期
    printf("当前日期: %04d-%02d-%02d，输入新日期（年 月 日）: ",
        sch.date.year, sch.date.month, sch.date.day);
    scanf("%d %d %d",
        &sch.date.year,
        &sch.date.month,
        &sch.date.day);

    // 修改放映时间
    printf("当前时间: %02d:%02d，输入新时间（时 分）: ",
        sch.time.hour, sch.time.minute);
    scanf("%d %d",
        &sch.time.hour,
        &sch.time.minute);

    // 调用业务逻辑层修改
    ret = Schedule_Srv_Modify(&sch);
    if (ret == 1) {
        printf("演出计划修改成功！\n");
    }
    else {
        printf(" 演出计划修改失败！\n");
    }

    return ret;
}
int Schedule_UI_Delete(int id)
{
    char confirm;
    int ret;

    // 先查询是否存在
    schedule_t temp;
    if (!Schedule_Srv_FetchByID(id, &temp)) {
        printf(" 错误：未找到ID为 %d 的演出计划！\n", id);
        return 0;
    }

    printf("\n========== 删除演出计划 ==========\n");
    printf("确认删除ID为 %d 的演出计划吗？(y/n): ", id);
    fflush(stdin);
    scanf(" %c", &confirm);

    if (confirm == 'y' || confirm == 'Y') {
        ret = Schedule_Srv_DeleteByID(id);
        if (ret == 1) {
            printf("演出计划删除成功！\n");
        }
        else {
            printf("演出计划删除失败！\n");
        }
    }
    else {
        printf("已取消删除操作。\n");
        ret = 0;
    }

    return ret;
}

void Schedule_UI_ListAll()
{
    int i;
    char choice;
    schedule_list_t list;      // 演出计划链表头
    schedule_node_t* pos;      // 遍历指针
    Pagination_t paging;       // 分页器
    play_t play_info;          // 剧目信息
    studio_t studio_info;      // 放映厅信息

    // a) 初始化链表与分页器
    List_Init(list, schedule_node_t);
    paging.offset = 0;
    paging.pageSize = SCHEDULE_PAGE_SIZE;

    // b) 获取所有演出计划
    paging.totalRecords = Schedule_Srv_FetchAll(list);
    Paging_Locate_FirstPage(list, paging);

    do {
        // 显示表头
        printf("\n==================================================================\n");
        printf("所有演出计划列表\n");
        printf("------------------------------------------------------------------\n");
        printf("%5s  %12s  %12s  %12s  %12s  %10s\n",
            "ID", "剧目名称", "放映厅名称", "放映日期", "放映时间", "座位数");
        printf("------------------------------------------------------------------\n");

        i = 0;
        pos = list->next;
        // 跳过分页偏移量
        for (int j = 0; j < paging.offset && pos != NULL; j++) {
            pos = pos->next;
        }

        // c) 遍历当前页数据，获取关联信息并显示
        while (pos != NULL && i < paging.pageSize) {
            // 获取剧目信息
            Play_Srv_FetchByID(pos->data.play_id, &play_info);
            // 获取放映厅信息
            Studio_Srv_FindByID(pos->data.studio_id, &studio_info);

            // d) 显示完整信息
            printf("%5d  %12s  %12s  %04d-%02d-%02d  %02d:%02d  %10d\n",
                pos->data.id,
                play_info.name,
                studio_info.name,
                pos->data.date.year, pos->data.date.month, pos->data.date.day,
                pos->data.time.hour, pos->data.time.minute,
                pos->data.seat_count);

            pos = pos->next;
            i++;
        }

        // 分页信息与操作菜单
        printf("------------------------------------------------------------------\n");
        printf("总记录数: %2d | 当前页: %2d/%2d\n",
            paging.totalRecords,
            Pageing_CurPage(paging),
            Pageing_TotalPages(paging));
        printf("==================================================================\n");
        printf("[P]上一页 | [N]下一页 | [Q]查询 | [R]返回\n");
        printf("==================================================================\n");
        printf("请输入操作: ");
        fflush(stdin);
        scanf(" %c", &choice);
        fflush(stdin);

        // e) 处理用户输入
        switch (choice) {
        case 'p':
        case 'P':
            if (!Pageing_IsFirstPage(paging)) {
                Paging_Locate_OffsetPage(list, paging, -1,schedule_node_t);
            }
            break;
        case 'n':
        case 'N':
            if (!Pageing_IsLastPage(paging)) {
                Paging_Locate_OffsetPage(list, paging, 1, schedule_node_t);
            }
            break;
        case 'q':
        case 'Q':
            Schedule_UI_Query();  // 调用查询函数
            break;
        case 'r':
        case 'R':
            break;  // 直接跳出循环，执行步骤f
        default:
            printf("输入错误，请重新选择功能！\n");
            break;
        }
    } while (choice != 'r' && choice != 'R');

    // f) 销毁链表，释放内存
    List_Destroy(list, schedule_node_t);
} 
int Schedule_UI_Query()
{
    char play_name[50];          // 存储用户输入的剧目名称
    play_list_t list_p = NULL;   // 存储匹配到的剧目链表
    schedule_list_t list_s = NULL;// 存储匹配到的演出计划链表
    play_node_t* p_pos = NULL;   // 遍历剧目链表的指针
    schedule_node_t* s_pos = NULL;// 遍历演出计划链表的指针
    play_t play_info;           // 存储单个剧目信息
    studio_t studio_info;       // 存储单个放映厅信息
    int count = 0;              // 记录最终的演出计划数量

    // a) 提示用户输入待查的剧目名称
    printf("\n========== 根据剧目名称查询演出计划 ==========\n");
    printf("请输入剧目名称: ");
    fflush(stdin);
    scanf("%[^\n]", play_name); // 支持带空格的名称输入

    // b) 调用业务逻辑层函数获取匹配的剧目链表
    list_p = (play_list_t)malloc(sizeof(play_node_t));
    List_Init(list_p, play_node_t); // 初始化剧目链表
    Play_Srv_FetchByName(list_p, play_name);

    // 如果没有找到对应剧目，直接返回0
    if (List_IsEmpty(list_p)) {
        printf("未找到名称为【%s】的剧目！\n", play_name);
        List_Destroy(list_p, play_node_t);
        return 0;
    }

    // 初始化演出计划链表
    list_s = (schedule_list_t)malloc(sizeof(schedule_node_t));
    List_Init(list_s, schedule_node_t);

    // c) 逐个取剧目链表结点的剧目ID，获取对应的演出计划
    List_ForEach(list_p, p_pos, play_node_t) {
        int current_play_id = p_pos->data.id;
        // 调用业务逻辑层函数，将该剧目下的演出计划加载到list_s
        count += Schedule_Srv_FetchByPlay(list_s, current_play_id);
    }

    // 如果没有找到对应的演出计划，返回0
    if (List_IsEmpty(list_s)) {
        printf("未找到【%s】相关的演出计划！\n", play_name);
        List_Destroy(list_p, play_node_t);
        List_Destroy(list_s, schedule_node_t);
        return 0;
    }

    // d) 逐个显示演出计划的详细信息
    printf("\n========== 【%s】相关演出计划 ==========\n", play_name);
    printf("%-15s  %-6s  %-10s  %-12s  %-10s  %-8s\n",
        "剧目名称", "时长(min)", "放映厅编号", "演出日期", "开始时间", "座位数");
    printf("-------------------------------------------------------------------------\n");

    List_ForEach(list_s, s_pos, schedule_node_t) {
        // 获取当前演出计划关联的剧目信息
        Play_Srv_FetchByID(s_pos->data.play_id, &play_info);
        // 获取当前演出计划关联的放映厅信息
        Studio_Srv_FetchByID(s_pos->data.studio_id, &studio_info);

        // 格式化输出信息
        printf("%-15s  %-6d  %-10d  %04d-%02d-%02d  %02d:%02d  %-8d\n",
            play_info.name,
            play_info.duration,
            studio_info.id,
            s_pos->data.date.year, s_pos->data.date.month, s_pos->data.date.day,
            s_pos->data.time.hour, s_pos->data.time.minute,
            s_pos->data.seat_count);
    }

    // e) 释放链表内存，返回演出计划数量
    List_Destroy(list_p, play_node_t);
    List_Destroy(list_s, schedule_node_t);
    return count;
}