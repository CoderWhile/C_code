#define _CRT_SECURE_NO_WARNINGS 1
#include<stdlib.h>
#include "Play.h"
#include"../Persistence/Play_Persist.h"
//#include"Play_UI.h"
//获取全部剧目服务
int Play_Srv_FetchAll(play_list_t list) {
	return Play_Perst_SelectAll(list);
}
//添加新剧目
int Play_Srv_Add(play_t* data) {
	return Play_Perst_Insert(data);
}
//修改剧目
int Play_Srv_Modify(const play_t* data) {
	return Play_Perst_Updata(data);
}
//删除剧目
int Play_Srv_DeleteByID(int id) {
	return Play_Perst_RemByID(id);
}
//获取剧目
int Play_Srv_FetchByID(int id, play_t* buf) {
	return Play_Perst_SelectByID(id, buf);
}