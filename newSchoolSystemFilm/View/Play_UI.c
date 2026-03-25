#define _CRT_SECURE_NO_WARNINGS 1
#define  _CRT_SECURE_NO_WARNINGS 1
#define PLAY_PAGE_SIZE 5
#include "Play_UI.h"
#include "../Service/Play.h"
#include"../View/ShowPlan_UI.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include"../Common/List.h"

void Play_UI_ShowPage(play_list_t list, Paging* paging) {
    printf("\n=============================================================================================\n");
    printf("                                      剧目列表  第%d/%d页                                      \n", paging->currentPage, paging->maxPage);
    printf("[S]查询  [A]添加  [U]修改  [D]删除  [P]上一页  [N]下一页  [R]返回\n");
    printf("=============================================================================================\n");

    if (list == NULL || paging == NULL) {
        return;
    }

    play_node_t* cur = list->next;
    int skip = (paging->currentPage - 1) * paging->pageSize;

    for (int i = 0; i < skip && cur != list; i++) {
        cur = cur->next;
    }

    for (int i = 0; i < paging->pageSize && cur != list; i++, cur = cur->next) {
        printf("ID: %3d | 名称: %10s | 类型: %2d | 分级: %2d | 时长: %3d分钟 | 地区: %4s | 开始:%04d-%02d-%02d | 结束: %04d-%02d-%02d | 票价: %3d\n",
            cur->data.id,
            cur->data.name,
            cur->data.type,
            cur->data.rating,
            cur->data.duration,
            cur->data.area,
            cur->data.start_date.year, cur->data.start_date.month, cur->data.start_date.day,
            cur->data.end_date.year, cur->data.end_date.month, cur->data.end_date.day,
            cur->data.price);
    }
}

// 剧目管理主入口
void Play_UI_MgtEntry(void) {
    play_list_t head = NULL;
    play_node_t* pos;
    List_Init(head, play_node_t);

    if (!head) {
        printf("链表初始化失败！\n");
        return;
    }
    int total = Play_Srv_FetchAll(head);
    Paging paging;
    paging.totalRecords = total;
    paging.pageSize = PLAY_PAGE_SIZE;
    paging.currentPage = 1;
    paging.maxPage = (total + paging.pageSize - 1) / paging.pageSize;

    char choice;
    do {
        Play_UI_ShowPage(head, &paging);
        scanf(" %c", &choice);
        //choice = getchar();
        (void)getchar();// 清除换行符

        switch (choice) {
        case 's': case 'S':
            printf("1:根据id查询   2:根据名称查询\n");
			printf("请输入查询方式：");
            int  se;
            scanf("%d", &se);
            if (se == 1)
            {
                Play_UI_Query();
                

               
            }
            else if (se == 2)
            {
                Play_UI_UFetchByName();
            }
            else
            {
				printf("输入错误，返回主菜单！\n");
                return 0;
            }
            break;

        case 'a': case 'A':
            Play_UI_Add();
            // 刷新列表
            List_Destroy(head, play_node_t);
            List_Init(head, play_node_t);
            total = Play_Srv_FetchAll(head);
            paging.totalRecords = total;
            paging.maxPage = (total + paging.pageSize - 1) / paging.pageSize;
            paging.currentPage = 1;
            break;

        case 'u': case 'U': {
            int modify_id;
            printf("请输入要修改的剧目ID: ");
            scanf("%d", &modify_id);
            (void)getchar();
            Play_UI_Modify(modify_id);

            // 刷新列表
            total = Play_Srv_FetchAll(head);
            paging.totalRecords = total;
            paging.maxPage = (total + paging.pageSize - 1) / paging.pageSize;
            paging.currentPage = 1;
            break;
        }


        case 'd': case 'D': {
            int del_id;
            printf("请输入要删除的剧目ID: ");
            scanf("%d", &del_id);
            (void)getchar();
            Play_UI_Delete(del_id);

            // 刷新列表
            
            total = Play_Srv_FetchAll(head);
            paging.totalRecords = total;
            paging.maxPage = (total + paging.pageSize - 1) / paging.pageSize;
            paging.currentPage = 1;
            break;
        }

        case 'p': case 'P':
            if (paging.currentPage > 1) {
                paging.currentPage--;
            }
            else {
                printf("已经是第一页！\n");
            }
            break;

        case 'n': case 'N':
            if (paging.currentPage < paging.maxPage) {
                paging.currentPage++;
            }
            else {
                printf("已经是最后一页！\n");
            }
            break;

        case 'r': case 'R':
            printf("退出剧目管理界面\n");
            break;

        default:
            printf("输入无效，请重新选择！\n");
            break;
        }
    } while (choice != 'r' && choice != 'R');

    List_Destroy(head,play_node_t);
}

//  添加新剧目界面
int Play_UI_Add(void) {
    play_t newPlay;
    int inputValid = 0; // 标记输入是否有效
    printf("\n=== Add New Play ===\n");
   // do {
       // printf("Enter Play ID (Positive Integer): ");
        //inputValid = scanf("%d", &newPlay.id);

        /*if (inputValid != 1 || newPlay.id <= 0) {
            printf("Error: Play ID must be a positive integer! Please try again.\n");
            while (getchar() != '\n');
            inputValid = 0;

            printf("Input error, back to menu!\n");
            return 0;
        }
        else {
            while (getchar() != '\n');
            inputValid = 1;
        }
    } while (inputValid != 1);*/

   // play_t temp;
    //int exists = Play_Perst_SelectByID(newPlay.id, &temp); // 直接调用持久层
   // if (exists !=0) {
     //   printf("Error: ID already exists! Back to menu!\n");
      //  return 0;
    //}

    // 2. 剧目名称：不能为空
    do {
        printf("Enter Play Name (Cannot be empty): ");
        fgets(newPlay.name, sizeof(newPlay.name), stdin);
        newPlay.name[strcspn(newPlay.name, "\n")] = '\0';

        if (strlen(newPlay.name) == 0 || strspn(newPlay.name, " ") == strlen(newPlay.name)) {
            printf("Error: Play Name cannot be empty! Please try again.\n");
            inputValid = 0;

            printf("Input error, back to menu!\n");
            return 0;
        }
        else {
            inputValid = 1;
        }
    } while (inputValid != 1);

    // 3. 剧目类型
    do {
        printf("Enter Play Type (1-Drama/2-Opera/3-Concert): ");
        inputValid = scanf("%d", &newPlay.type);

        if (inputValid != 1 || newPlay.type < 1 || newPlay.type > 3) {
            printf("Error: Play Type must be 1/2/3! Please try again.\n");
            while (getchar() != '\n');
            inputValid = 0;

            printf("Input error, back to menu!\n");
            return 0;
        }
        else {
            while (getchar() != '\n');
            inputValid = 1;
        }
    } while (inputValid != 1);

    // 4. 剧目分级
    do {
        printf("Enter Play Rating (1-Children/2-Teens/3-Adults): ");
        inputValid = scanf("%d", &newPlay.rating);

        if (inputValid != 1 || (newPlay.rating != 1 && newPlay.rating != 2 && newPlay.rating != 3)) {
            printf("Error: Play Rating must be 1/2/3! Please try again.\n");
            while (getchar() != '\n');
            inputValid = 0;

            printf("Input error, back to menu!\n");
            return 0;
        }
        else {
            while (getchar() != '\n');
            inputValid = 1;
        }
    } while (inputValid != 1);

    // 5. 剧目时长
    do {
        printf("Enter Play Duration (minutes, ≥5): ");
        inputValid = scanf("%d", &newPlay.duration);

        if (inputValid != 1 || newPlay.duration < 5) {
            printf("Error: Duration must be ≥5 minutes! Please try again.\n");
            while (getchar() != '\n');
            inputValid = 0;

            printf("Input error, back to menu!\n");
            return 0;
        }
        else {
            while (getchar() != '\n');
            inputValid = 1;
        }
    } while (inputValid != 1);

    //地区
    do {
        printf("Enter Play Area (Cannot be empty): ");
        fgets(newPlay.area, sizeof(newPlay.area), stdin);
        newPlay.area[strcspn(newPlay.area, "\n")] = '\0';

        if (strlen(newPlay.area) == 0 || strspn(newPlay.area, " ") == strlen(newPlay.area)) {
            printf("Error: Area cannot be empty!\n");
            inputValid = 0;
            printf("Input error, back to menu!\n");
            return 0;
        }
        else {
            inputValid = 1;
        }
    } while (inputValid != 1);

    //开始时间
    do {
        printf("Enter Start Date (yyyy mm dd, e.g., 2025 4 1): ");
        inputValid = scanf("%d %d %d",
            &newPlay.start_date.year,
            &newPlay.start_date.month,
            &newPlay.start_date.day);

        // 简单日期合法性校验
        if (inputValid != 3 ||
            newPlay.start_date.year < 1900 || newPlay.start_date.year > 2100 ||
            newPlay.start_date.month < 1 || newPlay.start_date.month > 12 ||
            newPlay.start_date.day < 1 || newPlay.start_date.day > 31) {
            printf("Error: Invalid date format or value!\n");
            while (getchar() != '\n');
            inputValid = 0;
            printf("Input error, back to menu!\n");
            return 0;
        }
        else {
            while (getchar() != '\n');
            inputValid = 1;
        }
    } while (inputValid != 1);

    //结束时间
    do {
        printf("Enter End Date (yyyy mm dd, e.g., 2025 4 30): ");
        inputValid = scanf("%d %d %d",
            &newPlay.end_date.year,
            &newPlay.end_date.month,
            &newPlay.end_date.day);

        if (inputValid != 3 ||
            newPlay.end_date.year < 1900 || newPlay.end_date.year > 2100 ||
            newPlay.end_date.month < 1 || newPlay.end_date.month > 12 ||
            newPlay.end_date.day < 1 || newPlay.end_date.day > 31) {
            printf("Error: Invalid date format or value!\n");
            while (getchar() != '\n');
            inputValid = 0;
            printf("Input error, back to menu!\n");
            return 0;
        }
        else {
            while (getchar() != '\n');
            inputValid = 1;
        }
    } while (inputValid != 1);

    //票价
    do {
        printf("Enter Ticket Price (≥0): ");
        inputValid = scanf("%d", &newPlay.price);

        if (inputValid != 1 || newPlay.price < 0) {
            printf("Error: Price must be a number ≥0!\n");
            while (getchar() != '\n');
            inputValid = 0;
            printf("Input error, back to menu!\n");
            return 0;
        }
        else {
            while (getchar() != '\n');
            inputValid = 1;
        }
    } while (inputValid != 1);


    int result = Play_Srv_Add(&newPlay);
    if (result > 0) {
        printf("Play added successfully!\n");
        return 1;
    }
    else {
        printf("Failed to add play!\n");
        return 0;
    }
}

//修改剧目界面
int Play_UI_Modify(int id) {
    play_t play;
    int fetchResult = Play_Srv_FetchByID(id, &play);
    if (fetchResult == 0) {
        printf("Play with ID %d not found!\n", id);
        return 0;
    }

    printf("\n=== Current Play Information ===\n");
    printf("ID: %d\nName: %s\nType: %d\nRating: %d\nDuration: %d minutes\n",
        play.id, play.name, play.type, play.rating, play.duration);
    printf("Area: %s\nStart: %04d-%02d-%02d\nEnd: %04d-%02d-%02d\nPrice: %d\n",
        play.area,
        play.start_date.year, play.start_date.month, play.start_date.day,
        play.end_date.year, play.end_date.month, play.end_date.day,
        play.price);

    printf("\n=== Enter New Play Information ===\n");

    printf("Enter new name: ");
    fgets(play.name, sizeof(play.name), stdin);
    play.name[strcspn(play.name, "\n")] = '\0';

    if (strlen(play.name) == 0 || strspn(play.name, " ") == strlen(play.name)) {
        printf("Input error! Back to menu!\n");
        return 0;
    }

    int inputValid;
    printf("Enter new type (1-Drama/2-Opera/3-Concert): ");
    inputValid = scanf("%d", &play.type);
    (void)getchar();

    if (inputValid != 1 || play.type < 1 || play.type > 3) {
        printf("Input error! Back to menu!\n");
        return 0;
    }

    printf("Enter new rating (1-Children/2-Teens/3-Adults): ");
    inputValid = scanf("%d", &play.rating);
    (void)getchar();

    if (inputValid != 1 || play.rating < 1 || play.rating > 3) {
        printf("Input error! Back to menu!\n");
        return 0;
    }

    printf("Enter new duration (minutes): ");
    inputValid = scanf("%d", &play.duration);
    (void)getchar();

    if (inputValid != 1 || play.duration < 5) {
        printf("Input error! Back to menu!\n");
        return 0;
    }

    printf("Enter new area: ");
    fgets(play.area, sizeof(play.area), stdin);
    play.area[strcspn(play.area, "\n")] = '\0';

    if (strlen(play.area) == 0 || strspn(play.area, " ") == strlen(play.area)) {
        printf("Input error! Back to menu!\n");
        return 0;
    }

    printf("Enter new start date (yyyy mm dd): ");
    inputValid = scanf("%d %d %d",
        &play.start_date.year,
        &play.start_date.month,
        &play.start_date.day);
    (void)getchar();

    if (inputValid != 3 ||
        play.start_date.year < 2000 || play.start_date.year > 2100 ||
        play.start_date.month < 1 || play.start_date.month > 12 ||
        play.start_date.day < 1 || play.start_date.day > 31) {
        printf("Input error! Back to menu!\n");
        return 0;
    }

    printf("Enter new end date (yyyy mm dd): ");
    inputValid = scanf("%d %d %d",
        &play.end_date.year,
        &play.end_date.month,
        &play.end_date.day);
    (void)getchar();

    if (inputValid != 3 ||
        play.end_date.year < 2000 || play.end_date.year > 2100 ||
        play.end_date.month < 1 || play.end_date.month > 12 ||
        play.end_date.day < 1 || play.end_date.day > 31) {
        printf("Input error! Back to menu!\n");
        return 0;
    }

    printf("Enter new price (>=0): ");
    inputValid = scanf("%d", &play.price);
    (void)getchar();

    if (inputValid != 1 || play.price < 0) {
        printf("Input error! Back to menu!\n");
        return 0;
    }

    

    int rtn = Play_Srv_Modify(&play);
    if (rtn == 1) {
        printf("Play modified successfully!\n");
    }
    else {
        printf("Failed to modify play!\n");
    }
    return rtn;
}

//  删除剧目界面
int Play_UI_Delete(int id) {
    int delResult = Play_Srv_DeleteByID(id);
    if (delResult != 0) {
        printf("剧目ID %d 删除成功！\n", id);
        return 1;
    }
    else {
        printf("剧目ID %d 删除失败！\n", id);
        return 0;
    }
}

//  查询剧目界面
int Play_UI_Query(void) {
    int playID;
    printf("请输入要查询的剧目ID: ");
    scanf("%d", &playID);
    (void)getchar();

    play_t play;
    int fetchResult = Play_Srv_FetchByID(playID, &play);
    if (fetchResult != 0) {
        printf("\n=== 查询成功！剧目信息如下 ===\n");
        printf("ID: %d\n名称: %s\n类型: %d\n分级: %d\n时长: %d分钟\n地区: %s\n开始日期: %04d-%02d-%02d\n结束日期: %04d-%02d-%02d\n票价: %d\n",
            play.id,
            play.name,
            play.type,
            play.rating,
            play.duration,
            play.area,
            play.start_date.year, play.start_date.month, play.start_date.day,
            play.end_date.year, play.end_date.month, play.end_date.day,
            play.price);
        Schedule_UI_MgtEntry(play.id);
        return 1;
    }
    else {
        printf("未找到ID为%d的剧目！\n", playID);
        return 0;
    }
}

//  ��Ʋ�ѯ��Ŀ
void Play_UI_UFetchByName() {
    char name[31];
    printf("请输入要查找的名字： ");
    //fgets(name, sizeof(name), stdin);
    
    //name[strcspn(name, "\n")] = '\0';
    scanf("%s", name);

    play_list_t head;
    play_node_t* pos;
    List_Init(head, play_node_t);

    int total = Play_Srv_FetchAll(head);
    if (total == 0) {
        printf("没有信息\n");
        List_Destroy(head, play_node_t);
        return;
    }

    printf("\n=== 查询成功！剧目信息如下 ===\n");
    List_ForEach(head, pos) {
        if (strstr(pos->data.name, name) != NULL) {
            printf("ID: %d\n名称: %s\n类型: %d\n分级: %d\n时长: %d分钟\n地区: %s\n开始日期: %04d-%02d-%02d\n结束日期: %04d-%02d-%02d\n票价: %d\n",
                pos->data.id,
                pos->data.name,
                pos->data.type,
                pos->data.rating,
                pos->data.duration,
                pos->data.area,
                pos->data.start_date.year, pos->data.start_date.month, pos->data.start_date.day,
                pos->data.end_date.year, pos->data.end_date.month, pos->data.end_date.day,
                pos->data.price);
            Schedule_UI_MgtEntry(pos->data.id);

        }
    }

    List_Destroy(head, play_node_t);
    system("pause");
}