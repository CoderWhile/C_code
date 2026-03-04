#define _CRT_SECURE_NO_WARNINGS 1
#include"BL.h"
#include"DL.h"


//对于id字符是否在15个字符以内进行检查
//返回1说明符合要求
int CheckId(PEODATA* peo)
{
	int flag = 0;
	char tmp[15];
	strcpy(tmp, peo->id);
	for (int i = 0; i < strlen(tmp); i++)
	{
		if (tmp[i] <= '9' && tmp[i] >= '0' || tmp[i] <= 'z' && tmp[i] >= 'a' || tmp[i] <= 'Z' && tmp[i] >= 'A')
		{
			flag = 0;
		}
		else
		{
			flag = 1;
			break;
		}
	}
	if (flag == 0)
		return 1;
	if (flag == 1)
		return 0;
}

//注册添加账户
int Register(PEODATA* peo)
{
	//直接添加
	ATIO* NewAccount = BuynewAccount(peo);
	//将账户添加到链表中
	//搞个头节点
	ATIO* plist = InitByAccount();
	//加载账户文件的信息
	Load(plist);

    InsertAccount(plist, NewAccount);
	//再保存到文件中
	SaveAccount(plist);
}

