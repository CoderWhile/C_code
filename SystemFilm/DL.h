#pragma once
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<assert.h>

//使用者的信息包括管理者和观影者、
typedef struct peodata
{
	char id[15];
	long long password;
	int identifier;//身份标识，1表示管理员，0表示观影者
}PEODATA;
typedef struct AccountInfo
{
	PEODATA* peoinfo;
	struct AccountInfo* prev;
	struct AccountInfo* next;
}ATIO;

ATIO* InitByAccount();
ATIO* BuynewAccount(PEODATA* peo);
void InsertAccount(ATIO* phead, ATIO* pos);
void SaveAccount(ATIO* phead);
void Load(ATIO* phead);




