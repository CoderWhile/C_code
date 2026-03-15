#define _CRT_SECURE_NO_WARNINGS 1
#include"DL.h"
//账户的添加


//如果链表为空创建一个新的头节点
ATIO* InitByAccount()
{
	ATIO* PheadAccount = (ATIO*)malloc(sizeof(ATIO));
	if (PheadAccount == NULL)
	{
		perror("mallocaccount:");
	}
	PheadAccount->prev = PheadAccount;
	PheadAccount->next = PheadAccount;
	PheadAccount->peoinfo = NULL;
	return PheadAccount;
}
//创建一个新的结点，以账户信息作为参数
ATIO* BuynewAccount(char id[15],char* password)
{
	ATIO* NewAccount = (ATIO*)malloc(sizeof(ATIO));
	if (NewAccount == NULL)
	{
		perror("mallocaccount:");
	}
	NewAccount->peoinfo = (PEODATA*)malloc(sizeof(PEODATA));
	strcpy(NewAccount->peoinfo->id, id);
	strcpy(NewAccount->peoinfo->password, password);
	if (NewAccount->peoinfo == NULL)
	{
		perror("mallocaccountinfo:");
	}
	NewAccount->next = NULL;
	NewAccount->prev = NULL;
	return NewAccount;
}
//将新的结点插入账户链表的头部
void InsertAccount(ATIO* phead, ATIO* pos)
{
	//头节点是phead，就插入在phead的后面；
	pos->next = phead->next;
	phead->next = pos;
	pos->prev = phead;
	//将新的链表保存到文件中
}

//保存链表的信息到文件中
void SaveAccount(ATIO* phead)
{
	assert(phead);
	FILE* pf = fopen("account_information.txt", "w");
	if (pf == NULL)
	{
		perror("Saveaccount");
		return;
	}
	//将链表中的信息写入文件
	ATIO* cur = phead->next;
	while (cur != phead)
	{
		if(fprintf(pf,"%s %s %d",cur->peoinfo->id,cur->peoinfo->password,cur->peoinfo->identifier)>0);
		cur = cur->next;
	}
	fclose(pf);
	pf = NULL;
	
}

//将文件中的信息加载到链表中
void Load(ATIO* phead)
{
	FILE* pf = fopen("account_information.txt", "r");
	if (pf == NULL)
	{
		perror("Load");
		return;
	}
	//读文件
	ATIO* cur = phead;
	PEODATA tmp;//临时变量
	
	while (fscanf(pf,"%s %s %d",tmp.id,tmp.password,&tmp.identifier)>0)
	{
		ATIO* newaccount = (ATIO*)malloc(sizeof(ATIO));
		newaccount->peoinfo = (PEODATA*)malloc(sizeof(PEODATA));
		if (newaccount == NULL || newaccount->peoinfo == NULL)
		{
			perror("malloc faild");
			return;
		}
		//尾插创建链表
		*newaccount->peoinfo= tmp;
		cur->next = newaccount;
		newaccount->prev = cur;
		newaccount ->next = phead;
		phead->prev = newaccount;
		cur = newaccount;
	}
	fclose(pf);
	pf = NULL;
}

//对于一个账户登录时账号和密码是否在已存的数据里面
//对于链表的查
//若已存信息中存在账户且密码对应正确返回1，否则返回0
int FindAccountUser(PEODATA* peo, ATIO* phead)
{
	ATIO* cur = phead->next;
	while (cur != phead)
	{
		if (strcmp(cur->peoinfo->id, peo->id))
		{
			cur = cur->next;
			continue;
		}
		if (strcmp(cur->peoinfo->password ,peo->password)==0&&cur->peoinfo->identifier==0)
		{
			return 1;
		}
		cur = cur->next;
	}
	return 0;
}
int FindAccountMana(PEODATA* peo, ATIO* phead)
{
	ATIO* cur = phead->next;
	while (cur != phead)
	{
		if (strcmp(cur->peoinfo->id, peo->id))
		{
			cur = cur->next;
			continue;
		}
		if (strcmp(cur->peoinfo->password,peo->password)==0 && cur->peoinfo->identifier == 1)
		{
			return 1;
		}
		cur = cur->next;
	}
	return 0;
}
//按照id查找某账户
ATIO* FindAccountUerName(char* id, ATIO* phead)
{
	ATIO* cur = phead->next;
	while (cur != phead)
	{
		if (strcmp(cur->peoinfo->id, id)==0 && cur->peoinfo->identifier == 0)
		{
			return cur;
		}
		cur = cur->next;
	}
	return	NULL;
}
//对于电影场次创建一个头节点
STIO* InitByShowTime()
{
	STIO* PheadShowTime = (STIO*)malloc(sizeof(STIO));
	if (PheadShowTime == NULL)
	{
		perror("mallocaShouTime:");
	}
	PheadShowTime->prev = PheadShowTime;
	PheadShowTime->next = PheadShowTime;
	PheadShowTime->showtimeinfo = NULL;
	return PheadShowTime;
}

////创建一个新的结点，以电影场次信息作为参数
STIO* BuynewShowTime(ShowTime* showtimeinfo,Hallnode*hallnode)
{
	STIO* NewShowTime= (STIO*)malloc(sizeof(STIO));
	if (NewShowTime == NULL)
	{
		perror("mallocshowinfo:");
	}
	NewShowTime->showtimeinfo = (ShowTime*)malloc(sizeof(ShowTime));
	if (NewShowTime->showtimeinfo == NULL)
	{
		perror("mallocaccountinfo:");
	}
	*(NewShowTime->showtimeinfo) = *showtimeinfo;
	NewShowTime->showtimeinfo->status = 0;//默认开始都为0；管理员更改是否放映的状态
	//将座位信息初始化
	//未售卖就是   '*'
	NewShowTime->showtimeinfo->row = hallnode->hallinfo->row;
	NewShowTime->showtimeinfo->col = hallnode->hallinfo->col;
	NewShowTime->showtimeinfo->seat[0][0] = 0;
	memcpy(NewShowTime->showtimeinfo->seat, hallnode->hallinfo->seat, 16 * 16 * sizeof(char));
	int j = 1;
	
	for (int i = 1; i <= hallnode->hallinfo->row; i++)
	{
		for (int j = 1; j <= hallnode->hallinfo->col; j ++)
		{
			if(hallnode->hallinfo->seat[i][j]=='@')
			NewShowTime->showtimeinfo->seat[i][j] ='*';
			else
				NewShowTime->showtimeinfo->seat[i][j] = '!';

		}
	}
	NewShowTime->next = NULL;
	NewShowTime->prev = NULL;
	return NewShowTime;
}

//查找
//按照名字查找电影
//参数：链表头节点和电影名字
STIO* FilmFindByName(STIO* phead, char name[])
{
	assert(phead);
	if (phead->next == phead)
	{
		printf("系统中没有电影\n");
		return NULL;
	}
	STIO* cur = phead->next;
	while (cur != phead)
	{
		if (strcmp(cur->showtimeinfo->NameFilm, name) == 0)
		{
			return cur;
		}
		else
		{
			cur = cur->next;
		}
	}
	printf("系统中没有该电影\n");
	return NULL;
}

//按照电影编号查找某个场次
STIO* FilmFindByNumber(STIO* phead, long long number)
{
	assert(phead);
	if (phead->next == phead)
	{
		//printf("系统中没有电影\n");
		return NULL;
	}
	STIO* cur = phead->next;
	while (cur != phead)
	{
		if (cur->showtimeinfo->number==number)
		{
			return cur;
		}
		else
		{
			cur = cur->next;
		}
	}
	printf("系统中没有该电影\n");
	return NULL;
}


//增加电影场次到某个位置
//增加电影场次到结点pos后面
void InsertShowTime(STIO* pos, STIO* phead,STIO*newshowtime)
{
	if (pos == NULL)
	{
		printf("添加场次失败\n");
		return;
	}
	
	STIO* prev = pos->prev;
	prev->next = newshowtime;
	newshowtime->next = pos;
	newshowtime->prev = prev;
	pos->prev = newshowtime;
	printf("增加成功\n");
}

//删除某个结点
//删除pos结点
void PopShowTime(STIO*phead,STIO*pos)
{
	assert(phead);
	if (phead->next == phead)
	{
		printf("系统中没有场次信息\n");
		return;
	} 

	STIO* cur = pos;
	STIO* prev = cur->prev;
	STIO* next = cur->next;
	prev->next = next;
	next->prev = prev;
}

//保存电影场次链表的信息到电影场次的文件中
//以二进制写
void SaveShowTime(STIO* phead)
{
	assert(phead);
	FILE* pf = fopen("showtime_information.txt", "wb");
	if (pf == NULL)
	{
		perror("Save");
		return;
	}
	//将链表中的信息写入文件
	STIO* cur = phead->next;
	//将电影场次信息写入
	while (cur != phead)
	{
		fwrite(cur->showtimeinfo, sizeof(ShowTime), 1, pf);
		cur = cur->next;
	}
	fclose(pf);
	pf = NULL;
}

//将电影场次的信息读到链表中
void LoadShowTime(STIO* phead)
{
	FILE* pf = fopen("showtime_information.txt", "rb");
	if (pf == NULL)
	{
		perror("LoadShowtime");
		return;
	}
	//读文件
	STIO* cur = phead;
	ShowTime tmp;//临时变量
	while (fread(&tmp, sizeof(ShowTime), 1, pf) == 1)
	{
		STIO* newnode = (STIO*)malloc(sizeof(STIO));
		newnode->showtimeinfo = (ShowTime*)malloc(sizeof(ShowTime));
		if (newnode == NULL || newnode->showtimeinfo == NULL)
		{
			perror("malloc faild");
			return;
		}
		*newnode->showtimeinfo = tmp;
		cur->next = newnode;
		newnode->prev = cur;
		newnode->next = phead;
		phead->prev = newnode;
		cur = newnode;
	}
	fclose(pf);
	pf = NULL;
}

//对电影信息
void SaveFilmInfo(FMIO* phead)
{
	assert(phead);
	FILE* pf = fopen("film_information.txt", "wb");
	if (pf == NULL)
	{
		perror("Save");
		return;
	}
	//将链表中的信息写入文件
	FMIO* cur = phead->next;
	//将电影信息写入
	while (cur != phead)
	{
		fwrite(cur->filminfo, sizeof(FILMINFO), 1, pf);
		cur = cur->next;
	}
	fclose(pf);
	pf = NULL;
}

//将电影信息读到链表中
void LoadFilmInfo(FMIO* phead)
{
	FILE* pf = fopen("film_information.txt", "rb");
	if (pf == NULL)
	{
		perror("Loadflim");
		return;
	}
	//读文件
	FMIO* cur = phead;
	FILMINFO tmp;//临时变量
	while (fread(&tmp, sizeof(FILMINFO), 1, pf) == 1)
	{
		FMIO* newnode = (FMIO*)malloc(sizeof(FMIO));
		newnode->filminfo = (FILMINFO*)malloc(sizeof(FILMINFO));
		if (newnode == NULL || newnode->filminfo == NULL)
		{
			perror("malloc faild");
			return;
		}
		*newnode->filminfo = tmp;
		cur->next = newnode;
		newnode->prev = cur;
		newnode->next = phead;
		phead->prev = newnode;
		cur = newnode;
	}
	fclose(pf);
}

//如果链表为空创建一个新的头节点
FMIO* InitByFilm()
{
	FMIO* PheadFilm = (FMIO*)malloc(sizeof(FMIO));
	if (PheadFilm == NULL)
	{
		perror("mallocfilm:");
	}
	PheadFilm->prev = PheadFilm;
	PheadFilm->next = PheadFilm;
	PheadFilm->filminfo = NULL;
	return PheadFilm;
}
//创建一个新的结点，以电影信息作为参数
FMIO* BuynewFilm(char name[30], char introduction[150],char directior[30])
{
	FMIO* NewFilm = (FMIO*)malloc(sizeof(FMIO));
	if (NewFilm == NULL)
	{
		perror("mallocfilm:");
	}
	NewFilm->filminfo = (FILMINFO*)malloc(sizeof(FILMINFO));
	if (NewFilm->filminfo == NULL)
	{
		perror("mallocaccountinfo:");
	}
	strcpy(NewFilm->filminfo->name,name);
	strcpy(NewFilm->filminfo->introduction, introduction);
	strcpy(NewFilm->filminfo->directior, directior);
	NewFilm->filminfo->status = 0;
	NewFilm->next = NULL;
	NewFilm->prev = NULL;
	return NewFilm;
}
//插入电影介绍的结点
void InsertFilmInfo(FMIO* pos, FMIO* phead, FMIO* newfilm)
{
	if (pos == NULL)
	{
		printf("添加电影信息失败\n");
		return;
	}

	FMIO* prev = pos->prev;
	prev->next = newfilm;
	newfilm->next = pos;
	newfilm->prev = prev;
	pos->prev = newfilm;
	printf("增加成功\n");
}

//按照电影名字查找电影介绍
FMIO* FindFilmbyname(FMIO* phead, char* name)
{
	assert(phead);
	if (phead->next == phead)
	{
		printf("系统中没有电影\n");
		return NULL;
	}
	FMIO* cur = phead->next;
	while (cur != phead)
	{
		if (strcmp(cur->filminfo->name, name) == 0)
		{
			return cur;
		}
		else
		{
			cur = cur->next;
		}
	}
	printf("系统中没有该电影\n");
	return NULL;
}



//对于每个用户买票信息的记录
//如果链表为空创建一个新的头节点
PERTICIO* InitByPerTicio()
{
	PERTICIO* PheadPerTicIo = (PERTICIO*)malloc(sizeof(PERTICIO));
	if (PheadPerTicIo == NULL)
	{
		perror("mallocpersonalticketio:");
	}
	PheadPerTicIo->prev = PheadPerTicIo;
	PheadPerTicIo->next = PheadPerTicIo;
	PheadPerTicIo->pertickinfo = NULL;
	return PheadPerTicIo;
}
//创建一个新的结点关于票的信息，以pertickinfo位参数
PERTICIO* Buynewpersonalticket(personalticket* info)
{
	PERTICIO*newticketnod = (PERTICIO*)malloc(sizeof(PERTICIO));
	if (newticketnod == NULL)
	{
		perror("mallocticket:");
	}
	newticketnod->pertickinfo = (personalticket*)malloc(sizeof(personalticket));
	if (newticketnod->pertickinfo == NULL)
	{
		perror("malloticketinfo:");
	}
	*(newticketnod->pertickinfo) = *info;

	newticketnod->next = NULL;
	newticketnod->prev = NULL;
	return newticketnod;
}
//插入票的信息到链表
void InsertTicketInfo(PERTICIO* pos, PERTICIO* phead, PERTICIO* newticket)
{
	if (pos == NULL)
	{
		printf("添加信息失败\n");
		return;
	}

	PERTICIO* prev = pos->prev;
	prev->next = newticket;
	newticket->next = pos;
	newticket->prev = prev;
	pos->prev = newticket;
}

//对购票的信息
void SavePerTicInfo(PERTICIO* phead)
{
	assert(phead);
	FILE* pf = fopen("personalticket_information.txt", "wb");
	if (pf == NULL)
	{
		perror("Save");
		return;
	}
	//将链表中的信息写入文件
	PERTICIO* cur = phead->next;
	//将电影信息写入
	while (cur != phead)
	{
		fwrite(cur->pertickinfo, sizeof(personalticket), 1, pf);
		cur = cur->next;
	}
	fclose(pf);
	pf = NULL;
}

//将电影购票信息读到链表中
void LoadPerTickInfo(PERTICIO* phead)
{
	FILE* pf = fopen("personalticket_information.txt", "rb");
	if (pf == NULL)
	{
		perror("Loadflim");
		return;
	}
	//读文件
	PERTICIO* cur = phead;
	personalticket tmp;//临时变量
	while (fread(&tmp, sizeof(personalticket), 1, pf) == 1)
	{
		PERTICIO* newnode = (PERTICIO*)malloc(sizeof(PERTICIO));
		newnode->pertickinfo = (personalticket*)malloc(sizeof(personalticket));
		if (newnode == NULL || newnode->pertickinfo == NULL)
		{
			perror("malloc faild");
			return;
		}
		*newnode->pertickinfo = tmp;
		cur->next = newnode;
		newnode->prev = cur;
		newnode->next = phead;
		phead->prev = newnode;
		cur = newnode;
	}
	fclose(pf);
	pf = NULL;
}

//按照场次编号和id查找个人购票信息
STIO* FilmFindByIdNumber(PERTICIO* phead, long long number,char*id)
{
	assert(phead);
	if (phead->next == phead)
	{
		printf("您没有购票信息\n");
		return NULL;
	}
	STIO* cur = phead->next;
	while (cur != phead)
	{
		if (cur->showtimeinfo->number == number)
		{
			return cur;
		}
		else
		{
			cur = cur->next;
		}
	}
	printf("系统中没有该电影\n");
	return NULL;
}


//删除个人购票的某个结点

void PopPerTic(PERTICIO* phead, PERTICIO* pos)
{
	assert(phead);
	if (phead->next == phead)
	{
		printf("系统中没有场次信息\n");
		return;
	}

	STIO* cur = pos;
	STIO* prev = cur->prev;
	STIO* next = cur->next;
	prev->next = next;
	next->prev = prev;
}

//对于每个电影票房链表
//如果链表为空创建一个新的头节点
FMSL* InitByfilmSale()
{
	FMSL* Phead = (FMSL*)malloc(sizeof(FMSL));
	if (Phead == NULL)
	{
		perror("mallocfilmSale:");
	}
	Phead->prev = Phead;
	Phead->next = Phead;
	Phead->filmsaleinfo = NULL;
	return Phead;
}
//创建一个新的结点关于电影票房的信息，
FMSL* BuynewfilmSalenod(filmsale* info)
{
	FMSL* newnod = (FMSL*)malloc(sizeof(FMSL));
	if (newnod == NULL)
	{
		perror("mallonewfilmsalenod:");
	}
	newnod->filmsaleinfo = (filmsale*)malloc(sizeof(filmsale));
	if (newnod->filmsaleinfo == NULL)
	{
		perror("malloticketinfo:");
	}
	*(newnod->filmsaleinfo) = *info;

	newnod->next = NULL;
	newnod->prev = NULL;
	return newnod;
}

//对购票的信息
void SaveFilmSale(FMSL* phead)
{
	assert(phead);
	FILE* pf = fopen("filmsale_information.txt", "wb");
	if (pf == NULL)
	{
		perror("Save");
		return;
	}
	//将链表中的信息写入文件
	FMSL* cur = phead->next;
	//将电影信息写入
	while (cur != phead)
	{
		fwrite(cur->filmsaleinfo, sizeof(filmsale), 1, pf);
		cur = cur->next;
	}
	fclose(pf);
	pf = NULL;
}

//将电影票房信息读到链表中
void LoadFilmSale(FMSL* phead)
{
	FILE* pf = fopen("filmsale_information.txt", "rb");
	if (pf == NULL)
	{
		perror("Loadflim");
		return;
	}
	//读文件
	FMSL* cur = phead;
	filmsale tmp;//临时变量
	while (fread(&tmp, sizeof(filmsale), 1, pf) == 1)
	{
		FMSL* newnode = (FMSL*)malloc(sizeof(FMSL));
		newnode->filmsaleinfo = (filmsale*)malloc(sizeof(filmsale));
		if (newnode == NULL || newnode->filmsaleinfo == NULL)
		{
			perror("malloc faild");
			return;
		}
		*newnode->filmsaleinfo = tmp;
		cur->next = newnode;
		newnode->prev = cur;
		newnode->next = phead;
		phead->prev = newnode;
		cur = newnode;
	}
	fclose(pf);
	pf = NULL;
}

//插入影片票房的信息到链表
void InsertFilmSale(FMSL* pos, FMSL* phead, FMSL* newnod)
{
	if (pos == NULL)
	{
		printf("添加信息失败\n");
		return;
	}

	FMSL* prev = pos->prev;
	prev->next = newnod;
	newnod->next = pos;
	newnod->prev = prev;
	pos->prev = newnod;
}

//按照电影名字查找电影票房
FMSL* FindFilmSalebyname(FMSL* phead, char* name)
{
	assert(phead);
	if (phead->next == phead)
	{
		printf("系统中没有电影\n");
		return NULL;
	}
	FMSL* cur = phead->next;
	while (cur != phead)
	{
		if (strcmp(cur->filmsaleinfo->filmname, name) == 0)
		{
			return cur;
		}
		else
		{
			cur = cur->next;
		}
	}

	return NULL;
}

void PopAccount(ATIO* phead, ATIO* pos)
{
	assert(phead);
	if (phead->next == phead)
	{
		printf("系统中没有场次信息\n");
		return;
	}

	ATIO* cur = pos;
	ATIO* prev = cur->prev;
	ATIO* next = cur->next;
	prev->next = next;
	next->prev = prev;
}
//如果影厅链表为空创建一个新的头节点
Hallnode* InitByHall()
{
	Hallnode* Phead = (Hallnode*)malloc(sizeof(Hallnode));
	if (Phead == NULL)
	{
		perror("mallochall:");
	}
	Phead->prev = Phead;
	Phead->next = Phead;
	return Phead;
}

Hallnode* BuynewHall(HallInfo*hallinfo)
{
	Hallnode* NewHallnode = (Hallnode*)malloc(sizeof(Hallnode));
	if (NewHallnode == NULL)
	{
		perror("mallocHallnode:");
	}
	NewHallnode->hallinfo = (HallInfo*)malloc(sizeof(HallInfo));
	if (NewHallnode->hallinfo == NULL)
	{
		perror("mallohallinfo:");
	}
	*(NewHallnode->hallinfo) = *hallinfo;
	//将座位信息初始化
	//未售卖就是   '*'
	//售卖：'#'
	//维修'|';
	NewHallnode->hallinfo->seat[0][0] = '0';

	int j = 1;
	char i = '1';
	for (j=1;j<=hallinfo->col;j++)
	{
		if (j == 10)
			i = 'a';
		NewHallnode->hallinfo->seat[0][j] = i;
		i++;
	}
	j = 1;
	i = '1';
	for (j = 1; j <= hallinfo->row; j++)
	{
		if (j == 10)
			i = 'a';
		NewHallnode->hallinfo->seat[j][0] = i;
		i++;
	}
	for (int m = 1; m <= hallinfo->row; m++)
	{
		for (int q = 1; q <= hallinfo->col; q++)
		{
			NewHallnode->hallinfo->seat[m][q] = '@';
		}
	}
	NewHallnode->next = NULL;
	NewHallnode->prev = NULL;
	return NewHallnode;
}
//
void SaveHall(Hallnode* phead)
{
	assert(phead);
	FILE* pf = fopen("hall_information.txt", "wb");
	if (pf == NULL)
	{
		perror("Savehall");
		return;
	}
	//将链表中的信息写入文件
	Hallnode* cur = phead->next;
	//将电影信息写入
	while (cur != phead)
	{
		fwrite(cur->hallinfo, sizeof(HallInfo), 1, pf);
		cur = cur->next;
	}
	fclose(pf);
	pf = NULL;
}

//将电影票房信息读到链表中
void LoadHall(Hallnode* phead)
{
	FILE* pf = fopen("hall_information.txt", "rb");
	if (pf == NULL)
	{
		perror("Loadhall");
		return;
	}
	//读文件
	Hallnode* cur = phead;
	HallInfo tmp;//临时变量
	while (fread(&tmp, sizeof(HallInfo), 1, pf) == 1)
	{
		Hallnode* newnode = (Hallnode*)malloc(sizeof(Hallnode));
		newnode->hallinfo = (HallInfo*)malloc(sizeof(HallInfo));
		if (newnode == NULL || newnode->hallinfo == NULL)
		{
			perror("malloc faild");
			return;
		}
		*newnode->hallinfo = tmp;
		cur->next = newnode;
		newnode->prev = cur;
		newnode->next = phead;
		phead->prev = newnode;
		cur = newnode;
	}
	fclose(pf);
	pf = NULL;
}
//按照影厅的名字查找影厅
Hallnode* FindHallbyname(Hallnode* phead, char* name)
{
	assert(phead);
	if (phead->next == phead)
	{
		printf("系统中没有该影厅\n");
		return NULL;
	}
	Hallnode* cur = phead->next;
	while (cur != phead)
	{
		if (strcmp(cur->hallinfo->nameofhall, name) == 0)
		{
			return cur;
		}
		else
		{
			cur = cur->next;
		}
	}
	printf("系统中没有该影厅\n");
	return NULL;
}
//影厅：结点插入
void InsertHall(Hallnode* pos, Hallnode* phead, Hallnode* newnode)
{
	if (pos == NULL)
	{
		printf("添加信息失败\n");
		return;
	}

	Hallnode* prev = pos->prev;
	prev->next = newnode;
	newnode->next = pos;
	newnode->prev = prev;
	pos->prev = newnode;
}
//删除电影厅pos结点
void PopHall(Hallnode* phead, Hallnode* pos)
{
	assert(phead);
	if (phead->next == phead)
	{
		printf("系统中没有该影厅\n");
		return;
	}

	Hallnode* cur = pos;
	Hallnode* prev = cur->prev;
	Hallnode* next = cur->next;
	prev->next = next;
	next->prev = prev;
}

//按照影厅名字查找电影场次；
//参数：链表头节点和电影名字
STIO* FilmFindByHallName(STIO* phead, char name[])
{
	assert(phead);
	if (phead->next == phead)
	{
		return NULL;
	}
	STIO* cur = phead->next;
	while (cur != phead)
	{
		if (strcmp(cur->showtimeinfo->Hall, name) == 0&&cur->showtimeinfo->status==0)
		{
			return cur;
		}
		else
		{
			cur = cur->next;
		}
	}
	return NULL;
}
STIO* FilmFindByHallName2(STIO* phead, char name[])
{
	assert(phead);
	if (phead->next == phead)
	{
		printf("系统中没有电影\n");
		return NULL;
	}
	STIO* cur = phead->next;
	while (cur != phead)
	{
		if (strcmp(cur->showtimeinfo->Hall, name) == 0 && cur->showtimeinfo->status == 1)
		{
			return cur;
		}
		else
		{
			cur = cur->next;
		}
	}
	return NULL;
}
