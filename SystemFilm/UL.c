#define _CRT_SECURE_NO_WARNINGS 1
#include"BL.h"
#include"DL.h"
#include"UL.h"

//登录注册页面
void LogRegisterShow()
{
	printf("***********************************************\n");
	printf("****************影院票务管理系统***************\n");
	printf("1.用户注册\n");
	printf("2.用户登录\n");
	printf("3.管理员注册\n");
	printf("4.管理员登录\n");
	printf("0.退出\n");
	printf("***********************************************\n");
	printf("***********************************************\n");
}

//用户选择注册注册界面
void RegisterShow()
{
	PEODATA tmp;
	tmp.identifier = 0;
	printf("请输入账号（长度为1-15，只可包含大小写字母和数字）：\n");
	scanf("%s", tmp.id);
	printf("请输入密码（10位以内的数字）\n");
	scanf("%lld", &tmp.password);
	//检验是否符合要求
	//调用业务层函数并判断是否正确，进而在链表中添加数据
	if (CheckId(&tmp))//符合要求
	{
		Register(&tmp);
	}
	else
	{
		printf("输入不符合要求请重新输入\n");
	}
}
