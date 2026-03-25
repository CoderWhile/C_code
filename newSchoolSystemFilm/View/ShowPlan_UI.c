#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <stdlib.h>
#include "../Service/ShowQuery.h"
#include "../Service/ShowPlan.h"
#include"../Service/Ticket.h"
#include "../View/Ticket_UI.h"
#include "../Common/common.h"
#include "../Service/play.h"
#include"../Service/Studio.h"

static void ClearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

static void Pause() {
    printf("\nPress Enter to continue....");
    getchar();
    getchar();
}

void Schedule_UI_MgtEntry(int play_id) {
    int choice;
    schedule_list_t list = NULL;
    List_Init(list, schedule_node_t)
    int count = Schedule_Srv_FetchByPlay(list, play_id);

    while (1) {
        ClearScreen();
        int count = Schedule_Srv_FetchByPlay(list, play_id);

        printf("==================== Repertoire ID: %d Performance Schedule Management ====================\n", play_id);
        printf("Current performance schedule list：\n");
        printf("------------------------------------------------------------\n");

        if (count == 0) {
            printf("There are no related performance plans yet.\n");
        }
        else {
            schedule_node_t* p = list->next;
            while (p != list) {
                printf("Performance ID: %d | Play ID: %d | studio ID: %d | Number of seats: %d |date:%d-%d-%d |time:%d %d\n",
                    p->data.id, p->data.play_id, p->data.studio_id, p->data.seat_count,p->data.date.year, p->data.date.month, p->data.date.day, p->data.time.hour,p->data.time.minute );
                p = p->next;
            }
        }
        printf("------------------------------------------------------------\n");
        printf("  1. Newly added performance plan\n");
        printf("  2. Modify the performance plan\n");
        printf("  3. Delete the performance plan\n");
        printf("  4. Search the performance plan\n");

        printf("  0. Return to the previous menu\n");
        printf("============================================================\n");
        printf("");
        scanf("%d", &choice);

        switch (choice) {
        case 1: {

            if (Schedule_UI_Add(play_id))
            {
                printf("add successfully\n");
            }
            else {
                printf("add failed！\n");
            }
            break;
        }
        case 2: {
            int sch_id;
            printf("\nPlease enter the performance ID to be modified：");
            scanf("%d", &sch_id);

            schedule_t mod_sch;
            printf("Please enter the revised information：\n");
            mod_sch.id = sch_id;
            mod_sch.play_id = play_id;
            printf(" studioID:"); scanf("%d", &mod_sch.studio_id);
            printf(" seat_count:"); scanf("%d", &mod_sch.seat_count);

            if (Schedule_Srv_Modify(&mod_sch)) {
                printf("modify successfully！\n");
                list = NULL;
                count = Schedule_Srv_FetchByPlay(&list, play_id);
            }
            else {
                printf("Modification failed (the performance ID was not found)！\n");
            }
            break;
        }
        case 3: {
            int sch_id;
            printf("\nPlease enter the performance ID to be deleted：");
            scanf("%d", &sch_id);

            if (Schedule_Srv_DeleteByID(sch_id)) {
                printf("Deletion successful！\n");
                list = NULL;
                count = Schedule_Srv_FetchByPlay(&list, play_id);
            }
            else {
                printf("Deletion failed (the performance ID was not found)！\n");
            }
            break;
        }
        case 4: {
            int sch_id;
            printf("\nPlease enter the performance ID to be search:");
            scanf("%d", &sch_id);
            schedule_t pos;
            if (Schedule_Srv_FetchByID(sch_id,&pos)) {
                printf("Find successful！\n");
                Ticket_UI_MgtEntry(pos.id);
            }
            else {
                printf("Deletion failed (the performance ID was not found)！\n");
            }
            break;
        }
        case 0:
            return;
        default:
            printf("Input error, please select again!\n");
        }
        Pause();
    }
}

int Schedule_UI_Add(int play_id) {
    schedule_t new_sch;
    printf("\n==================== Newly added performance plan ====================\n");

  
    new_sch.play_id = play_id;
    printf("Please enter the performance hall ID：");
    scanf("%d", &new_sch.studio_id);
    studio_list_t head;
    List_Init(head, studio_node_t);
    Studio_Srv_FetchAll(head);
    studio_t buf;
    Studio_Srv_FetchByID(new_sch.studio_id, &buf);
    //printf("Please enter the seatofcount：");
    //scanf("%d", &new_sch.seat_count);
    new_sch.seat_count = buf.seatsCount;
    printf("Please enter the Release date：");
    scanf("%d %d %d", &new_sch.date.year, &new_sch.date.month, &new_sch.date.day);
    printf("Please enter the Show time：");
    scanf("%d %d %d", &new_sch.time.hour, &new_sch.time.minute, &new_sch.time.second);

    int result = Schedule_Srv_Add(&new_sch);
    if (result == 1) {
        printf("The new performance plan has been added successfully.！\n");
   
    }
    else {
        printf("Failed to add the new performance plan\n");
    }
    return result;
}

int Schedule_UI_Modify(int id) {
    schedule_t sch;
    if (!Schedule_Srv_FetchById(&sch,id)) {
        printf("No performance plan with ID %d found!\n", id);
        return 0;
    }

    printf("\n==================== Revise the performance plan ====================\n");
    printf("Current performance schedule information：\n");
    printf("schedule ID：%d\n", sch.id);
    printf("play ID：%d\n", sch.play_id);
    printf("studio ID：%d\n", sch.studio_id);
    printf("count of seat：%d\n", sch.seat_count);
    printf("Release date：%4d-%2d-%2d\n", sch.date.year, sch.date.month, sch.date.day);
    printf("Show time：%2d:%2d:%2d\n", sch.time.hour, sch.time.minute, sch.time.second);
    printf("----------------------------------------------------\n");

    printf("input new play ID（pre：%d）：", sch.play_id);
    scanf("%d", &sch.play_id);
    printf("input new studio ID（pre：%d）：", sch.studio_id);
    scanf("%d", &sch.studio_id);
    printf("input new count of seat（pre：%d）：", sch.seat_count);
    scanf("%d", &sch.seat_count);
    printf("input new Release date（pre：%d %d %d）：", sch.date.year, sch.date.month, sch.date.day);
    scanf("%d %d %d", &sch.date.year, &sch.date.month, &sch.date.day);
    printf("input new Show time（pre：%d %d %d）：", sch.time.hour, sch.time.minute, sch.time.second);
    scanf("%d %d %d", &sch.time.hour, &sch.time.minute, &sch.time.second);

    int result = Schedule_Srv_Modify(&sch);
    if (result == 1) {
        printf("modify successful！\n");
    }
    else {
        printf(" modify faild！\n");
    }
    return result;
}

int Schedule_UI_Delete(int id) {
    schedule_t sch;
    if (!Schedule_Srv_FetchById(&sch, id)) {
        printf("No performance plan with ID %d found！\n", id);
        return 0;
    }

    printf("\n==================== Delete the performance plan ====================\n");
    printf("Are you sure you want to delete the following performance plan?？\n");
    printf("schedule ID：%d\n", sch.id);
    printf("play ID：%d\n", sch.play_id);
    printf("studiio ID：%d\n", sch.studio_id);
    printf("count of seat：%d\n", sch.seat_count);
    printf("Release date：%4d-%2d-%2d\n", sch.date.year, sch.date.month, sch.date.day);
    printf("Show time：%2d-%2d-%2d\n", sch.time.hour, sch.time.minute, sch.time.second);
    printf("----------------------------------------------------\n");
    printf("Enter 'y' to confirm deletion, enter anything else to cancel : ");
    char confirm;
    scanf(" %c", &confirm);

    if (confirm != 'y' && confirm != 'Y') {
        printf("  The deletion operation has been canceled!\n");
        return 0;
    }

    int result = Schedule_Srv_DeleteByID(id);
    if (result == 1) {
        printf("  The performance plan has been deleted successfully.\n");
    }
    else {
        printf("  演出计划删除失败！\n");
    }
    return result;
}