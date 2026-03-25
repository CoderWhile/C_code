#define _CRT_SECURE_NO_WARNINGS 1

/*
* Copyright(C), 2007-2008, XUPT Univ.	 
* File name: Main_Menu.c			 	 
* Description : TTMS ϵͳ���˵�	
* Author:   XUPT  		 
* Version:  v.1 	 
* Da
*/

#include "../View/Main_Menu.h"

#include <stdio.h>

#include "../View/Studio_UI.h"
#include "../View/Play_UI.h"
#include "../View/ShowQuery_UI.h"
#include "../View/Sale_UI.h"
#include "../View/Saleanalysis_UI.h"
#include "../View/Account_UI.h"

//ϵͳ���˵� 
void Main_Menu(void) {
	char choice;
	do { 
		printf("\n==================================================================\n");
		printf("**************** Theater Ticket Management System ****************\n");
		printf("[S]tudio Management.\n");
		printf("[P]lay Management.\n");
		printf("[T]icket Sale.\n");
		printf("[R]eturn Ticket.\n");
		printf("[Q]stasals\n");
		printf("Ra[n]king and Statistics.\n");
		printf("[A]ccount Management.\n");
		printf("[E]xist.\n");
		printf("\n==================================================================\n");
		printf("Please input your choice:");
		fflush(stdin);
		scanf(" %c", &choice);
		//choice = getchar();
		switch (choice) {
		case 'S':
		case 's':
			Studio_UI_MgtEntry();
			break;
		case 'P':
		case 'p':
			Play_UI_MgtEntry();
			break;
		case 'Q':
		case 'q':
			StaSales_UI_MgtEntry();

			break;
		case 'T':
		case 't':
			Sale_UI_MgtEntry();
			break;
		case 'R':
		case 'r':
			Sale_UI_ReturnTicket();
			break;
		case 'N':
		case 'n':
			SalesAnalysis_UI_MgtEntry();
			break;
		case 'A':
		case 'a':
			Account_UI_MgtEntry();
			break;
		}
	} while ('E' != choice && 'e' != choice);
}

