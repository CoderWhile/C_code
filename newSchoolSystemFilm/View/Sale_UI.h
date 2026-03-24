#pragma once
#include "../Common/list.h"
#include "../Service/Sale.h"
#include"../Service/Seat.h"

void Sale_UI_MgtEntry();
void Sale_UI_ShowScheduler(int playID);
int Sale_UI_SellTicket(ticket_list_t tickList, seat_list_t seatList);
void Sale_UI_ReturnTicket();

