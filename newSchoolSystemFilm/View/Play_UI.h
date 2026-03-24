#pragma once
#pragma once
#include"../Service/Play.h"
#ifndef PLAY_UI_H
#define PLAY_UI_H

#include "../Service/Play.h" 
typedef struct Paging {
    int totalRecords;
    int pageSize;
    int currentPage;
    int maxPage;
} Paging;





void Play_UI_ShowPage(play_list_t list, Paging* paging); // 声明这个函数

//管理剧目界面
void Play_UI_MgtEntry(void);
//添加新剧目界面
int Play_UI_Add(void);
//修改剧目界面
int Play_UI_Modify(int id);
//删除剧目界面
int Play_UI_Delete(int id);
//查询剧目界面
int Play_UI_Query(void);
#endif
