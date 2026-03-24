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
	int rtn = 0;
	long key = EntKey_Perst_GetNewKeys(PLAY_KEY_NAME, 1); //为新演出厅分配获取
	if (key <= 0)			//主键分配失败，直接返回
		return 0;
	data->id = key;
	FILE* fp = fopen("Play.dat", "ab");
	if (fp == NULL) {
		printf("打开失败\n");
		return 0;
	}
	rtn = fwrite(data, sizeof(play_t), 1, fp);
	fclose(fp);
	return rtn;
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