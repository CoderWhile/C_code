#pragma once
#include "../View/Studio_UI.h"

#include "../Common/List.h"
#include"Schedule.h"
#include"../Service/Studio.h"
#include<stdio.h>
#include<stdlib.h>
#include"../Service/play.h"
#include"Ticket_UI.h"
static const int SCHEDULE_PAGE_SIZE = 5;



void Schedule_UI_MgtEntry(int play_id);
int Schedule_UI_Add(int play_id);
int Schedule_UI_Modify(int id);
int Schedule_UI_Delete(int id);
void Schedule_UI_ListAll();
int Schedule_UI_Query();

