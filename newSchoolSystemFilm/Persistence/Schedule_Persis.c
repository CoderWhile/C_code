#define _CRT_SECURE_NO_WARNINGS 1
#include"../Service/Schedule.h"
#include"assert.h"
#include"../Common/List.h"
#include"stdio.h"
#include "../Persistence/EntityKey_Persist.h"	 
#include <stdlib.h>
#include<string.h>



static const char SCHEDULE_DATA_FILE[] = "Schedule.dat"; //演出厅文件名常量 
static const char SCHEDULE_DATA_TEMP_FILE[] = "ScheduleTmp.dat"; //演出厅临时文件名常量 
static const char SCHEDULE_KEY_NAME[] = "Schedule";
//根据剧目id载入演出计划
int Schedule_Perst_SelectByPlay(schedule_list_t list, int play_id)
{
	schedule_node_t* newNode;
	schedule_t data;
	int recCount = 0;

	assert(NULL != list);

	List_Free(list, schedule_node_t);

	FILE* fp = fopen(SCHEDULE_DATA_FILE, "rb");
	if (NULL == fp) { //文件不存在
		return 0;
	}

	while (!feof(fp)) {
		if (fread(&data, sizeof(schedule_t), 1, fp)) {
			if(data.play_id==play_id)
			{
				newNode = (schedule_node_t*)malloc(sizeof(schedule_node_t));
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
	}
	fclose(fp);
	return recCount;
}


//像演出计划末尾文件添加一条新的演出计划
int Schedule_Perst_Insert(schedule_t* data)
{
	assert(NULL != data);

	////以下是新设计方案方案添加的代码
	////以下是新设计方案方案添加的代码
	long key = EntKey_Perst_GetNewKeys(SCHEDULE_KEY_NAME, 1); //为新演出厅分配获取
	if (key <= 0)			//主键分配失败，直接返回
		return 0;
	data->id = key;		//赋给新对象带回到UI层
	////以上是新设计方案方案添加的代码
	////以上是新设计方案方案添加的代码
	FILE* fp = fopen(SCHEDULE_DATA_FILE, "ab");
	int rtn = 0;
	if (NULL == fp) {
		printf("Cannot open file %s!\n", SCHEDULE_DATA_FILE);
		return 0;
	}

	rtn = fwrite(data, sizeof(schedule_t), 1, fp);

	fclose(fp);
	return rtn;
}

//
int Schedule_Perst_Update(const schedule_t* data)
{
	assert(NULL != data);

	FILE* fp = fopen(SCHEDULE_DATA_FILE, "rb+");
	if (NULL == fp) {
		printf("Cannot open file %s!\n", SCHEDULE_DATA_FILE);
		return 0;
	}

	schedule_t buf;
	int found = 0;

	while (!feof(fp)) {
		if (fread(&buf, sizeof(schedule_t), 1, fp)) {
			if (buf.id == data->id) {
				fseek(fp, -((int)sizeof(schedule_t)), SEEK_CUR);
				fwrite(data, sizeof(schedule_t), 1, fp);
				found = 1;
				break;
			}

		}
	}
	fclose(fp);

	return found;
}

//根据Id删除演出计划
int Schedule_Perst_RemByID(int id)
{
	if (rename(SCHEDULE_DATA_FILE, SCHEDULE_DATA_TEMP_FILE) < 0) {
		printf("Cannot open file %s!\n", SCHEDULE_DATA_FILE);
		return 0;
	}

	FILE* fpSour, * fpTarg;
	fpSour = fopen(SCHEDULE_DATA_TEMP_FILE, "rb");
	if (NULL == fpSour) {
		printf("Cannot open file %s!\n", SCHEDULE_DATA_FILE);
		return 0;
	}

	fpTarg = fopen(SCHEDULE_DATA_FILE, "wb");
	if (NULL == fpTarg) {
		printf("Cannot open file %s!\n", SCHEDULE_DATA_TEMP_FILE);
		return 0;
	}


	schedule_t buf;

	int found = 0;
	while (!feof(fpSour)) {
		if (fread(&buf, sizeof(schedule_t), 1, fpSour)) {
			if (id == buf.id) {
				found = 1;
				continue;
			}
			fwrite(&buf, sizeof(schedule_t), 1, fpTarg);
		}
	}

	fclose(fpTarg);
	fclose(fpSour);

	//删除临时文件
	remove(SCHEDULE_DATA_TEMP_FILE);
	return found;
}

int Schedule_Perst_SelectAll(schedule_list_t list)
{
	schedule_node_t* newNode;
	schedule_t data;
	int recCount = 0;

	assert(NULL != list);

	List_Free(list, schedule_node_t);

	FILE* fp = fopen(SCHEDULE_DATA_FILE, "rb");
	if (NULL == fp) { //文件不存在
		return 0;
	}

	while (!feof(fp)) {
		if (fread(&data, sizeof(schedule_t), 1, fp)) {
			newNode = (schedule_node_t*)malloc(sizeof(schedule_node_t));
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
int Play_Perst_SelectByName(play_list_t list, char condt[])
{
	int recCount = 0;  // 符合条件的记录数
	FILE* fp = NULL;
	play_t data;  // 临时变量存储单条剧目记录

	// a) 判断文件是否存在
	fp = fopen("Play.dat", "rb");
	if (fp == NULL) {
		return 0;  // 文件不存在直接返回0
	}

	// b) 初始化链表为空
	list->next = NULL;
	list->prev = NULL;

	// c) 判断是否读到文件末尾
	while (!feof(fp)) {
		// d) 读取一条记录到临时变量
		if (fread(&data, sizeof(play_t), 1, fp) == 1) {
			// 判断剧目名称是否包含关键词
			if (strstr(data.name, condt) != NULL) {
				// 构造新结点并添加到链表尾部
				play_node_t* newNode = (play_node_t*)malloc(sizeof(play_node_t));
				newNode->data = data;
				newNode->next = NULL;
				newNode->prev = NULL;
				List_AddTail(list, newNode)
				// 尾插法添加到链表
				recCount++;  // 计数加1
			}
		}
	}
	fclose(fp);
	return recCount;
}