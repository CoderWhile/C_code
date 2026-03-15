#define _CRT_SECURE_NO_WARNINGS 1
#include"BL.h"
#include"DL.h"
#include"UI.h"

//登录注册页面
void LogRegisterShow()
{
	printf("\n");
	system("cls");
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
void RegisterShowUser()
{
	char tmpid[15];
	long long tmppassword;
	
	printf("***********************************************\n");
	printf("****************影院票务管理系统***************\n");
	printf("请输入账号（长度为1-15，只可包含大小写字母和数字）：\n");
	scanf("%s", tmpid);
	printf("请输入密码（必须为6位数字）\n");
	char*newpassword= GetPasswd();

	//scanf("%lld", &tmppassword);

	//检验是否符合要求
	//调用业务层函数并判断是否正确，进而在链表中添加数据
	if (CheckId(tmpid)&& CheckPass(newpassword))//id和密码都符合基本要求
	{
		RegisterUser(tmpid,newpassword);
	}
	else
	{
		printf("输入不符合要求请重新输入\n");
	}
}

void RegisterShowMana()
{
	char tmpid[15];
	long long tmppassword;
	printf("***********************************************\n");
	printf("****************影院票务管理系统***************\n");
	printf("请输入账号（长度为1-15，只可包含大小写字母和数字）：\n");
	scanf("%s", tmpid);
	printf("请输入密码（必须为6位数字）\n");
	char* newpassword = GetPasswd();
	//scanf("%lld", &tmppassword);
	if (strlen(newpassword) != 6)
	{
		printf("密码长度有问题\n");
		return 0;
	}
	//检验是否符合要求
	//调用业务层函数并判断是否正确，进而在链表中添加数据
	if (CheckId(tmpid) && CheckPass(newpassword))//id和密码都符合基本要求
	{
		RegisterMana(tmpid, newpassword);
	}
	else
	{
		printf("输入不符合要求请重新输入\n");
	}
}

//用于登录界面
int LogShow(int flag,char *id)
{
	char tmpid[15];
	long long tmppassword;
	for(int i=0;i<3;i++)
	{
		printf("***********************************************\n");
		printf("****************影院票务管理系统***************\n");
		printf("请输入账号（长度为1-15，只可包含大小写字母和数字）：\n");
		scanf("%s", tmpid);
		strcpy(id, tmpid);
		printf("请输入密码（必须为6位数字）\n");
		//scanf("%lld", &tmppassword);
		char* newpassword = GetPasswd();
		//检验是否符合要求
		//调用业务层函数并判断是否正确，进而在链表中添加数据
		if (CheckId(tmpid) && CheckPass(newpassword))//id和密码都符合基本要求
		{
	
			if(flag==0)
			{
				if (CheckAccountInfileUser(tmpid, newpassword))
				{
					//登录成功
					return 1;
				}
			}
			else
			{
				if (CheckAccountInfileMana(tmpid, newpassword))
				{
					//登录成功
					return 1;
				}
			}
			
				printf("账号或密码错误重新输入：\n");
			
		}
		else
		{
			printf("输入不符合要求请重新输入\n");
		}
	}
	printf("警告：！！！！多次错误！！！！\n");
	return 0;
}

//管理者操作界面
void ManagerControl()
{
	printf("***********************************************\n");
	printf("****************影院票务管理系统***************\n");
	printf("1.查看电影场次\n");
	printf("2.增加电影场次\n");
	printf("3.删除电影场次\n");
	printf("4.修改电影场次信息\n");
	printf("5.查看电影介绍\n");
	printf("6.增加电影介绍\n");
	printf("7.查看票房信息\n");
	printf("8.管理使用者账户信息\n");
	printf("9.管理影厅\n");
	printf("0.后退\n");
	printf("***********************************************\n");
	printf("***********************************************\n");
}
//管理者选择界面
void ManagerSelect()
{
	char input;
	int tmp;
	do
	{
		ManagerControl();
		scanf(" %c", &input);
		if (input < '0' || input>'9')
		{
			printf("输入错误\n");
			continue;
		}
		tmp = input - '0';
		switch (tmp)
		{
		case 1://查看电影场次
			system("cls");
			ViewShowTime();

			break;
		case 2:
			system("cls");//增加电影场次
			AddShow();

			break;
		case 3://删除电影场次
			system("cls");
			PopShow();
			break;
		case 4://修改电影场次
			system("cls"); 
			ModifyShow();
			break;
		case 5://查看电影介绍
			system("cls");
			ShowFilmInfo();
			break;
		case 6://增加电影介绍
			system("cls");
			AddFilmInfo();
			break;
		case 7://查看票房信息
			system("cls");
			FilmSaleShow();

			break;
		case 8://管理使用者账户信息
			system("cls");
			MangeUserAccount();
			break;
		case 9://管理影厅
			system("cls");
			HallSelect();

			break;
		case 0:
			
			break;
		default:
			printf("输入错误\n");
			continue;
		}
	} while (tmp);
}

void UserAccountcontrolShow()
{
	printf("***********************************************\n");
	printf("****************影院票务管理系统***************\n");
	printf("1.查看影院所有用户账户信息\n");
	printf("2.注销用户账户\n");
	printf("0.后退\n");
	printf("***********************************************\n");
	printf("***********************************************\n");
}
void MangeUserAccount()
{
	char input;
	int tmp;
	do
	{
		UserAccountcontrolShow();
		scanf(" %c", &input);
		if (input < '0' || input>'9')
		{
			printf("输入错误\n");
			continue;
		}
		tmp = input - '0';
		switch (tmp)
		{
		case 1://查看影院所有用户账户信息
			system("cls");
			UserAccouttraverse();
			break;
		case 2:
			system("cls");//注销用户账户
			LogoutUser();
			break;
		case 0:
			break;
		default:
			printf("输入错误\n");
			continue;
		}
	} while (tmp);
}
//添加电影场次的显示
void AddShow()
{
	ViewShowTime();
	printf("请选择要添加场次的位置（序号）\n");
	//判断序号是否合法
	//写个函数
	//————————————————————————————————————————————————

	
	//获取电影场次信息--BL层获取
	//
	NewShowTime();

}
void PopShow()
{
	printf("请选择要删除的场次的编号\n");
	Popreceive();
}
void ModifyShow()
{
	printf("请选择要修改的场次的编号\n");
	Modifyreceive();
}

void HallContrlShow()
{
	printf("1.增加影厅\n");
	printf("2.删除影厅\n");
	printf("3.维修影厅座位\n");
	printf("0.后退\n");
}
void ModifyControlShow()
{
	
	printf("***********************************************\n");
	printf("****************影院票务管理系统***************\n");
	printf("选择修改信息\n");
	printf("1.场次时间\n");
	printf("2.场次电影\n");
	printf("3.场次价格\n");
	printf("4.场次座位售卖情况\n");
	printf("5.场次是否放映\n");
	printf("0.后退\n");
	printf("***********************************************\n");
	printf("***********************************************\n");
}

//用户选择界面
void UserSelectShow()
{
	printf("***********************************************\n");
	printf("****************影院票务管理系统***************\n");
	printf("1.购票\n");
	printf("2.查看已购订单\n");
	printf("3.影片信息查询\n");
	printf("4.退票\n");
	printf("5.搜索电影\n");
	printf("0.后退\n");
	printf("***********************************************\n");
	printf("***********************************************\n");

}
//用户选择界面
void UserSelect(char*id)
{
	
	char input;
	int tmp;
	do
	{
		UserSelectShow();
		scanf(" %c", &input);
		if (input < '0' || input>'9')
		{
			printf("输入错误\n");
			continue;
		}
		tmp = input - '0';
		switch (tmp)
		{
		case 1://购票
			system("cls");
			Buyticket(id);

			break;
		case 2:
			system("cls");//查看已购订单
			PersonalTicketShow(id);


			break;
		case 3://影片信息查询
			system("cls");
			ShowFilmInfo();
			break;
		case 4://退票
			system("cls");
			ticketRefund(id);
			break;
		case 5://模糊搜索电影
			system("cls");
			FuzzySearchFilm();
			break;

		case 0:
			
			break;
		default:
			printf("输入错误\n");
			continue;
		}
	} while (tmp);
}

//用户选择界面
void HallSelect()
{
	char input;
	int tmp;
	do
	{
		HallContrlShow();
		scanf(" %c", &input);
		if (input < '0' || input>'9')
		{
			printf("输入错误\n");
			continue;
		}
		tmp = input - '0';
		switch (tmp)
		{
		case 1://增加影厅
			system("cls");
			AddHall();
			break;
		case 2:
			system("cls");//删除影厅
			PopHallnode();
			break;
		case 3://修改影厅座位
			system("cls");
			Modifyhallseat();
			break;
		case 0:
			break;
		default:
			printf("输入错误\n");
			continue;
		}
	} while (tmp);
}