#pragma once
#include"../Service/Ticket.h"
#include"../Service/play.h"


typedef enum
{
	SALE_SELL = 1,//��Ʊ
	SALE_REFOUND = -1//��Ʊ
}sale_type_t;
typedef struct
{
	long id;//���ۼ�¼id
	int user_id;//��ƱԱid
	int ticket_id;//Ʊid
	ttms_date_t date;//��������
	ttms_time_t time;//����ʱ��
	int  value;//Ʊ��
	sale_type_t type;//��������
}sale_t;

typedef struct sale_node
{
	sale_t data;
	struct sale_node* next;
	struct sale_node* prev;
}sale_node_t,*sale_list_t;

int Sale_Srv_Add(const sale_t* data);


int Ticket_Srv_Modify(const ticket_t* data);

int Ticket_Srv_FetchBySchID(ticket_list_t list, int ID);

ticket_node_t* Ticket_Srv_FetchBySeatID(ticket_list_t list, int seat_id);

