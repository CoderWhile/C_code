#pragma once
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<assert.h>

//使用者的信息包括管理者和观影者、
typedef struct peodata
{
	char id[15];
	char password[10];
	int identifier;//身份标识，1表示管理员，0表示观影者
}PEODATA;
typedef struct AccountInfo
{
	PEODATA* peoinfo;
	struct AccountInfo* prev;
	struct AccountInfo* next;
}ATIO;


//把时间封装成一个结构体
typedef struct time
{
	int smonth;
	int sday;
	int shour;
	int sminute;

	int fmonth;
	int fday;
	int fhour;
	int fminute;
}Time;

//电影场次结构体
typedef struct showtime
{
	long long number;//场次编号十位数
	Time sessiom;
	char NameFilm[20];
	char seat[16][16];//电影厅就九排15座
	int row, col;//影厅多少排多少列
	float price;
	int status;//场次的放映状态1->已放映 0->未放映， 默认为0；
	char Hall[15];//放映厅
}ShowTime;

typedef struct showtimenode
{
	ShowTime* showtimeinfo;
	struct showtimenode* prev;
	struct showtimenode* next;
}STIO;//电影场次信息

//电影的相关介绍
typedef struct filminfo
{
	char name[30];//电影名字
	char introduction[150];//中文50字以内；简短的介绍
	char directior[30];//是个汉字以内，，导演名字
	int  status;//状态表示该电影当前是否上映0-》未上映  1->已上映
}FILMINFO;
typedef struct filminfonode
{
	FILMINFO* filminfo;
	struct filminfonode* prev;
	struct filminfonode* next;
}FMIO;

typedef struct personalticket
{
	char id[15];
	ShowTime showtime;//该场次信息
	//座位信息
	//几张票
	int row;//排
	int col;//列

}personalticket;
typedef struct personalticketnode
{
	personalticket* pertickinfo;
	struct personalticketnode* prev;
	struct personalticketnode* next;

}PERTICIO;

typedef struct filmSale
{
	char filmname[30];//按照电影名字统计票房
	int ticketnumber;//买出票数
	float totalmoney;//总票房数
}filmsale;
typedef struct filmSalenod
{
	filmsale* filmsaleinfo;
	struct filmSalenod* prev;
	struct filmSalenod* next;

}FMSL;

typedef struct HallInfo
{
	char seat[16][16];//影厅最大15*15
	char nameofhall[15];//影厅名字
	int row;//影厅多少排
	int col;//多少列
}HallInfo;
typedef struct Hall
{
	HallInfo* hallinfo;
	struct Hall* prev;
	struct Hall* next;
}Hallnode;


ATIO* InitByAccount();
ATIO* BuynewAccount(char id[15], char* password);
void InsertAccount(ATIO* phead, ATIO* pos);
void SaveAccount(ATIO* phead);
void Load(ATIO* phead);
int FindAccountUser(PEODATA* peo, ATIO* phead);
int FindAccountMana(PEODATA* peo, ATIO* phead);
void SaveFilmInfo(FMIO* phead);
void LoadFilmInfo(FMIO* phead);
FMIO* InitByFilm();
FMIO* FindFilmbyname(FMIO* phead, char* name);
FMIO* BuynewFilm(char name[30], char introduction[150], char directior[30]);
void InsertFilmInfo(FMIO* pos, FMIO* phead, FMIO* newfilm);

//对于电影场次创建一个头节点
STIO* InitByShowTime();
////创建一个新的结点，以电影场次信息作为参数
STIO* BuynewShowTime(ShowTime* showtimeinfo, Hallnode* hallnode);

//查找
//按照名字查找电影
//参数：链表头节点和电影名字
STIO* FilmFindByName(STIO* phead, char name[]);
//按照电影编号查找某个场次
STIO* FilmFindByNumber(STIO* phead, long long number);
//增加电影场次到某个位置
//增加电影场次到结点pos后面
void InsertShowTime(STIO* pos, STIO* phead, STIO* newshowtime);
//删除pos结点
void PopShowTime(STIO* phead, STIO* pos);
void LoadShowTime(STIO* phead);
void SaveShowTime(STIO* phead);
void InsertTicketInfo(PERTICIO* pos, PERTICIO* phead, PERTICIO* newticket);
PERTICIO* Buynewpersonalticket(personalticket* info);
PERTICIO* InitByPerTicio();
void LoadPerTickInfo(PERTICIO* phead);
void SavePerTicInfo(PERTICIO* phead);
STIO* FilmFindByIdNumber(PERTICIO* phead, long long number, char* id);
void PopPerTic(PERTICIO* phead, PERTICIO* pos);
FMSL* BuynewfilmSalenod(filmsale* info);
FMSL* InitByfilmSale();
void LoadFilmSale(FMSL* phead);
void SaveFilmSale(FMSL* phead);
void InsertFilmSale(FMSL* pos, FMSL* phead, FMSL* newnod);
FMSL* FindFilmSalebyname(FMSL* phead, char* name);
ATIO* FindAccountUerName(char* id, ATIO* phead);
void PopAccount(ATIO* phead, ATIO* pos);
Hallnode* InitByHall();
Hallnode* BuynewHall(HallInfo* hallinfo);
void LoadHall(Hallnode* phead);
void SaveHall(Hallnode* phead);
void PopHall(Hallnode* phead, Hallnode* pos);
void InsertHall(Hallnode* pos, Hallnode* phead, Hallnode* newnode);
Hallnode* FindHallbyname(Hallnode* phead, char* name);
STIO* FilmFindByHallName(STIO* phead, char name[]);
STIO* FilmFindByHallName2(STIO* phead, char name[]);
