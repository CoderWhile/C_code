#define _CRT_SECURE_NO_WARNINGS 1
#include"Account.h"
#include"MaiAccount_UI.h"
#include<stdio.h>
void MaiAccount_UI_MgtEntry()
{
	account_t cdata = g1_CurUser;
	char cpassword[30];
	printf("ÇëÊäÈëĞÂµÄÃÜÂë£º\n");
	scanf("%s", cpassword);
	strcpy(cdata.password, cpassword);
	Account_Srv_Modify(&cdata);
}