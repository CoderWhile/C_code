#define _CRT_SECURE_NO_WARNINGS 1

/*
* Copyright(C), 2007-2008, XUPT Univ.	
* ������ţ�TTMS_UC_01	 
* File name: Studio_UI.c			  
* Description : �ݳ������������	
* Author:   XUPT  		 
* Version:  v.1 	 
* Date: 	2015��4��22��	
*/

#include "../View/Studio_UI.h"

#include "../Common/List.h"
#include "../Service/Studio.h"
#include "../Service/Seat.h"
#include"../View/Seat_UI.h"


static const int STUDIO_PAGE_SIZE = 5;

#include <stdio.h>

/*
函数名称：TTMS_SCU_Studio_Perst_SelAll
函数功能：从文件中读取所有演出数据到链表。
函数说明：list 为 studio_list_t 类型指针，指向数据链表的头指针。
返回值：成功时返回 1，并返回链表的节点数量
*/
void Studio_UI_MgtEntry(void) {
	int i, id;
	char choice;

	studio_list_t head;
	studio_node_t *pos;
	Pagination_t paging;

	List_Init(head, studio_node_t);
	paging.offset = 0;
	paging.pageSize = STUDIO_PAGE_SIZE;

	//��������
	paging.totalRecords = Studio_Srv_FetchAll(head);
	Paging_Locate_FirstPage(head, paging);

	do {
		printf(
				"\n==================================================================\n");
		printf(
				"********************** Projection Room List **********************\n");
		printf("%5s  %18s  %10s  %10s  %10s\n", "ID", "Name", "Rows Count",
				"Columns Count", "Seats Count");
		printf(
				"------------------------------------------------------------------\n");
		//��ʾ����
		Paging_ViewPage_ForEach(head, paging, studio_node_t, pos, i){
			printf("%5d  %18s  %10d  %10d  %10d\n", pos->data.id,
					pos->data.name, pos->data.rowsCount, pos->data.colsCount,
					pos->data.seatsCount);
		}

		printf(
					"------- Total Records:%2d ----------------------- Page %2d/%2d ----\n",
					paging.totalRecords, Paging_CurPage(paging),
					Paging_TotalPages(paging));
		printf(
				"******************************************************************\n");
		printf(
				"[P]revPage|[N]extPage | [A]dd|[D]elete|[U]pdate | [S]eat | [R]eturn");
		printf(
				"\n==================================================================\n");
		printf("Your Choice:");
		fflush(stdin);
		scanf(" %c", &choice);
		fflush(stdin);

		switch (choice) {
		case 'a':
		case 'A':
			if (Studio_UI_Add()) //�����ӳɹ����������һҳ��ʾ
			{
				paging.totalRecords = Studio_Srv_FetchAll(head);
				Paging_Locate_LastPage(head, paging, studio_node_t);
			}
			break;
		case 'd':
		case 'D':
			printf("Input the ID:");
			scanf("%d", &id);
			if (Studio_UI_Delete(id)) {	//������������
				paging.totalRecords = Studio_Srv_FetchAll(head);
				List_Paging(head, paging, studio_node_t);
			}
			break;
		case 'u':
		case 'U':
			printf("Input the ID:");
			scanf("%d", &id);
			if (Studio_UI_Modify(id)) {	//������������
				paging.totalRecords = Studio_Srv_FetchAll(head);
				List_Paging(head, paging, studio_node_t);
			}
			break;
		case 's':
		case 'S':
			printf("Input the ID:");
			scanf("%d", &id);
			Seat_UI_MgtEntry(id);
			paging.totalRecords = Studio_Srv_FetchAll(head);
			List_Paging(head, paging, studio_node_t);
			break;
		case 'p':
		case 'P':
			if (!Paging_IsFirstPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, -1, studio_node_t);
			}
			break;
		case 'n':
		case 'N':
			if (!Paging_IsLastPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, 1, studio_node_t);
			}
			break;
		}
	} while (choice != 'r' && choice != 'R');
	//�ͷ������ռ�
	List_Destroy(head, studio_node_t);
}

/*
标识说明TTMS_SCU_Studio_UI_Add
功能描述：向系统中添加一个新的数据字典数据。
参数说明：无。
返回值：布尔类型，成功返回数据字典的ID值
*/
int Studio_UI_Add(void) {
	studio_t rec;
	int newRecCount = 0;
	char choice;

	do {
		printf("\n=======================================================\n");
		printf("****************  Add New Projection Room  ****************\n");
		printf("-------------------------------------------------------\n");
		printf("Room Name:");
		fflush(stdin);
		//gets(rec.name);
		scanf("%s", rec.name);
		printf("Row Count of Seats:");
		scanf("%d", &(rec.rowsCount));
		printf("Column Count of Seats:");
		scanf("%d", &(rec.colsCount));
		rec.seatsCount = rec.colsCount*rec.rowsCount;
		printf("=======================================================\n");

		if (Studio_Srv_Add(&rec)) {
			newRecCount += 1;
			printf("The new room added successfully!\n");
		} else
			printf("The new room added failed!\n");
		printf("-------------------------------------------------------\n");
		printf("[A]dd more, [R]eturn:");
		fflush(stdin);
		scanf("%c", &choice);
	} while ('a' == choice || 'A' == choice);
	return newRecCount;
}

/*
标识说明TTMS_SCU_Studio_UI_Mod
函数功能：用于修改系统中某一数据字典的数据。
参数说明：id为输入参数，表示要修改的数据字典ID。
返回值：输出参数，表示是否成功修改数据字典的标志。
*/
int Studio_UI_Modify(int id) {
	studio_t rec;
	int rtn = 0;
	int newrow, newcolumn;
	seat_list_t list;
	int seatcount;

	/*Load record*/
	if (!Studio_Srv_FetchByID(id, &rec)) {
		printf("The room does not exist!\nPress [Enter] key to return!\n");
		getchar();
		return 0;
	}

	printf("\n=======================================================\n");
	printf("****************  Update Projection Room  ****************\n");
	printf("-------------------------------------------------------\n");
	printf("Room ID:%d\n", rec.id);
	printf("Room Name[%s]:", rec.name);
	fflush(stdin);
	gets(rec.name);

	List_Init(list, seat_node_t);
	seatcount = Seat_Srv_FetchByRoomID(list, rec.id);
	if (seatcount) {
		do {			//�����λ�ļ���������λ��Ϣ������µ����б������ǰ�󣬷�����������
			printf("Row Count of Seats should >= [%d]:", rec.rowsCount);
			scanf("%d", &(newrow));
			printf("Column Count of Seats should >= [%d]:", rec.colsCount);
			scanf("%d", &(newcolumn));
		} while (newrow < rec.rowsCount || newcolumn < rec.colsCount);
		rec.rowsCount = newrow;
		rec.colsCount = newcolumn;
		rec.seatsCount = seatcount;
	} else {
		printf("Row Count of Seats:");
		scanf("%d", &rec.rowsCount);
		printf("Column Count of Seats:");
		scanf("%d", &rec.colsCount);
		rec.seatsCount = 0;
	}

	printf("-------------------------------------------------------\n");

	if (Studio_Srv_Modify(&rec)) {
		rtn = 1;
		printf(
				"The room data updated successfully!\nPress [Enter] key to return!\n");
	} else
		printf("The room data updated failed!\nPress [Enter] key to return!\n");

	getchar();
	return rtn;
}

/*
标识说明TTMS_SCU_Studio_UI_Del
函数功能：用于删除系统中指定的一条数据记录。
参数说明：id为整数，代表要删除的数据记录ID。
返回值：整数，表示是否成功删除数据记录的标志。
*/
int Studio_UI_Delete(int id) {

	int rtn = 0;

	if (Studio_Srv_DeleteByID(id)) {
		//删除任务调度时间同时根据调度id删除对应文件夹中的对应文件
		if (Seat_Srv_DeleteAllByRoomID(id))
			printf("The seats of the room deleted successfully!\n");
		printf(
				"The room deleted successfully!\nPress [Enter] key to return!\n");
		rtn = 1;
	} else {
		printf("The room does not exist!\nPress [Enter] key to return!\n");
	}

	getchar();
	return rtn;
}

