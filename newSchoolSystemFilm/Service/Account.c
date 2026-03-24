#define _CRT_SECURE_NO_WARNINGS 1
#include"Account.h"
#include "../Persistence/Studio_Persist.h"
#include <stdlib.h>
#include <assert.h>
#include"../Persistence/Account_Persistence.h"
#include<string.h>
#include<stdio.h>
account_t g1_CurUser = { 0,USR_ANOMY,"anonymous","" };

void Account_Srv_InitSys()
{
	if (Account_Perst_CheckAccFile() == 1)
	{
		return;
	}
	account_t data_admin;
	strcpy(data_admin.username ,"admin");
	strcpy(data_admin.password, "123456");
	Account_Srv_Add(&data_admin);
}
int Account_Srv_Verify(char usrName[], char pwd[])
{
	account_t usr;
	int ifExist = Account_Perst_SelByName(usrName, &usr);
	if (!ifExist) {
		return 0;
	}
	if (strcmp(usr.password, pwd) != 0) {
		return 0;
	}

	// e) 将用户信息保存到全局变量
	g1_CurUser = usr;

	// f) 验证成功返回1
	return 1;
}
int Account_Srv_Add(const account_t* data)
{
	if (Account_Perst_Insert(data))
		return 1;
	return 0;
}
int Account_Srv_Modify(const account_t* data)
{
	if (Account_Perst_Update(data))
		return 1;
	return 0;
}
int Account_Srv_DeleteByID(int usrID)
{
	if (Account_Perst_RemByID(usrID))
		return 1;
	return 0;
}
int Account_Srv_FetchAll(account_list_t list)
{
	return(Account_Perst_SelectALL(list));
}
account_node_t* Account_Srv_FindByUsrName(account_list_t list, char usrName[])
{
	account_node_t* pos;
	pos = list->next;
	while (pos != list)
	{
		if (strcmp(pos->data.username, usrName) == 0)
		{
			return pos;
		}
		pos = pos->next;
	}
	return NULL;
}