#pragma once
/*
* 用例编号：TTMS_UC_11
* File name: SalesAnalysis_Persist.h
* Description : 统计票房持久化层头文件
*/

#include "Saleanalysis.h"

//根据票id载入销售记录
int Sale_Perst_SelByTicketID(int ticket_id, sale_t* sale);