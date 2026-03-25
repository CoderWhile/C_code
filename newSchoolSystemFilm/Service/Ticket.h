#pragma once
#pragma once
//#include"Ticket.h"
#include"ShowPlan.h"

typedef enum {
	TICKET_AVL = 0,//待售
	TICKET_SOLD = 1,//已售
	TICKET_RESV = 9//预留
}ticket_status_t;


typedef struct {
	int id;      //票id
	int schedule_id;  //演出计划id
	int seat_id;   //座位id
	int price;    //票价
	ticket_status_t status;   //票状态
}ticket_t;


typedef struct ticket_node {
	ticket_t data;
	struct ticket_node* next, * prev;
}ticket_node_t, * ticket_list_t;


// 1. 根据ID获取演出计划
int Schedule_Srv_FetchByID(int id, schedule_t* buf);

// 2. 批量生成演出票
void Ticket_Srv_GenBatch(int schedule_id, int stuID);

// 3. 批量删除演出票
int Ticket_Srv_DeleteBatch(int schedule_id);


// 5. 根据演出厅ID获取有效座位
//6nt Seat_Srv_FetchValidByRoomID(int room_id, seat_list_t list);

// 根据ID获取演出票（对应教材 TTMS_SCU_Ticket_Srv_FetchByID）
int Ticket_Srv_FetchByID(int id, ticket_t* buf);
