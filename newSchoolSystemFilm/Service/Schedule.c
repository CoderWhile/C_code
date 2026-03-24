#define _CRT_SECURE_NO_WARNINGS 1
#include"../Service/ShowPlan.h"
#include"../Persistence/Schedule_Persist.h"
int Schedule_Srv_FetchByPlay(schedule_list_t list, int play_id)
{
	int trn= Schedule_Perst_SelectByPlay(list, play_id);
	return trn;

}
int Schedule_Srv_Add(schedule_t* data)
{
	int trn = Schedule_Perst_Insert(data);
	return trn;

}
int Schedule_Srv_Modify(const schedule_t* data)
{
	int trn= Schedule_Perst_Update(data);
	return trn;
}
int Schedule_Srv_FetchAll(schedule_list_t list)
{
	int rtn = Schedule_Perst_SelectAll(list);
	return rtn;
}
int Play_Srv_FetchByName(play_list_t list, char condt[])
{
	int rtn = Play_Perst_SelectByName(list,condt);
	return rtn;
}