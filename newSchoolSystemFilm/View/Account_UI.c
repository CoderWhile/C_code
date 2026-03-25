#define _CRT_SECURE_NO_WARNINGS 1
#include "../View/Studio_UI.h"
#include"../Service/Account.h"
#include"Account_UI.h"
#include "../Common/List.h"
#include "../Service/Studio.h"
#include "../Service/Seat.h"
#include"string.h"

static const int ACCOUNT_PAGE_SIZE = 5;


#include<stdio.h>

//const char* account_type_to_str(account_type_t type)
//{
//	switch (type)
//	{
//	case USR_ANOMY: return "匿名类型";
//	case USR_CLERK: return "销售员类型";
//	case USR_MANG:  return "经理类型";
//	case USR_ADMIN: return "系统管理员类型";
//	default:        return "未知类型";
//	}
//}
int SysLogin()
{
	Account_Srv_InitSys();
	for (int i = 0; i < 3; i++)
	{
		char usrname[30];
		char password[30];
		printf("请输入用户名：\n");
		scanf("%s", usrname);
		printf("请输入密码：\n");
		scanf("%s", password);
		if (Account_Srv_Verify(usrname, password));
		{
			return 1;
		}
		printf("用户名或密码错误\n");
	}
	printf("验证超过三次退出\n");
	return 0;
}

void Account_UI_MgtEntry()
{
	account_list_t head;
	account_node_t* pos;
	int i, id;
	char choice;
	char usrnametmp[30];
	Pagination_t paging;

	List_Init(head, account_node_t);
	paging.offset = 0;
	paging.pageSize = ACCOUNT_PAGE_SIZE;

	//载入数据
	paging.totalRecords = Account_Srv_FetchAll(head);
	Paging_Locate_FirstPage(head, paging);

	do {
		printf(
			"\n==================================================================\n");
		printf(
			"********************** Projection Room List **********************\n");
		printf("%5s  %10s  %10s %10s\n", "ID", "type", "UserName","password");
		printf(
			"------------------------------------------------------------------\n");
		//显示数据
		Paging_ViewPage_ForEach(head, paging, account_node_t, pos, i) {
			printf("%5d  %10d  %10s  %10s  \n", pos->data.id,
				pos->data.type,  pos->data.username, pos->data.password);
		}

		printf(
				"------- Total Records:%2d ----------------------- Page %2d/%2d ----\n",
				paging.totalRecords, Paging_CurPage(paging),
				Paging_TotalPages(paging));
		printf(
			"******************************************************************\n");
		printf(
			"[P]revPage|[N]extPage | [A]dd|[D]elete|[M]odify");
		printf(
			"\n==================================================================\n");
		printf("Your Choice:");
		fflush(stdin);
		scanf("%c", &choice);
		fflush(stdin);

		switch (choice) {
		case 'a':
		case 'A':
			if (Account_UI_Add(head)) //新添加成功，跳到最后一页显示
			{
				paging.totalRecords = Account_Srv_FetchAll(head);
				Paging_Locate_LastPage(head, paging, account_node_t);
			}
			break;
		case 'd':
		case 'D':
			printf("Input the username:");
			scanf("%s", usrnametmp);
			if (Account_UI_Delete(head,usrnametmp)) {	//从新载入数据
				paging.totalRecords = Studio_Srv_FetchAll(head);
				List_Paging(head, paging, account_node_t);
			}
			break;
		case 'm':
		case 'M':
			printf("Input the username:");
			scanf("%s", usrnametmp);
			if (Account_UI_Modify(head,usrnametmp)) {	//从新载入数据
				paging.totalRecords = Account_Srv_FetchAll(head);
				List_Paging(head, paging, account_node_t);
			}
			break;
		//case 'q':
		//case 'Q':
		//	printf("Input the username:");
		//	scanf("%s", usrnametmp);
		//	Account_UI_Query(head,usrnametmp);
		//	paging.totalRecords = Studio_Srv_FetchAll(head);
		//	List_Paging(head, paging, account_node_t)
		//		;
		//	break;
		case 'p':
		case 'P':
			if (!Paging_IsFirstPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, -1, account_node_t);
			}
			break;
		case 'n':
		case 'N':
			if (!Paging_IsLastPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, 1, account_node_t);
			}
			break;
		}
	} while (choice != 'r' && choice != 'R');
	//释放链表空间
	List_Destroy(head, account_node_t);
}
int Account_UI_Add(account_list_t list)
{
	account_t newUser;
	printf("\n=== 添加新系统用户 ===\n");
	printf("请输入新用户名: ");
	scanf("%s", newUser.username);
	printf("请输入新用户类型：\n");
	scanf("%d", &newUser.type);
	printf("请输入新密码: ");
	scanf("%s", newUser.password);
	if (Account_Srv_FindByUsrName(list, newUser.username) != NULL) {
		printf(" 错误：用户名 [%s] 已存在！\n", newUser.username);
		return 0; 
	}
	if (Account_Srv_Add(&newUser) == 1) {
		printf("新用户添加成功！\n");

		account_node_t* newNode = (account_node_t*)malloc(sizeof(account_node_t));
		if (newNode == NULL) {
			printf(" 错误：内存分配失败\n");
			return 0;
		}
		newNode->data = newUser;
		newNode->next = NULL;
		newNode->prev = NULL;
		List_AddTail(list, newNode);
		return 1;
	}
}
int Account_UI_Modify(account_list_t list, char usrName[])
{
	account_node_t* cur = Account_Srv_FindByUsrName(list, usrName);
	if (cur == NULL)
	{
		printf("不存在该账户信息\n");
		return 0;
	}
	printf("请输入新密码\n");
	char newpassword[30];
	scanf("%s", cur->data.password);
	
	if (Account_Srv_Modify(&(cur->data)))
	{
		return 1;

	}
	return 0;
}
int Account_UI_Delete(account_list_t list, char usrName[])
{
	account_node_t* cur = Account_Srv_FindByUsrName(list, usrName);
	if (cur == NULL)
	{
		printf("不存在该账户信息\n");
		return 0;
	}
	if (Account_Srv_DeleteByID(cur->data.id))
	{
		printf("删除成功n");
		return 1;
	}
	List_FreeNode(cur);
}
int Account_UI_Query(account_list_t list, char usrName[])
{
	account_node_t* pos = Account_Srv_FindByUsrName(list, usrName);
	if (pos == NULL)
	{
		printf("不存在该账户信息\n");
		return 0;
	}
	printf("detail information:\n");
	printf("%5s  %10s  %10s %10s\n", "ID", "Name", "UserName", "password");
	printf(
		"------------------------------------------------------------------\n");
	//显示数据
		printf("%5d  %18s  %10s  %10s  \n", pos->data.id,
			pos->data.type, pos->data.username, pos->data.password);
		return 1;
}