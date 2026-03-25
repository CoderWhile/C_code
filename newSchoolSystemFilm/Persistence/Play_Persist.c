#define _CRT_SECURE_NO_WARNINGS 1
#include"../Service/Play.h"
#include"Play_Persist.h"
#include "../Persistence/EntityKey_Persist.h"	
#define FILE_NAME "Play.dat"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"../common/list.h"
#include<assert.h>

static const char PLAY_DATA_FILE[] = "Play.dat"; //
static const char PLAY_DATA_TEMP_FILE[] = "PlayTmp.dat"; //临时文件名常量 
static const char PLAY_KEY_NAME[] = "Play"; // 



//载入全部剧目
int Play_Perst_SelectAll(play_list_t list) {
	int recCount = 0;
	FILE* fp = fopen("Play.dat", "rb");
	if (fp == NULL) {
		return 0;
	}
	List_Free(list, play_node_t);
	play_t data;
	play_node_t* newNode;

	while (fread(&data, sizeof(play_t), 1, fp) == 1) {
		newNode = (play_node_t*)malloc(sizeof(play_node_t));
		newNode->data = data;
		List_AddTail(list, newNode);
		recCount++;
	}
	fclose(fp);
	return recCount;
}
//存储新剧目
int Play_Perst_Insert(play_t* data) {
	// 1. 校验入参合法性（避免空指针/非法ID导致添加失败）
	if (data == NULL) {
		printf("Error: Invalid play data (NULL pointer)!\n");
		return 0;
	}

	// 2. 保留用户输入的ID（不再强制覆盖），仅当ID≤0时自动分配
	long key = data->id;
	if (key <= 0) {
		key = EntKey_Perst_GetNewKeys(PLAY_KEY_NAME, 1); // 自动分配ID
		if (key <= 0) { // 主键分配失败，提示并返回
			printf("Error: Failed to allocate new play ID!\n");
			return 0;
		}
		data->id = key; // 赋值自动分配的ID
	}

	int retryCount = 3; // 最多重试3次
	FILE* fp = NULL;
	int rtn = 0;

	while (retryCount > 0) {
		fp = fopen("Play.dat", "ab"); // 以追加二进制模式打开
		if (fp != NULL) {
			// 4. 写入数据并校验写入结果
			rtn = fwrite(data, sizeof(play_t), 1, fp);
			if (rtn != 1) {
				printf("Error: Failed to write play data to file (retry %d times left)!\n", retryCount - 1);
			}
			else {
				break; // 写入成功，退出重试
			}
			fclose(fp);
			retryCount--;
		}
		else {
			printf("Error: Failed to open Play.dat (retry %d times left)!\n", retryCount - 1);
			retryCount--;
			// 短暂延迟后重试（可选，避免快速重试）
			// Sleep(500); // Windows下需包含 <windows.h>
		}
	}

	// 5. 最终清理和结果返回
	if (fp != NULL) {
		fclose(fp);
	}

	if (rtn == 1) {
		printf("Success: Play data inserted (ID: %ld)\n", data->id);
		return 1; // 成功返回1
	}
	else {
		printf("Error: Failed to insert play data after 3 retries!\n");
		return 0; // 失败返回0
	}
}
//更新剧目
int Play_Perst_Updata(const play_t* data) {
	int found = 0;
	play_t buf;
	FILE* fp = fopen("Play.dat", "rb+");
	if (fp == NULL) {
		printf("打开失败\n");
		return found;
	}
	while (fread(&buf, sizeof(play_t), 1, fp) == 1) {
		if (buf.id == data->id) {
			fseek(fp, -((int)sizeof(play_t)), SEEK_CUR);
			fwrite(data, sizeof(play_t), 1, fp);
			found = 1;
			break;
		}
	}
	fclose(fp);
	return found;
}
//根据ID去除剧目
int Play_Perst_RemByID(int id) {
	int found = 0;
	play_t buf;
	if (rename("Play.dat", "PlayTemp.dat") != 0) {
		printf("无法重命名 Play.dat 文件！\n");
		return found;
	}
	FILE* fpOld = fopen("PlayTemp.dat", "rb");
	FILE* fpNew = fopen("Play.dat", "wb");
	if (fpOld == NULL || fpNew == NULL) {
		printf("无法打开文件！\n");
		if (fpOld) fclose(fpOld);
		if (fpNew) fclose(fpNew);
		remove("Play.dat");
		if (rename("PlayTemp.dat", "Play.dat") != 0) {
			printf("恢复文件名失败\n");
		}
		return found;
	}
	while (fread(&buf, sizeof(play_t), 1, fpOld) == 1) {
		if (buf.id == id) {
			found = 1;
		}
		else {
			fwrite(&buf, sizeof(play_t), 1, fpNew);
		}
	}
	fclose(fpOld);
	fclose(fpNew);
	if (remove("PlayTemp.dat") != 0) {
		printf("Failed to delete PlayTemp.dat!\n");
	}
	return found;
}
//根据ID载入剧目
int Play_Perst_SelectByID(int id, play_t* buf) {
	int found = 0;
	play_t data;
	FILE* fp = fopen("Play.dat", "rb");
	if (fp == NULL) {
		printf("打开失败\n");
		return found;
	}
	while (fread(&data, sizeof(play_t), 1, fp) == 1) {
		if (data.id == id) {
			found = 1;
			*buf = data;
			break;
		}
	}
	fclose(fp);
	return found;
}