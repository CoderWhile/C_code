#define _CRT_SECURE_NO_WARNINGS 1

/*
* 版权所有（C），2007-2008，西安邮电大学
* 功能编号：TTMS_UC_01
* 文件名称：Studio_Persist.c
* 描述：演出厅数据持久化模块
* 作者：西安邮电大学
* 版本：v.1
* 日期：2015年4月22日
*/

#include "Studio_Persist.h"
#include "../Service/Studio.h"
#include "EntityKey_Persist.h"	 
#include "../Common/List.h"
#include <stdlib.h>
#include <stdio.h>
//#include<unistd.h>
#include"../View/Seat_UI.h"
#include"../Service/Seat.h"
#include <assert.h>


static const char STUDIO_DATA_FILE[] = "Studio.dat"; //�ݳ����ļ������� 
static const char STUDIO_DATA_TEMP_FILE[] = "StudioTmp.dat"; //�ݳ�����ʱ�ļ������� 
static const char STUDIO_KEY_NAME[] = "Studio"; //�ݳ��������� 

/*
函数名称：TTMS_SCU_Studio_Perst_Insert
函数功能：向数据文件中插入一条演出厅相关数据。
参数说明：data 为 studio_t 类型指针，指向要添加的数据的结构体变量。
返回值：int 类型，表示是否成功操作数据的标志。
*/

int Studio_Perst_Insert(studio_t *data) {	 
	assert(NULL!=data);

	long key = EntKey_Perst_GetNewKeys(STUDIO_KEY_NAME, 1); //Ϊ���ݳ��������ȡ
	if(key<=0)			
		return 0;
	data->id = key;		 


	FILE *fp = fopen(STUDIO_DATA_FILE, "ab");
	int rtn = 0;
	if (NULL == fp) {
		printf("Cannot open file %s!\n", STUDIO_DATA_FILE);
		return 0;
	}

	rtn = fwrite(data, sizeof(studio_t), 1, fp);
	for (int i = 1; i <= data->rowsCount; i++)
	{
		for (int j = 1; j <= data->colsCount; j++)
		{
			seat_t newSeat;
			newSeat.roomID = data->id;  //演出厅ID
			newSeat.row = i;  //行
			newSeat.column = j;  //列
			newSeat.status = SEAT_NONE;  //座位状态
			seat_node_t* newNode = (seat_node_t*)malloc(sizeof(seat_node_t));
			newNode->data = newSeat;
			if (Seat_Srv_Add(&newSeat)) {
				//新增成功：同步到链表
				//Seat_Srv_AddToSoftedList(list, newNode);
				//printf("Success: Seat added\n");
				//return 1;
			}
		}
	}
	

	fclose(fp);
	return rtn;
}

/*
函数名称：TTMS_SCU_Studio_Perst_Update
函数功能：更新文件中与某一演出相关的数据。
函数说明：data为studio_t类型指针，指向要更新的演出相关数据信息。
返 回 值：布尔类型，表示是否成功更新演出的标识。
*/
int Studio_Perst_Update(const studio_t * data) {
	assert(NULL!=data);

	FILE *fp = fopen(STUDIO_DATA_FILE, "rb+");
	if (NULL == fp) {
		printf("Cannot open file %s!\n", STUDIO_DATA_FILE);
		return 0;
	}

	studio_t buf;
	int found = 0;

	while (!feof(fp)) {
		if (fread(&buf, sizeof(studio_t), 1, fp)) {
			if (buf.id == data->id) {
				fseek(fp, -((int)sizeof(studio_t)), SEEK_CUR);
				fwrite(data, sizeof(studio_t), 1, fp);
				found = 1;
				break;
			}

		}
	}
	fclose(fp);

	return found;
}

/*
函数名称：TTMS_SCU_Studio_Perst_DeleteByID
函数功能：从数据文件中删除一条演出计划数据。
参数说明：第一个参数ID为整型，表示要删除数据的演出计划ID；第二个参数buf为studio_t指针，指向演出计划数据的指针。
返回值：整型，表示是否成功删除演出计划的标志。
*/
int Studio_Perst_DeleteByID(int ID) {

	//将原文件的内容完整地读取出来并写入到新文件中，不需要删除多余的说明文字

    //原文件的内容如下
	if(rename(STUDIO_DATA_FILE, STUDIO_DATA_TEMP_FILE)<0){
		printf("Cannot open file %s!\n", STUDIO_DATA_FILE);
		return 0;
	}

	FILE *fpSour, *fpTarg;
	fpSour = fopen(STUDIO_DATA_TEMP_FILE, "rb");
	if (NULL == fpSour ){
		printf("Cannot open file %s!\n", STUDIO_DATA_FILE);
		return 0;
	}

	fpTarg = fopen(STUDIO_DATA_FILE, "wb");
	if ( NULL == fpTarg ) {
		printf("Cannot open file %s!\n", STUDIO_DATA_TEMP_FILE);
		return 0;
	}


	studio_t buf;

	int found = 0;
	while (!feof(fpSour)) {
		if (fread(&buf, sizeof(studio_t), 1, fpSour)) {
			if (ID == buf.id) {
				found = 1;
				continue;
			}
			fwrite(&buf, sizeof(studio_t), 1, fpTarg);
		}
	}

	fclose(fpTarg);
	fclose(fpSour);


	remove(STUDIO_DATA_TEMP_FILE);
	return found;
}

/*
函数名称：TTMS_SCU_Studio_Perst_SelectByID
函数功能：从数据文件中查找一个演出计划的数据。
参数说明：第一个参数ID为整型，表示要查找的演出计划ID；第二个参数buf为studio_t指针，指向存放演出计划数据的指针。
返 回 值：整型，表示是否成功找到演出计划的标志。
*/
//检查id对应的文件中是否存在演出计划
int Studio_Perst_SelectByID(int ID, studio_t *buf) {
	assert(NULL!=buf);

	FILE *fp = fopen(STUDIO_DATA_FILE, "rb");
	if (NULL == fp) {
		return 0;
	}

	studio_t data;
	int found = 0;

	while (!feof(fp)) {
		if (fread(&data, sizeof(studio_t), 1, fp)) {
			if (ID == data.id) {
				*buf = data;
				found = 1;
				break;
			};

		}
	}
	fclose(fp);

	return found;
}

/*
函数名称：TTMS_SCU_Studio_Perst_SelAll
函数功能：从文件中读取所有演出数据到链表。
函数说明：list 为 studio_list_t 类型指针，指向数据链表的头指针。
返回值：成功时返回 1，并返回链表的节点数量
*/
int Studio_Perst_SelectAll(studio_list_t list) {
	studio_node_t *newNode;
	studio_t data;
	int recCount = 0;

	assert(NULL!=list);

	List_Free(list, studio_node_t);

	FILE *fp = fopen(STUDIO_DATA_FILE, "rb");
	if (NULL == fp) { 
		return 0;
	}

	while (!feof(fp)) {
		if (fread(&data, sizeof(studio_t), 1, fp)) {
			newNode = (studio_node_t*) malloc(sizeof(studio_node_t));
			if (!newNode) {
				printf(
						"Warning, Memory OverFlow!!!\n Cannot Load more Data into memory!!!\n");
				break;
			}
			newNode->data = data;
			List_AddTail(list, newNode);
			recCount++;
		}
	}
	fclose(fp);
	return recCount;
}
