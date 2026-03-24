#pragma once
#include"Schedule.h"
//根据剧目id载入演出计划
int Schedule_Perst_SelectByPlay(schedule_list_t list, int play_id);
int Schedule_Perst_Insert(schedule_t* data);
int Schedule_Perst_Update(const schedule_t* data);
int Schedule_Perst_RemByID(int id);
int Schedule_Perst_SelectAll(schedule_list_t list);
int Play_Perst_SelectByName(play_list_t list, char condt[]);
