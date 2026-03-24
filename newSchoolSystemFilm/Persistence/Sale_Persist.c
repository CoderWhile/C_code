#define _CRT_SECURE_NO_WARNINGS 1

#include"../Service/Ticket.h"
#include "../common/list.h"

#include"../Persistence/Sale_Persist.h"
#include "../Persistence/Studio_Persist.h"
#include "../Service/studio.h"
#include "../Persistence/EntityKey_Persist.h"	 
#include <stdlib.h>
#include <stdio.h>
//#include<unistd.h>
#include <assert.h>
#include "../common/list.h"


static const char SALE_DATA_FILE[] = "Sale.dat"; //演出厅文件名常量 
static const char SALE_DATA_TEMP_FILE[] = "SaleTmp.dat"; //演出厅临时文件名常量 
static const char SALE_KEY_NAME[] = "Sale"; //演出厅名常量 
static const char TICKET_DATA_FILE[] = "Ticket.dat"; //演出厅文件名常量 
static const char TICKET_DATA_TEMP_FILE[] = "Ticket.dat"; //演出厅临时文件名常量 
static const char TICKET_KEY_NAME[] = "Ticket"; //演出厅名常量 


//将参数所指的订单信息写入售票文件中
int Sale_Perst_Insert(const sale_t* data)
{
	assert(NULL != data);
	long key = EntKey_Perst_GetNewKeys(SALE_KEY_NAME, 1); //为新演出厅分配获取
	if (key <= 0)			//主键分配失败，直接返回
		return 0;
	sale_t sale = *data;
	sale.id = key;
		//赋给新对象带回到UI层
	////以上是新设计方案方案添加的代码
	////以上是新设计方案方案添加的代码
	FILE* fp = fopen(SALE_DATA_FILE, "ab");
	int rtn = 0;
	if (NULL == fp) {
		printf("Cannot open file %s!\n", SALE_DATA_FILE);
		return 0;
	}

	rtn = fwrite(&sale, sizeof(studio_t), 1, fp);

	fclose(fp);
	return rtn;
}


int Ticekt_Perst_Update(const ticket_t* data)
{
	assert(NULL != data);

	FILE* fp = fopen(TICKET_DATA_FILE, "rb+");
	if (NULL == fp) {
		printf("Cannot open file %s!\n", TICKET_DATA_FILE);
		return 0;
	}

	ticket_t rec;
	int found = 0;
	size_t read_ret;
	while ((read_ret=fread(&rec, sizeof(ticket_t), 1, fp))==1) {
			if (rec.id == data->id) {
				fseek(fp, -((int)sizeof(ticket_t)), SEEK_CUR);
				fwrite(data, sizeof(ticket_t), 1, fp);
				found = 1;
				break;
			}
	}
	fclose(fp);

	return found;
}

//根据演出计划id载入演出票的数据
int Ticket_Srv_SelBySchID(int id, ticket_list_t list)
{
		ticket_node_t* newNode;
		ticket_t data;
		int recCount = 0;

		assert(NULL != list);

		List_Free(list, ticket_node_t);

		FILE* fp = fopen(TICKET_DATA_FILE, "rb");
		if (NULL == fp) { //文件不存在
			return 0;
		}

		while (!feof(fp)) {
			if (fread(&data, sizeof(ticket_t), 1, fp)) {
				if (data.schedule_id == id)
				{
					newNode = (ticket_node_t*)malloc(sizeof(ticket_node_t));
					if (newNode == NULL) {
						printf(
							"Warning, Memory OverFlow!!!\n Cannot Load more Data into memory!!!\n");
						free(newNode);
						break;
					}

					newNode->data = data;
					List_AddTail(list, newNode);
					recCount++;
				}
			}
		}
		fclose(fp);
		return recCount;
}
