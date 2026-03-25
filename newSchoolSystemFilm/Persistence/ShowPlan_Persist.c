#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ShowPlan_Persist.h"
#include "../Service/ShowPlan.h"
#include"../Persistence/EntityKey_Persist.h"
#include "../Common/List.h"

static const char SCHEDULE_DATA_FILE[] = "Schedule.dat";
static const char SCHEDULE_DATA_TEMP_FILE[] = "ScheduleTmp.dat";
static const char SCHEDULE_KEY_NAME[] = "Schedule";

// 根据剧目id载入演出计划
int Schedule_Perst_SelectByPlay(schedule_list_t list, int play_id)
{
	schedule_node_t* newNode;
	schedule_t data;
	int recCount = 0;

	assert(NULL != list);

	List_Free(list, schedule_node_t);

	FILE* fp = fopen(SCHEDULE_DATA_FILE, "rb");
	if (NULL == fp) {
		return 0;
	}

	while (!feof(fp)) {
		if (fread(&data, sizeof(schedule_t), 1, fp)) {
			if (data.play_id == play_id)
			{
				newNode = (schedule_node_t*)malloc(sizeof(schedule_node_t));
				if (!newNode) {
					printf("Warning, Memory OverFlow!!!\n Cannot Load more Data into memory!!!\n");
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

// 向演出计划文件添加新的演出计划
int Schedule_Perst_Insert(schedule_t* data)
{
	assert(NULL != data);

	long key = EntKey_Perst_GetNewKeys(SCHEDULE_KEY_NAME, 1);
	if (key <= 0)
		return 0;
	data->id = key;

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

// 更新演出计划
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

// 根据Id删除演出计划
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
	remove(SCHEDULE_DATA_TEMP_FILE);
	return found;
}

// 获取所有演出计划
int Schedule_Perst_SelectAll(schedule_list_t list)
{
	schedule_node_t* newNode;
	schedule_t data;
	int recCount = 0;

	assert(NULL != list);

	List_Free(list, schedule_node_t); 

	FILE* fp = fopen(SCHEDULE_DATA_FILE, "rb");
	if (NULL == fp) {
		return 0;
	}

	while (!feof(fp)) {
		if (fread(&data, sizeof(schedule_t), 1, fp)) {
			newNode = (schedule_node_t*)malloc(sizeof(schedule_node_t));
			if (!newNode) {
				printf("Warning, Memory OverFlow!!!\n Cannot Load more Data into memory!!!\n");
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

// ✅ 补全：根据ID查询单条演出计划
int Schedule_Perst_SelectById(schedule_t* out_data, int id)
{
	FILE* fp = fopen(SCHEDULE_DATA_FILE, "rb");
	if (!fp) return 0;

	schedule_t buf;
	while (fread(&buf, sizeof(schedule_t), 1, fp) == 1) {
		if (buf.id == id) {
			memcpy(out_data, &buf, sizeof(schedule_t));
			fclose(fp);
			return 1;
		}
	}
	fclose(fp);
	return 0;
}

// ✅ 补全：根据剧目ID查询剧目
int Play_Perst_SelByID(play_t* out_data, int id)
{
	FILE* fp = fopen("Play.dat", "rb");
	if (!fp) return 0;

	play_t buf;
	while (fread(&buf, sizeof(play_t), 1, fp) == 1) {
		if (buf.id == id) {
			memcpy(out_data, &buf, sizeof(play_t));
			fclose(fp);
			return 1;
		}
	}
	fclose(fp);
	return 0;
}

// 修复：根据剧目名称查询剧目（补全分号）
int Play_Perst_SelectByName(play_list_t list, char condt[])
{
	int recCount = 0;
	FILE* fp = NULL;
	play_t data;

	fp = fopen("Play.dat", "rb");
	if (fp == NULL) {
		return 0;
	}

	list->next = NULL;
	list->prev = NULL;

	while (!feof(fp)) {
		if (fread(&data, sizeof(play_t), 1, fp) == 1) {
			if (strstr(data.name, condt) != NULL) {
				play_node_t* newNode = (play_node_t*)malloc(sizeof(play_node_t));
				newNode->data = data;
				newNode->next = NULL;
				newNode->prev = NULL;
				List_AddTail(list, newNode); // 修复：补充分号
				recCount++;
			}
		}
	}
	fclose(fp);
	return recCount;
}