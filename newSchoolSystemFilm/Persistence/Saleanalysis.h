// ǿ��������п��ܵ�ǰ������/�궨�壬�Ӹ�Դ��������ͳ�ͻ

// ����Ψһ��ͷ�ļ������꣬�����κγ�ͻ
#ifndef TTMS_BOX_OFFICE_SALE_ANALYSIS_H
#define TTMS_BOX_OFFICE_SALE_ANALYSIS_H

/*
* ������ţ�TTMS_UC_11
* File name: SalesAnalysis.h
* Description : ͳ��Ʊ���߼���ͷ�ļ�
*/

// �ṹ�嶨����ɺ�����������ͷ�ļ�
#include "../Service/Saleanalysis.h"
#include "../Service/Ticket.h"
#include "../Service/Sale.h"

// ��������
int SalesAnalysis_Srv_StaticSale(salesanalysis_list_t list);
void SalesAnalysis_Srv_SortBySale(salesanalysis_list_t list);
int Schedule_Srv_StatRevByPlay(int play_id, int* soldCount);
int Ticket_Srv_StatRevBySchID(int schedule_id, int* soldCount);
int Ticket_Srv_FetchBySchID(ticket_list_t list, int schedule_id);
int Sale_Srv_FetchByTicketID(int ticket_id, sale_t* sale);

#endif