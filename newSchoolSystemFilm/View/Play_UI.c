#define _CRT_SECURE_NO_WARNINGS 1
#define  _CRT_SECURE_NO_WARNINGS 1
#define PLAY_PAGE_SIZE 5
#include "Play_UI.h"
#include "../Service/Play.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include"../Common/List.h"

void Play_UI_ShowPage(play_list_t list, Paging* paging) {
    printf("\n===== 剧目列表 第%d/%d页 =====\n", paging->currentPage, paging->maxPage);
    printf("[S]查询 [A]添加 [U]修改 [D]删除 [P]上一页 [N]下一页 [R]返回\n");

    if (list == NULL || paging == NULL) {
        return;
    }

    play_node_t* cur = list->next;
    int skip = (paging->currentPage - 1) * paging->pageSize;

    // ����ǰ���ҳ
    for (int i = 0; i < skip && cur != list; i++) {
        cur = cur->next;
    }

    // ��ʾ��ǰҳ
    for (int i = 0; i < paging->pageSize && cur != list; i++, cur = cur->next) {
        printf("ID: %d | 名称: %s | 类型: %d | 分级: %d | 时长: %d分钟\n",
            cur->data.id, cur->data.name, cur->data.type, cur->data.rating, cur->data.duration);
    }
}



// ��Ŀ���������
void Play_UI_MgtEntry(void) {
    play_list_t head;
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
        (void)getchar(); // ������з�

        switch (choice) {
        case 's': case 'S':
            Play_UI_Query();
            break;

        case 'a': case 'A':
            Play_UI_Add();
            // ˢ���б�
        
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

            // ˢ���б�
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

            // ˢ���б�
            
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

//  �����¾�Ŀ����
int Play_UI_Add(void) {
    play_t newPlay;
    printf("\n=== 添加新剧目 ===\n");
    printf("请输入剧目ID: ");
    scanf("%d", &newPlay.id);
    (void)getchar();

    printf("请输入剧目名称: ");
    fgets(newPlay.name, sizeof(newPlay.name), stdin);
    newPlay.name[strcspn(newPlay.name, "\n")] = '\0';

    printf("请输入剧目类型(1-话剧/2-歌剧/3-音乐会): ");
    scanf("%d", &newPlay.type);
    (void)getchar();

    printf("请输入剧目分级(1-儿童/2-青少年/3-成人): ");
    scanf("%d", &newPlay.rating);
    (void)getchar();

    printf("请输入剧目时长(分钟): ");
    scanf("%d", &newPlay.duration);
    (void)getchar();

    int result = Play_Srv_Add(&newPlay);
    if (result > 0) {
        printf("剧目添加成功！\n");
        return 1;
    }
    else {
        printf("剧目添加失败！\n");
        return 0;
    }
}

//�޸ľ�Ŀ����
int Play_UI_Modify(int id) {
    play_t play;
    int fetchResult = Play_Srv_FetchByID(id, &play);
    if (fetchResult == 0) {
        printf("未找到ID为%d的剧目！\n", id);
        return 0;
    }

    printf("\n=== 当前剧目信息 ===\n");
    printf("ID: %d\n名称: %s\n类型: %d\n分级: %d\n时长: %d分钟\n",
        play.id, play.name, play.type, play.rating, play.duration);

    printf("\n=== 输入新的剧目信息 ===\n");
    printf("请输入新名称: ");
    fgets(play.name, sizeof(play.name), stdin);
    play.name[strcspn(play.name, "\n")] = '\0';

    printf("请输入新类型(1-话剧/2-歌剧/3-音乐会): ");
    scanf("%d", &play.type);
    (void)getchar();

    printf("请输入新分级(1-儿童/2-青少年/3-成人): ");
    scanf("%d", &play.rating);
    (void)getchar();

    printf("请输入新时长(分钟): ");
    scanf("%d", &play.duration);
    (void)getchar();

    int rtn = Play_Srv_Modify(&play);
    if (rtn == 1) {
        printf("剧目修改成功！\n");
    }
    else {
        printf("剧目修改失败！\n");
    }
    return rtn;
}

//  ɾ����Ŀ����
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

//  ��ѯ��Ŀ����
int Play_UI_Query(void) {
    int playID;
    printf("请输入要查询的剧目ID: ");
    scanf("%d", &playID);
    (void)getchar();

    play_t play;
    int fetchResult = Play_Srv_FetchByID(playID, &play);
    if (fetchResult != 0) {
        printf("\n=== 查询成功！剧目信息如下 ===\n");
        printf("ID: %d\n名称: %s\n类型: %d\n分级: %d\n时长: %d分钟\n",
            play.id, play.name, play.type, play.rating, play.duration);
        return 1;
    }
    else {
        printf("未找到ID为%d的剧目！\n", playID);
        return 0;
    }
}

//  ��Ʋ�ѯ��Ŀ
void Play_UI_FetchByName() {
    char name[31];
    printf("请输入要查找的名字： ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0';

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
            printf("ID: %d\n名称: %s\n类型: %d\n分级: %d\n时长: %d分钟\n",
                pos->data.id, pos->data.name, pos->data.type, pos->data.rating, pos->data.duration);
        }
    }

    List_Destroy(head, play_node_t);
    system("pause");
}