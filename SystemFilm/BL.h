#pragma once
#include"UI.h"
#include"DL.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

char* GetPasswd(void);
int RegisterUser(char id[15], char* password);
int RegisterMana(char id[15], char* password);
int CheckPass(long long password);

int CheckId(char tmpid[15]);

int CheckAccountInfileUser(char id[15], char* password);
int CheckAccountInfileMana(char id[15], char* password);
void ViewShowTime();
//获取电影场次信息并搞一个新的电影场次结构体
void NewShowTime();
void Popreceive();
void Modifyreceive();
void ShowFilmInfo();
void AddFilmInfo();
void Buyticket(char* id);
void PersonalTicketShow(char* id);
void ticketRefund(char* id);
void AddFilmInfoInShowTime(char* name);
void FilmSaleShow();
FMSL* AddFilmSale(char*tmpname);
void RefundFilmSaleByname(char* name, STIO* pos);
void AddFilmSaleByname(char* name, STIO* pos);
void UserAccouttraverse();
void LogoutUser();
void AddHall();
void PopHallnode();
void Modifyhallseat();
int CheckTime(int month, int day, int hour, int minute);
void FuzzySearchFilm();
int runtime(Time session);
long long TimeToMinutes(int month, int day, int hour, int minute);
int CheckTimeConflict(Time session, STIO* phead);





