#define _CRT_SECURE_NO_WARNINGS 1
#include"Sale_UI.h"
#include"../Service/Play.h"
#include "../View/Seat_UI.h"
#include "../Service/Seat.h"
#include"../Service/Schedule.h"
#include "../Service/Studio.h"
#include "../Common/List.h"
#include"../Common/common.h"
#include <stdio.h>
#include <ctype.h>

#define SALESANALYSIS_PAGE_SIZE 5

// ��Ŀ����ת�ַ���
const char* Play_Type_ToString(play_type_t type) {
	switch (type) {
	case PLAY_TYPE_FILE:    return "Movie";
	case PLAY_TYPE_OPEAR:  return "Opera";
	case PLAY_TYPE_CONCERT:return "Concert";
	default:               return "Unknown";
	}
}

// ��Ŀ�ּ�ת�ַ���
const char* Play_Rating_ToString(play_rating_t rating) {
	switch (rating) {
	case PLAY_RATE_CHILD:   return "Child";
	case PLAY_RATE_TEENAGE: return "Teenage";
	case PLAY_RATE_ADULT:   return "Adult";
	default:                return "Unknown";
	}
}

void Sale_UI_MgtEntry()
{
    int i, playid;
    int tmpid;
    play_t buf;
    int id;
    char choice;
    play_list_t head;
    play_node_t* pos;
    Pagination_t paging;

    // ��ʼ�������ͷ�ҳ����
    List_Init(head, play_node_t);
    paging.offset = 0;
    paging.pageSize = SALESANALYSIS_PAGE_SIZE;

    // �������о�Ŀ���ݣ���ȡ�ܼ�¼��
    paging.totalRecords = Play_Srv_FetchAll(head);
    // ��λ����һҳ
    Paging_Locate_FirstPage(head, paging);

    do {
        // ѭ��������Ч�ľ�ĿID
        do {
            // ��������ѡ���������飩
            system("cls"); // Windows������Linux/macOS��system("clear")
            // ��ӡ��ͷ
            printf("\n==================================================================\n");
            printf("**************************************** Play List ****************************************\n");
            printf("%5s %30s %10s %10s %8s %8s %12s %12s %8s\n",
                "ID", "Name", "Type", "Area", "Rating", "Duration", "Start Date", "End Date", "Price");
            printf("-------------------------------------------------------------------------------------------\n");

            // ��������ʾ��ǰҳ�ľ�Ŀ����
            Paging_ViewPage_ForEach(head, paging, play_node_t, pos, i) {
                printf("%5d %30s %10s %10s %8s %8d %04d-%02d-%02d %04d-%02d-%02d %8d\n",
                    pos->data.id,
                    pos->data.name,
                    Play_Type_ToString(pos->data.type),       // ö��ת�ַ���
                    pos->data.area,
                    Play_Rating_ToString(pos->data.rating),   // ö��ת�ַ���
                    pos->data.duration,
                    pos->data.start_date.year, pos->data.start_date.month, pos->data.start_date.day,
                    pos->data.end_date.year, pos->data.end_date.month, pos->data.end_date.day,
                    pos->data.price);
            }

            // ��ӡ��ҳ��Ϣ
            printf("------- Total Records:%2d ----------------------- Page %2d/%2d ----\n",
				paging.totalRecords, Paging_CurPage(paging), Paging_TotalPages(paging));

            // �����ĿID��У����Ч��
            printf("\nPlease input PlayId (input 0 to cancel): ");
            scanf("%d", &tmpid);
            // ȡ��������߼�
            if (tmpid == 0) break;

        } while (!Play_Srv_FetchByID(tmpid, &buf)); // ֱ��������Ч�ľ�ĿID

        // ����û�����0ȡ����ֱ����ʾ�˵�
        if (tmpid == 0) {
            // ���¶�λ����һҳ�����÷�ҳ��
            Paging_Locate_FirstPage(head, paging);
        }

        // ��ӡ���ܲ˵�
        printf("\n==================================================================\n");
        printf("[P]revPage | [N]extPage | [C]showScheduler | [S]earchPlay | [F]ilterByName | [R]eturn\n");
        printf("==================================================================\n");
        printf("Your Choice: ");

        // ������뻺���������scanf��ȡ�ַ�ʱ�Ĳ������⣩
        while (getchar() != '\n'); // ���fflush(stdin)����ƽ̨����ȫ
        scanf("%c", &choice);
        choice = toupper(choice); // ͳһתΪ��д�����ݴ�Сд����

        // �����û�ѡ��
        switch (choice) {
        case 'C': // �鿴�ݳ��ƻ�
            if (tmpid != 0) { // ������������Ч��ĿIDʱִ��
                Sale_UI_ShowScheduler(tmpid);
                // �����������ݣ�ȷ�����غ��������£�
                paging.totalRecords = Play_Srv_FetchAll(head);
                Paging_Locate_FirstPage(head, paging);
            }
            break;

        case 'F': // �����ƹ��˾�Ŀ
            Play_UI_UFetchByName(); // ���ð����Ʋ�ѯ����
            // ���¼����ҳ
            paging.totalRecords = Play_Srv_FetchAll(head);
            Paging_Locate_FirstPage(head, paging);
            break;

        case 'S': // ������Ŀ����F�߼����룬�ɸ������������
            Play_UI_UFetchByName();
            paging.totalRecords = Play_Srv_FetchAll(head);
            Paging_Locate_FirstPage(head, paging);
            break;

        case 'P': // ��һҳ
            if (!Paging_IsFirstPage(paging)) {
                Paging_Locate_OffsetPage(head, paging, -1, play_node_t);
            }
            else {
                printf("\n!!! Already on the first page !!!\n");
                system("pause"); // ��ͣ��ʾ
            }
            break;

        case 'N': // ��һҳ
            if (!Paging_IsLastPage(paging)) {
                Paging_Locate_OffsetPage(head, paging, 1, play_node_t);
            }
            else {
                printf("\n!!! Already on the last page !!!\n");
                system("pause"); // ��ͣ��ʾ
            }
            break;

        case 'R': // ����/�˳�
            printf("\nReturn to upper menu...\n");
            break;

        default: // ��Ч����
            printf("\n!!! Invalid choice, please try again !!!\n");
            system("pause");
            break;
        }

    } while (choice != 'R'); // ֱ������R�˳�

    // �ͷ������ڴ棬�����ڴ�й©
    List_Destroy(head, play_node_t);
}
void Sale_UI_ShowScheduler(int playID)
{
    int i;
    schedule_list_t sch_head = NULL;
    schedule_node_t* sch_pos = NULL;
    Pagination_t paging;
    char choice;
    int tmp_sch_id;
    schedule_t sch_buf;
    List_Init(sch_head, schedule_node_t);
    paging.offset = 0;
    paging.pageSize = SALESANALYSIS_PAGE_SIZE;
    paging.totalRecords = Schedule_Srv_FetchByPlay(sch_head, playID);

    if (paging.totalRecords == 0) {
        printf("\nThere are currently no performance plans for this play.\n");
        system("pause");
        List_Destroy(sch_head, schedule_node_t);
        return;
    }
    Paging_Locate_FirstPage(sch_head, paging);

    do {
        system("cls");
        printf("\n==================================================================\n");
        printf("************************Performance Schedule List (Play ID: %d) ************************\n", playID);
        printf("%8s %12s %10s %10s %10s %8s\n",
            "Plan ID", "Release date", "Release time", "studio ID", "playID", "Number of seats");
        printf("---------------------------------------------------------------------------\n");

        // 2. ��ҳ��ʾ�ݳ��ƻ���������Ľṹ�壩
        Paging_ViewPage_ForEach(sch_head, paging, schedule_node_t, sch_pos,i) {
            printf("%8d %04d-%02d-%02d %02d:%02d:%02d %10d %10d %8d\n",
                sch_pos->data.id,
                sch_pos->data.date.year, sch_pos->data.date.month, sch_pos->data.date.day,
                sch_pos->data.time.hour, sch_pos->data.time.minute, sch_pos->data.time.second,
                sch_pos->data.studio_id,
                sch_pos->data.play_id,
                sch_pos->data.seat_count);
        }

        printf("------- Total Records:%2d ----------------------- Page %2d/%2d ----\n",
				paging.totalRecords, Paging_CurPage(paging), Paging_TotalPages(paging));

        // 3. �����˵�
        printf("\n==================================================================\n");
        printf("[P]revPage | [N]revPage | [T]Ticket Management | [R]Return to the main interface\n");
        printf("==================================================================\n");
        printf("please input: ");

        while (getchar() != '\n');
        scanf("%c", &choice);
        choice = toupper(choice);

        switch (choice) {
        case 'P':
            if (!Paging_IsFirstPage(paging)) {
                Paging_Locate_OffsetPage(sch_head, paging, -1, schedule_node_t);
            }
            else {
                printf("\nIt's already the first page.\n");
                system("pause");
            }
            break;
        case 'N':
            if (!Paging_IsLastPage(paging)) {
                Paging_Locate_OffsetPage(sch_head, paging, 1, schedule_node_t);
            }
            else {
                printf("\nIt's already the last page.\n");
                system("pause");
            }
            break;
        case 'T':
            printf("\nPlease enter the ID of the performance plan to be managed: ");
            scanf("%d", &tmp_sch_id);
            if (Schedule_Srv_FetchByID(tmp_sch_id, &sch_buf)) {
                seat_list_t seat_head = NULL;
                ticket_list_t ticket_head = NULL;
                List_Init(seat_head, seat_node_t);
                List_Init(ticket_head, ticket_node_t);

                // ���ظó��ε���λ��Ʊ����
                Seat_Srv_FetchValidByRoomID(seat_head,sch_buf.studio_id);
                Ticket_Srv_FetchBySchID(ticket_head,tmp_sch_id);

                // ������Ʊ����
                Sale_UI_SellTicket(ticket_head, seat_head);

                // �ͷ���ʱ��Դ
                List_Destroy(seat_head, seat_node_t);
                List_Destroy(ticket_head, ticket_node_t);
            }
            else {
                printf("\nThe performance schedule was not found.\n");
                system("pause");
            }
            break;
        case 'R':
            printf("\nReturn the list of plays....\n");
            system("pause");
            break;
        default:
            printf("\nInvalid operation\n");
            system("pause");
            break;
        }
    } while (choice != 'R');

    // 4. �ͷ������ڴ�
    List_Destroy(sch_head, schedule_node_t);
}
int Sale_UI_SellTicket(ticket_list_t tickList, seat_list_t seatList) {
    int row, col;          // �������λ�кš��к�
    int seat_id = -1;     // ���ҵ�����λID
    ticket_t ticket;      // ���ҵ���Ʊ��Ϣ
    sale_t sale;          // ���ۼ�¼��������Ľṹ�嶨�壩

    // a) ������λ���кź��к�
    printf("Please enter the seat row number ");
    scanf("%d", &row);
    printf("Please enter the seat col number ");
    scanf("%d", &col);

    // b) ����Seat_Srv_FindByRC�������в�����λID
    seat_node_t*seatcur = Seat_Srv_FindByRowCol(seatList, row, col);
    if (seatcur==NULL) {
        printf("not found seat\n");
        return -1;
    }

    // c) ������λID���Ҷ�Ӧ��Ʊ
    if (!Ticket_Srv_FetchByID(seat_id, &ticket)) {
        printf("No ticket found for this seat\n");
        return -1;
    }

    // d) �ж�Ʊ�Ƿ�����
    if (ticket.status == TICKET_SOLD) {
        printf("This seat has been sold.\n");
        return -1;
    }

    // e) �޸�Ʊ��״̬Ϊ����
    ticket.status = TICKET_SOLD;

    Ticket_Srv_Modify(&ticket);

    // f) �������ۼ�¼sale���ϸ�����Ľṹ�帳ֵ��
    // �������л�ȡ��ǰ���ڡ�ʱ�䡢��ƱԱID�ĺ���
    sale.id = 0; // �¶�������ΨһID���ɵ���ID���ɺ�����
    sale.user_id = 1; // ʾ����ƱԱID��ʵ�ʿɴӵ�¼��Ϣ��ȡ
    sale.ticket_id = ticket.id;
    user_date_t date = DateNow();
    user_time_t time = TimeNow();


    sale.date.day = date.day;// ��ȡ��ǰ���ڣ�ttms_date_t���ͣ�
    sale.date.month = date.month;
    sale.date.year = date.year;
    sale.time.hour = time.hour; // ��ȡ��ǰʱ�䣨ttms_time_t���ͣ�
    sale.time.minute = time.minute;
    sale.time.second = time.second;
    sale.value = ticket.price; // ��Ʊʱvalue>0
    sale.type = SALE_SELL;     // ��������Ϊ��Ʊ

    // �洢���ۼ�¼
    Sale_Srv_Add(&sale);

    // g) ������λID
    return seat_id;
}

void Sale_UI_ReturnTicket()
{
    int row, col;          // �������λ�кš��к�
    int seat_id = -1;     // ���ҵ�����λID
    ticket_t ticket;      // ���ҵ���Ʊ��Ϣ
    sale_t sale;          // ���ۼ�¼��������Ľṹ�嶨�壩
    int ticket_id;

    //printf("This seat has been sold.\n");
    scanf("%d", &ticket_id);

    // c) ����ID���Ҷ�Ӧ��Ʊ
    if (!Ticket_Srv_FetchByID(seat_id, &ticket)) {
        printf("not fount ticket\n");
        return -1;
    }

    // d) �ж�Ʊ�Ƿ�����
    if (ticket.status == TICKET_SOLD) {
        printf("This seat has been sold.\n");
        return -1;
    }

    // e) �޸�Ʊ��״̬Ϊ����
    ticket.status = TICKET_AVL;

    Ticket_Srv_Modify(&ticket);

    //f) 根据销售明细记录构建索引值
// 需要获取当前的日期、时间、收银员 ID 和编号
    sale.id = 0; // �¶�������ΨһID���ɵ���ID���ɺ�����
    sale.user_id = 1; // ʾ����ƱԱID��ʵ�ʿɴӵ�¼��Ϣ��ȡ
    sale.ticket_id = ticket.id;
    user_date_t date = DateNow();
    user_time_t time = TimeNow();


    sale.date.day = date.day;// ��ȡ��ǰ���ڣ�ttms_date_t���ͣ�
    sale.date.month = date.month;
    sale.date.year = date.year;
    sale.time.hour = time.hour; // ��ȡ��ǰʱ�䣨ttms_time_t���ͣ�
    sale.time.minute = time.minute;
    sale.time.second = time.second;
    sale.value = ticket.price; // ��Ʊʱvalue>0
    sale.type = SALE_REFOUND;     // ��������Ϊ��Ʊ

    // �洢���ۼ�¼
    Sale_Srv_Add(&sale);

    // g) ������λID
    return seat_id;
}