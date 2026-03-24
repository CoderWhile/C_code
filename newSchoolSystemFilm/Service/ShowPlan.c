//#define _CRT_SECURE_NO_WARNINGS 1
//#include "ShowPlan.h"
//
//#ifndef SCHEDULE_SRV_H_
//#define SCHEDULE_SRV_H_
//#include "../Common/List.h"
//#include "../Persistence/ShowPlan_Persist.h"
//
//#include <assert.h>
//#include <stdio.h>
//int Schedule_Srv_FetchByPlay(schedule_list_t list, int play_id)
//{
//	int trn = Schedule_Perst_SelectByPlay(list, play_id);
//	return trn;
//
//}
//int Schedule_Srv_Add(schedule_t* data)
//{
//	int trn = Schedule_Perst_Insert(data);
//	return trn;
//
//}
//int Schedule_Srv_Modify(const schedule_t* data)
//{
//	int trn = Schedule_Perst_Update(data);
//	return trn;
//}
//int Schedule_Srv_FetchAll(schedule_list_t list)
//{
//	int rtn = Schedule_Perst_SelectAll(list);
//	return rtn;
//}
//int Play_Srv_FetchByName(play[_list_t list, char condt[])
//{
//	int rtn = Play_Perst_SelectByName(list, condt);
//	return rtn;
//}
//#endif


#define _CRT_SECURE_NO_WARNINGS 1
#include "ShowPlan.h"

#ifndef SCHEDULE_SRV_H_
#define SCHEDULE_SRV_H_
#include "../Common/List.h"
#include "../Persistence/ShowPlan_Persist.h"

#include <assert.h>
#include <stdio.h>

// 根据剧目ID获取演出计划
int Schedule_Srv_FetchByPlay(schedule_list_t list, int play_id)
{
    int trn = Schedule_Perst_SelectByPlay(list, play_id);
    return trn;
}

// 新增演出计划
int Schedule_Srv_Add(schedule_t* data)
{
    int trn = Schedule_Perst_Insert(data);
    return trn;
}

// 修改演出计划
int Schedule_Srv_Modify(const schedule_t* data)
{
    int trn = Schedule_Perst_Update(data);
    return trn;
}

// 获取所有演出计划
int Schedule_Srv_FetchAll(schedule_list_t list)
{
    int rtn = Schedule_Perst_SelectAll(list);
    return rtn;
}

// 根据ID删除演出计划
int Schedule_Srv_DeleteByID(int id)
{
    int rtn = Schedule_Perst_RemByID(id);
    return rtn;
}

// 根据ID查询单条演出计划（只调用持久层，不自己操作文件）
int Schedule_Srv_FetchById(schedule_t* out_data, int id)
{
    return Schedule_Perst_SelectById(out_data, id);
}

// 根据剧目名称查询剧目
int Play_Srv_FetchByName(play_list_t list, char condt[])
{
    int rtn = Play_Perst_SelectByName(list, condt);
    return rtn;
}

#endif