#pragma once
#pragma once
#ifndef STASALES_H
#define STASALES_H
#include"play.h"
#include"Account.h"
#include"Sale.h"

//typedef enum {
//    SALE_SELL = 1,
//    SALE_REFOUND=-1
//}sale_type_t;



int StaSales_Srv_CompSaleVal(int usrID, ttms_date_t stDate, ttms_date_t endDate);
#endif
