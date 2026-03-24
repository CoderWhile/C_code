#define _CRT_SECURE_NO_WARNINGS 1
#include"Sale.h"
#include"../Persistence/Sale_Persist.h"
#include"Ticket.h"
#include<stdio.h>

int Sale_Srv_Add(const sale_t*data)
{
	Sale_Perst_Insert(data);
}

int Ticket_Srv_Modify(const ticket_t* data)
{
	return(Ticekt_Perst_Update(data));
}

//int Ticket_Srv_FetchBySchID(int ID, ticket_list_t list)
//{
//	return(Ticket_Srv_SelBySchID(ID, list));
//}
ticket_node_t* Ticket_Srv_FetchBySeatID(ticket_list_t list, int seat_id)
{
	ticket_node_t* cur;
	cur = list->next;
	while (cur != list)
	{
		if (seat_id == cur->data.id)
			return cur;
		cur = cur->next;

	}
	return NULL;
}