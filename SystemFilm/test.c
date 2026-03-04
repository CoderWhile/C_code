#define _CRT_SECURE_NO_WARNINGS 1
#include"UL.h"
#include"BL.h"
#include"DL.h"

int main()
{
	char input;
	do
	{
		LogRegisterShow();
		scanf(" %c", &input);
		if (input < '0' || input>'9')
		{
			printf("输入错误\n");
			continue;
		}
		int tmp = input - '0';
		switch (tmp)
		{
		case 1:
			system("cls");
			RegisterShow();
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		case 0:
			printf("退出系统\n");
			break;
		default:
			printf("输入错误\n");
			continue;
		}

	} while (input);
}