#define _CRT_SECURE_NO_WARNINGS 1
#define _CRT_SECURE_NO_WARNINGS
/*
* 用例编号：TTMS_UC_11
* File name: SalesAnalysis_Persist.h
* Description : 统计票房持久化层
*/

#include <stdio.h>
#include <string.h>
#include "../Service/Sale.h"

static const char SALE_DATA_FILE[] = "Sale.dat";

//根据票id载入销售记录
int Sale_Perst_SelByTicketID(int ticket_id, sale_t* sale) {
    int found = 0;
    sale_t data;
    FILE* fp = fopen(SALE_DATA_FILE, "rb");

    //初始化found，打开文件
    if (fp == NULL) {
        return found;
    }

    //循环读取文件直到末尾
    while (fread(&data, sizeof(sale_t), 1, fp) == 1) {
        //读取一条记录到data
        //判断是否匹配ticket_id
        if (data.ticket_id == ticket_id) {
            found = 1;
            memcpy(sale, &data, sizeof(sale_t));  //将匹配记录拷贝到输出参数
            break;
        }
    }

    fclose(fp);
    return found;
}