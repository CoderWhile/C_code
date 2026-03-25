#pragma once
#ifndef SCHEDULE_UI_H_
#define SCHEDULE_UI_H_
#include "../Service/ShowPlan.h"
#include"../Common/List.h"
static const int SCHEDULE_PAGE_SIZE = 5;



void Schedule_UI_MgtEntry(int play_id);
int Schedule_UI_Add(int play_id);
int Schedule_UI_Modify(int id);
int Schedule_UI_Delete(int id);
void Schedule_UI_ListAll();
int Schedule_UI_Query();

#endif // SCHEDULE_UI_H_