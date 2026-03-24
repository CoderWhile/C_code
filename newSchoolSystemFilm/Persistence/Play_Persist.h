#pragma once
#ifndef PLAY_PERSIST_H_
#define PLAY_PERSIST_H_

#include"../Service/Play.h"


//载入全部剧目
int Play_Perst_SelectAll(play_list_t list);
//存储新剧目
int Play_Perst_Insert(play_t* data);
//更新剧目
int Play_Perst_Updata(const play_t* data);
//根据ID去除jum
int Play_Perst_RemByID(int id);
//根据ID载入剧目
int Play_Perst_SelectByID(int id, play_t* buf);
#endif
