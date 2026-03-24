#define _CRT_SECURE_NO_WARNINGS 1
#include"../Service/Account.h"
#include "../persistence/Studio_Persist.h"
#include "../Service/studio.h"
#include "../persistence/EntityKey_Persist.h"	 
#include "../common/list.h"
#include <stdlib.h>
#include <stdio.h>
#include<string.h>
#include<Windows.h>
//#include<unistd.h>
#include"io.h"
#include <assert.h>


static const char ACCOUNT_DATA_FILE[] = "Account.dat"; //演出厅文件名常量 
static const char ACCOUNT_DATA_TEMP_FILE[] = "AccountTmp.dat"; //演出厅临时文件名常量 
static const char ACCOUNT_KEY_NAME[] = "Account"; //演出厅名常量 



int Account_Perst_CheckAccFile()
{
	if (_access(ACCOUNT_DATA_FILE, 0) == 0)
	{
		return 1;
	}
	return 0;
}
int Account_Perst_Insert(account_t* data)
{
	assert(NULL != data);

	////以下是新设计方案方案添加的代码
	////以下是新设计方案方案添加的代码
	long key = EntKey_Perst_GetNewKeys(ACCOUNT_KEY_NAME, 1); //为新演出厅分配获取
	if (key <= 0)			//主键分配失败，直接返回
		return 0;
	data->id = key;		//赋给新对象带回到UI层
	////以上是新设计方案方案添加的代码
	////以上是新设计方案方案添加的代码
	FILE* fp = fopen(ACCOUNT_DATA_FILE, "ab");
	int rtn = 0;
	if (NULL == fp) {
		printf("Cannot open file %s!\n", ACCOUNT_DATA_FILE);
		return 0;
	}

	rtn = fwrite(data, sizeof(account_t), 1, fp);

	fclose(fp);
	return rtn;
}
int Account_Perst_Update(account_t* data)
{
	assert(NULL != data);

	FILE* fp = fopen(ACCOUNT_DATA_FILE, "rb+");
	if (NULL == fp) {
		printf("Cannot open file %s!\n", ACCOUNT_DATA_FILE);
		return 0;
	}

	account_t buf;
	int found = 0;

	while (!feof(fp)) {
		if (fread(&buf, sizeof(account_t), 1, fp)) {
			if (buf.id == data->id) {
				fseek(fp, -((int)sizeof(account_t)), SEEK_CUR);
				fwrite(data, sizeof(account_t), 1, fp);
				found = 1;
				break;
			}

		}
	}
	fclose(fp);

	return found;
}
int Account_Perst_RemByID(int id) {

	//将原始文件重命名，然后读取数据重新写入到数据文件中，并将要删除的实体过滤掉。

	//对原始数据文件重命名
	if (rename(ACCOUNT_DATA_FILE, ACCOUNT_DATA_TEMP_FILE) < 0) {
		printf("Cannot open file %s!\n", ACCOUNT_DATA_FILE);
		return 0;
	}

	FILE* fpSour, * fpTarg;
	fpSour = fopen(ACCOUNT_DATA_TEMP_FILE, "rb");
	if (NULL == fpSour) {
		printf("Cannot open file %s!\n", ACCOUNT_DATA_TEMP_FILE);
		return 0;
	}

	fpTarg = fopen(ACCOUNT_DATA_FILE, "wb");
	if (NULL == fpTarg) {
		printf("Cannot open file %s!\n", ACCOUNT_DATA_FILE);
		return 0;
	}


	account_t buf;

	int found = 0;
	while (!feof(fpSour)) {
		if (fread(&buf, sizeof(account_t), 1, fpSour)) {
			if (id == buf.id) {
				found = 1;
				continue;
			}
			fwrite(&buf, sizeof(account_t), 1, fpTarg);
		}
	}

	fclose(fpTarg);
	fclose(fpSour);

	//删除临时文件
	remove(ACCOUNT_DATA_TEMP_FILE);
	return found;
}
//载入全部系统用户
int Account_Perst_SelectALL(account_list_t list)
{
	account_node_t* newNode;
	account_t data;
	int recCount = 0;

	assert(NULL != list);

	List_Free(list, account_node_t);

	FILE* fp = fopen(ACCOUNT_DATA_FILE, "rb");
	if (NULL == fp) { //文件不存在
		return 0;
	}

	while (!feof(fp)) {
		if (fread(&data, sizeof(account_t), 1, fp)) {
			newNode = (account_node_t*)malloc(sizeof(account_node_t));
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
int Account_Perst_SelByName(char *usrName, account_t*usr)
{
	FILE* fp = fopen("Account.dat", "rb");
	if (fp == NULL) {
		return 0; // 文件打开失败，直接返回未找到
	}

	account_t temp;
	// 逐条读取用户记录，匹配用户名
	while (fread(&temp, sizeof(account_t), 1, fp) == 1) {
		if (strcmp(temp.username, usrName) == 0) {
			// 找到匹配用户，拷贝到输出参数
			*usr = temp;
			fclose(fp);
			return 1;
		}
	}

	// 遍历完所有记录未找到
	fclose(fp);
	return 0;
}