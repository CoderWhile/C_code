#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "../Service/ShowQuery.h"
#include "../Service/ShowPlan.h"

static void ClearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

static void Pause() {
    printf("\n按回车键继续...");
    getchar();
    getchar();
}

void Schedule_UI_MgtEntry(int play_id) {
    int choice;
    schedule_list_t list = NULL;
    int count = Schedule_Srv_FetchByPlay(&list, play_id);

    while (1) {
        ClearScreen();
        printf("==================== 剧目ID:%d 演出计划管理 ====================\n", play_id);
        printf("当前演出计划列表：\n");
        printf("------------------------------------------------------------\n");

        if (count == 0) {
            printf("  暂无关联演出计划！\n");
        }
        else {
            schedule_node_t* p = list;
            while (p != NULL) {
                printf("  演出ID:%d | 剧目ID:%d | 演出厅ID:%d | 座位数:%d\n",
                    p->data.id, p->data.play_id, p->data.studio_id, p->data.seat_count);
                p = p->next;
            }
        }
        printf("------------------------------------------------------------\n");
        printf("  1. 新增演出计划\n");
        printf("  2. 修改演出计划\n");
        printf("  3. 删除演出计划\n");
        printf("  0. 返回上级菜单\n");
        printf("============================================================\n");
        printf("");
        scanf("%d", &choice);

        switch (choice) {
        case 1: {
            schedule_t new_sch;
            printf("\n請輸入新演出計劃信息\n");
            printf("演出id："); scanf("%d", &(new_sch.id));
            new_sch.play_id = play_id;
            printf("演出聼id："); scanf("%d", &(new_sch.studio_id));
            printf("座位數："); scanf("%d", &(new_sch.seat_count));

            if (Schedule_Srv_Add(&new_sch)) {
                printf("新增成功！\n");
                list = NULL;
                count = Schedule_Srv_FetchByPlay(&list, play_id);
            }
            else {
                printf("新增失敗！\n");
            }
            break;
        }
        case 2: {
            int sch_id;
            printf("\n请输入要修改的演出ID：");
            scanf("%d", &sch_id);

            schedule_t mod_sch;
            printf("请输入修改后的信息：\n");
            mod_sch.id = sch_id;
            mod_sch.play_id = play_id;
            printf("  演出厅ID："); scanf("%d", &mod_sch.studio_id);
            printf("  座位数："); scanf("%d", &mod_sch.seat_count);

            if (Schedule_Srv_Modify(&mod_sch)) {
                printf("  修改成功！\n");
                list = NULL;
                count = Schedule_Srv_FetchByPlay(&list, play_id);
            }
            else {
                printf("  修改失败（未找到该演出ID）！\n");
            }
            break;
        }
        case 3: {
            int sch_id;
            printf("\n请输入要删除的演出ID：");
            scanf("%d", &sch_id);

            if (Schedule_Srv_DeleteByID(sch_id)) {
                printf("  删除成功！\n");
                list = NULL;
                count = Schedule_Srv_FetchByPlay(&list, play_id);
            }
            else {
                printf("  删除失败（未找到该演出ID）！\n");
            }
            break;
        }
        case 0:
            return;
        default:
            printf("  输入错误，请重新选择！\n");
        }
        Pause();
    }
}

int Schedule_UI_Add(int play_id) {
    schedule_t new_sch;
    printf("\n==================== 新增演出计划 ====================\n");

    printf("请输入演出计划ID：");
    scanf("%d", &new_sch.id);
    new_sch.play_id = play_id;
    printf("请输入演出厅ID：");
    scanf("%d", &new_sch.studio_id);
    printf("请输入座位数：");
    scanf("%d", &new_sch.seat_count);

    int result = Schedule_Srv_Add(&new_sch);
    if (result == 1) {
        printf("  新演出计划添加成功！\n");
    }
    else {
        printf("  新演出计划添加失败！\n");
    }
    return result;
}

int Schedule_UI_Modify(int id) {
    schedule_t sch;
    if (!Schedule_Srv_FetchById(&sch, id)) {
        printf("  未找到ID为%d的演出计划！\n", id);
        return 0;
    }

    printf("\n==================== 修改演出计划 ====================\n");
    printf("当前演出计划信息：\n");
    printf("  演出ID：%d\n", sch.id);
    printf("  剧目ID：%d\n", sch.play_id);
    printf("  演出厅ID：%d\n", sch.studio_id);
    printf("  座位数：%d\n", sch.seat_count);
    printf("----------------------------------------------------\n");

    printf("请输入新的剧目ID（原：%d）：", sch.play_id);
    scanf("%d", &sch.play_id);
    printf("请输入新的演出厅ID（原：%d）：", sch.studio_id);
    scanf("%d", &sch.studio_id);
    printf("请输入新的座位数（原：%d）：", sch.seat_count);
    scanf("%d", &sch.seat_count);

    int result = Schedule_Srv_Modify(&sch);
    if (result == 1) {
        printf("  演出计划修改成功！\n");
    }
    else {
        printf("  演出计划修改失败！\n");
    }
    return result;
}

int Schedule_UI_Delete(int id) {
    schedule_t sch;
    if (!Schedule_Srv_FetchById(&sch, id)) {
        printf("  未找到ID为%d的演出计划！\n", id);
        return 0;
    }

    printf("\n==================== 删除演出计划 ====================\n");
    printf("你确定要删除以下演出计划吗？\n");
    printf("  演出ID：%d\n", sch.id);
    printf("  剧目ID：%d\n", sch.play_id);
    printf("  演出厅ID：%d\n", sch.studio_id);
    printf("  座位数：%d\n", sch.seat_count);
    printf("----------------------------------------------------\n");
    printf("输入 y 确认删除，其他键取消：");
    char confirm;
    scanf(" %c", &confirm);

    if (confirm != 'y' && confirm != 'Y') {
        printf("  已取消删除操作！\n");
        return 0;
    }

    int result = Schedule_Srv_DeleteByID(id);
    if (result == 1) {
        printf("  演出计划删除成功！\n");
    }
    else {
        printf("  演出计划删除失败！\n");
    }
    return result;
}