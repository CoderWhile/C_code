#ifndef SCHEDULE_PERSIST_H_
#define SCHEDULE_PERSIST_H_

#include "../Service/ShowPlan.h"

// 根据剧目ID从数据库载入演出计划链表
int Schedule_Perst_SelectByPlay(schedule_list_t* list, int play_id);

// 存储新演出计划到数据库（返回值：0成功，非0失败）
int Schedule_Perst_Insert(const schedule_t* data);

// 更新数据库中演出计划（返回值：0成功，非0失败）
int Schedule_Perst_Update(const schedule_t* data);

// 根据ID从数据库移除演出计划（返回值：0成功，非0失败）
int Schedule_Perst_RemByID(int id);

// 根据ID查询单条演出计划（补全）
int Schedule_Perst_SelectById(schedule_t* out_data, int id);

// 根据ID查询剧目（补全）
int Play_Perst_SelByID(play_t* out_data, int id);

// 根据剧目名称查询剧目（已存在）
int Play_Perst_SelectByName(play_list_t list, char condt[]);

#endif // SCHEDULE_PERSIST_H_