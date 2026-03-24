#pragma once
typedef enum
{
	USR_ANOMY = 0,   //匿名类型
	USR_CLERK = 1,     //销售员类型
	USR_MANG = 2,       //经理类型
	USR_ADMIN = 9         //系统管理员类型
}account_type_t;

typedef struct
{
	int id;         //用户id
	account_type_t type;     //用户 类型
	char username[30];      //用户名
	char password[30];    //用户密码
}account_t;

extern account_t g1_CurUser;

typedef struct account_node {
	account_t data;
	struct account_node* next;
	struct account_node* prev;
}account_node_t,*account_list_t;

void Account_Srv_InitSys();
int Account_Srv_Verify(char usrName[], char pwd[]);
int Account_Srv_Add(const account_t *data);
int Account_Srv_Modify(const account_t* data);
int Account_Srv_DeleteByID(int usrID);
int Account_Srv_FetchAll( account_list_t list);
account_node_t* Account_Srv_FindByUsrName(account_list_t list, char usrName[]);