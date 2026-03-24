#define _CRT_SECURE_NO_WARNINGS 1
/*
* Copyright(C), 2007-2008, XUPT Univ.	 
* File name: TTMS.c			  
* Description : Main in C, Ansi-style	
* Author:   XUPT  		 
* Version:  v.1 	 
* Date: 	2015年4月22日	
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <io.h>
#include "./View/Main_Menu.h"
#include <windows.h> 

//系统运行入口函数main 
int main(void) {
	SetConsoleOutputCP(CP_UTF8);
	// 也可以同时设置输入编码，避免输入中文乱码
	SetConsoleCP(CP_UTF8);
	setvbuf(stdout, NULL, _IONBF, 0);
	Main_Menu();
	return EXIT_SUCCESS;
}
