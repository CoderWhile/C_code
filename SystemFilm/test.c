#define _CRT_SECURE_NO_WARNINGS 1
#include"UI.h"
#include"BL.h"
#include"DL.h"

int main()
{
	char input;
	int tmp;
	do
	{

		LogRegisterShow();
		scanf(" %c", &input);
		if (input < '0' || input>'9')
		{
			printf("输入错误\n");
			continue;
		}
		tmp = input - '0';
		switch (tmp)
		{
		case 1:
			system("cls");
			RegisterShowUser();
			break;
		case 2:
			system("cls");
			char id[15];
			//如果登录成功就进入下一个界面买票
			if (LogShow(0,id))
			{
				printf("成功登录\n");
				//选购电影票
				UserSelect(id);

			}
			
			break;
		case 3:
			system("cls");
			RegisterShowMana();
			break;
		case 4:
			system("cls");
			char manid[15];
			if (LogShow(1,manid))
			{
				printf("成功登录\n");
				system("cls");
				ManagerSelect();
				
			}
			break;
		case 0:
			printf("退出系统\n");
			break;
		default:
			printf("输入错误\n");
			continue;
		}

	} while (tmp);
}