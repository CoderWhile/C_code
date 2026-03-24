#pragma once
#ifndef _TICKET_BOXOFFICE_UI_H
#define _TICKET_BOXOFFICE_UI_H


#include "../Service/Ticket.h"


// 生成演出票界面（对应教材 TTMS_SCU_Ticket_UI_MgtEnt）
void Ticket_UI_MgtEntry(int schedule_id);

// 查询演出票入口界面（对应教材 TTMS_SCU_Ticket_UI_Qry）
void Ticket_UI_Query(void);

// 显示演出票界面（对应教材 TTMS_SCU_Ticket_UI_ShowTicket）
int Ticket_UI_ShowTicket(int ticket_id);

#endif
