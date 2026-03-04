#define _CRT_SECURE_NO_WARNINGS 1
#include"DL.h"
//账户的添加


//如果链表为空创建一个新的头节点
ATIO* InitByAccount()
{
	ATIO* PheadAccount = (ATIO*)malloc(sizeof(ATIO));
	if (PheadAccount == NULL)
	{
		perror("mallocaccount:");
	}
	PheadAccount->prev = PheadAccount;
	PheadAccount->next = PheadAccount;
	PheadAccount->peoinfo = NULL;
}
//创建一个新的结点，以账户信息作为参数
ATIO* BuynewAccount(PEODATA* peo)
{
	ATIO* NewAccount = (ATIO*)malloc(sizeof(ATIO));
	if (NewAccount == NULL)
	{
		perror("mallocaccount:");
	}
	NewAccount->peoinfo = (PEODATA*)malloc(sizeof(PEODATA));
	if (NewAccount->peoinfo == NULL)
	{
		perror("mallocaccountinfo:");
	}
	*(NewAccount->peoinfo) = *peo;
	NewAccount->next = NULL;
	NewAccount->prev = NULL;
}
//将新的结点插入账户链表的头部
void InsertAccount(ATIO* phead, ATIO* pos)
{
	//头节点是phead，就插入在phead的后面；
	pos->next = phead->next;
	phead->next = pos;
	pos->prev = phead;
	//将新的链表保存到文件中
}

//保存链表的信息到文件中
void SaveAccount(ATIO* phead)
{
	assert(phead);
	FILE* pf = fopen("account_information.txt", "w");
	if (pf == NULL)
	{
		perror("Saveaccount");
		return;
	}
	//将链表中的信息写入文件
	ATIO* cur = phead->next;
	while (cur != phead)
	{
		if(fprintf(pf,"%s %lld %d",cur->peoinfo->id,cur->peoinfo->password,cur->peoinfo->identifier)>0);
		cur = cur->next;
	}
	fclose(pf);
	pf = NULL;
	
}

//将文件中的信息加载到链表中
void Load(ATIO* phead)
{
	FILE* pf = fopen("account_information.txt", "r");
	if (pf == NULL)
	{
		perror("Load");
		return;
	}
	//读文件
	ATIO* cur = phead;
	PEODATA tmp;//临时变量
	tmp.password = 0;
	while (fscanf(pf,"%s %lld %d",tmp.id,&tmp.password,&tmp.identifier)>0)
	{
		ATIO* newaccount = (ATIO*)malloc(sizeof(ATIO));
		newaccount->peoinfo = (PEODATA*)malloc(sizeof(PEODATA));
		if (newaccount == NULL || newaccount->peoinfo == NULL)
		{
			perror("malloc faild");
			return;
		}
		//尾插创建链表
		*newaccount->peoinfo= tmp;
		cur->next = newaccount;
		newaccount->prev = cur;
		newaccount ->next = phead;
		phead->prev = newaccount;
		cur = newaccount;
	}
	fclose(pf);
	pf = NULL;
}

//对于一个账户登录时账号和密码是否在已存的数据里面
//对于链表的查
//若已存信息中存在账户且密码对应正确返回1，否则返回0
int FindAccount(PEODATA* peo, ATIO* phead)
{
	ATIO* cur = phead->next;
	while (cur != phead)
	{
		if (strcmp(cur->peoinfo->id, peo->id))
		{
			cur = cur->next;
			continue;
		}
		if (cur->peoinfo->password == peo->password)
		{
			return 1;
		}
	}
	return 0;
}