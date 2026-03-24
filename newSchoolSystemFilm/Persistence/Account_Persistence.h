#pragma once
#include"../Service/Account.h"

int Account_Perst_CheckAccFile();
int Account_Perst_Insert(account_t*data);
int Account_Perst_Update(account_t* data);
int Account_Perst_RemByID(int id);
int Account_Perst_SelectALL(account_list_t list);
int Account_Perst_SelByName(char* usrName, account_t* usr);




