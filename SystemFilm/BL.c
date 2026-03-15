#define _CRT_SECURE_NO_WARNINGS 1
#include"BL.h"
#include"DL.h"
#include <stdio.h>
#include <conio.h>

#define MAX_STR_LEN  32

char passwd[MAX_STR_LEN] = { 0 };

/*char* GetPasswd(void)
{
	char c;
	int i = 0;
	int len = MAX_STR_LEN - 1;
	while (getchar() != '\n');
	while ((c = _getch()) != '\r')
	{
		passwd[i] = c;
		putchar('*');
		i++;
		if (i >= len)
		{
			break;
		}
	}

	return passwd;
}*/
char* GetPasswd(void)
{
	char c;
	int i = 0;
	int len = MAX_STR_LEN - 1;

	// 清空输入缓冲区（避免残留字符干扰）
	fflush(stdin);

	while ((c = _getch()) != '\r')  // '\r'是回车键，结束输入
	{
		// ========== 核心：处理退格键 ==========
		if (c == 8) {  // 8是退格键（Backspace）的ASCII码
			if (i > 0) {  // 只有已输入字符时才删除
				i--;                // 索引回退，覆盖原字符
				printf("\b \b");   // 视觉上删除终端的*：\b回退光标→空格覆盖→再回退
			}
			continue;  // 跳过后续赋值逻辑
		}

		// 正常输入字符（仅接受可见字符，过滤控制字符）
		if (c >= 32 && c <= 126) {
			passwd[i] = c;
			putchar('*');  // 显示*替代真实密码
			i++;
			if (i >= len) {  // 达到最大长度停止
				break;
			}
		}
	}

	passwd[i] = '\0';  // 必须：添加字符串结束符
	putchar('\n');     // 回车后换行，终端更美观
	return passwd;
}

//对于id字符是否在15个字符以内进行检查
//返回1说明符合要求
int CheckId(char tmpid[15])
{
	int flag = 0;
	char tmp[15];
	strcpy(tmp, tmpid);
	for (int i = 0; i < strlen(tmpid); i++)
	{
		if (tmp[i] <= '9' && tmp[i] >= '0' || tmp[i] <= 'z' && tmp[i] >= 'a' || tmp[i] <= 'Z' && tmp[i] >= 'A')
		{
			flag = 0;
		}
		else
		{
			flag = 1;
			break;
		}
	}
	if (flag == 0)
		return 1;
	if (flag == 1)
		return 0;
}

//检查密码是否符合要求
int CheckPass(char* password)
{
	if (strlen(password) != 6)
	{
		return 0;
	}
	int flag = 0;
	char tmp[15];
	strcpy(tmp, password);
	for (int i = 0; i < strlen(password); i++)
	{
		if (tmp[i] <= '9' && tmp[i] >= '0' )
		{
			flag = 0;
		}
		else
		{
			flag = 1;
			break;
		}
	}
	if (flag == 0)
		return 1;
	if (flag == 1)
		return 0;

}

//注册添加账户
int RegisterUser(char id[15],char* password)
{
	ATIO* plist = InitByAccount();
	//加载账户文件的信息
	Load(plist);
	//判断id是否重复
	ATIO* pos = FindAccountUerName(id, plist);
	if (pos != NULL)
	{
		printf("该id已重复不可用\n");
		return 0;
	}
	//直接添加
	ATIO* NewAccount = BuynewAccount(id,password);
	//0表示观影者
	NewAccount->peoinfo->identifier = 0;
	//将账户添加到链表中
	//搞个头节点
	

    InsertAccount(plist, NewAccount);
	//再保存到文件中
	SaveAccount(plist);
}

int RegisterMana(char id[15], char* password)
{
	//直接添加
	ATIO* plist = InitByAccount();
	//加载账户文件的信息
	Load(plist);
	//判断id是否重复
	ATIO* pos = FindAccountUerName(id, plist);
	if (pos != NULL)
	{
		printf("该id已重复不可用\n");
		return 0;
	}
	ATIO* NewAccount = BuynewAccount(id, password);
	//0表示观影者
	NewAccount->peoinfo->identifier = 1;
	//将账户添加到链表中
	//搞个头节点
	InsertAccount(plist, NewAccount);
	//再保存到文件中
	SaveAccount(plist);
}

//账号和密码对比文件信息是否正确
int CheckAccountInfileUser(char id[15],char* password)
{
	int flag = 0;
	PEODATA peo;
	strcpy(peo.id, id);
	strcpy(peo.password, password);
	
	//将账户添加到链表中
	//搞个头节点
	ATIO* plist = InitByAccount();
	//加载账户文件的信息
	Load(plist);
	//
	if (FindAccountUser(&peo, plist))
	{
		return 1;
	}
	else
		return 0;

}
int CheckAccountInfileMana(char id[15], char* password)
{
	int flag = 0;
	PEODATA peo;
	strcpy(peo.id, id);
	strcpy(peo.password, password);
	//将账户添加到链表中
	//搞个头节点
	ATIO* plist = InitByAccount();
	//加载账户文件的信息
	Load(plist);
	//
	if (FindAccountMana(&peo, plist))
	{
		return 1;
	}
	else
		return 0;

}
//
//查看电影场次
//遍历链表
//打印数据
void ViewShowTime()
{
	//创建一个场次的头节点
	STIO* plist = InitByShowTime();
	//将文件信息读到场次的链表中
	LoadShowTime(plist);
	if (plist->next == plist)
	{
		printf("没有场次信息\n");
		return;
	}
	//遍历打印链表信息
	int cnt = 1;
	STIO* cur = plist->next;
	while (cur != plist)
	{
		//打印信息
		printf("%-5s\t%-20s\t%-20s\t\t%-15s\t%-10s\t%-15s\n", "序号","电影编号","时间", "电影名字", "单价","影厅");
		printf("%-5d\t%-20lld\t%-d-%-d-%d:%d--%-d-%-d-%d:%d\t\t%-15s\t%-10.2f\t%-15s\n",cnt,cur->showtimeinfo->number,cur->showtimeinfo->sessiom.smonth, cur->showtimeinfo->sessiom.sday, cur->showtimeinfo->sessiom.shour, cur->showtimeinfo->sessiom.sminute, cur->showtimeinfo->sessiom.fmonth, cur->showtimeinfo->sessiom.fday, cur->showtimeinfo->sessiom.fhour, cur->showtimeinfo->sessiom.fminute, cur->showtimeinfo->NameFilm,cur->showtimeinfo->price,cur->showtimeinfo->Hall);
		printf("座位信息：\n");
		printf("*--->未售\n#--->已售\n!--->故障\n");
		for (int i = 0; i <= cur->showtimeinfo->row; i++)
		{
			for (int j = 0; j <= cur->showtimeinfo->col; j++)
			{                
				printf("%-3c", cur->showtimeinfo->seat[i][j]);
			}
			printf("\n");
		}
		printf("--------------------------------------------------------------------------------\n");
		cnt++;
		cur = cur->next;
	}

}

//获取电影场次信息
void NewShowTime()
{
	int tmpNumber;
	int flag = 0;//查看序号是否有效
	//插在这个电影序号的位置
	scanf("%d", &tmpNumber);
	//通过这个电影序号找到序号为tmpNumber-1的结点
	//创建一个场次的头节点
	int cnt = 1;
	STIO* plist = InitByShowTime();
	//将文件信息读到场次的链表中
	LoadShowTime(plist);
	//如果链表位空直接添加
	int  tmpsmonth, tmpsday, tmpshour, tmpsminute,  tmpfmonth, tmpfday, tmpfhour, tmpfminute;
	long long tmpnumber;
	char tmpname[20];
	float tmpprice;
	char tmphallname[15];
	if (plist->next == plist)
	{
		ShowTime tmpshowtimeinfo;
		// 读入新节点的信息并且添加到cur结点之后
			//读入编号/时间等信心，并对这两个信息进行检查
			
		//默认座位都是初始化好的
		printf("请输入该场次的编号：\n");
		scanf("%lld", &tmpnumber);
		//对编号合法话以及是否重复进行检查
		//——————————————————————————————————————————————————————————————————
		STIO* pos = FilmFindByNumber(plist, tmpnumber);
		if (pos != NULL)
		{
			printf("输入编号重复，请重新操作\n");
		}

		printf("请输入该场次的时间：\n");
		printf("month-day-hour:minute--month-day-hour:minute\n");
		scanf("%d-%d-%d:%d--%d-%d-%d:%d", &tmpsmonth, &tmpsday, &tmpshour, &tmpsminute,&tmpfmonth,&tmpfday,&tmpfhour,&tmpfminute);
		//判断时间是否冲突
		//——————————————————————————————————————————————————————————————————
		if (CheckTime(tmpsmonth, tmpsday, tmpshour, tmpsminute) == 0||CheckTime(tmpfmonth, tmpfday, tmpfhour, tmpfminute)==0)
		{
			printf("时间输入格式有问题\n");
			return;
		}
		tmpshowtimeinfo.sessiom.smonth = tmpsmonth;
		tmpshowtimeinfo.sessiom.sday = tmpsday;
		tmpshowtimeinfo.sessiom.shour = tmpshour;
		tmpshowtimeinfo.sessiom.sminute = tmpsminute;
		tmpshowtimeinfo.sessiom.fmonth = tmpfmonth;
		tmpshowtimeinfo.sessiom.fday = tmpfday;
		tmpshowtimeinfo.sessiom.fhour = tmpfhour;
		tmpshowtimeinfo.sessiom.fminute = tmpfminute;
		if (CheckTimeConflict(tmpshowtimeinfo.sessiom, plist) == 0)//有冲突
		{
			printf("输入时间有冲突自动退回\n");
			return 0;
		}

		printf("请输入该场次的电影名字：\n");
		scanf("%s", tmpname);
		//对比电影介绍的的文件，若没有该电影，添加介绍
		FMIO* plistFilm = InitByFilm();
		LoadFilmInfo(plistFilm);
		FMIO* film = FindFilmbyname(plistFilm, tmpname);
		if (film == NULL)
		{
			//添加新的电影介绍
			AddFilmInfoInShowTime(tmpname);
		}
		printf("请输入该场次的影厅名字：\n");
		scanf("%s", tmphallname);
		Hallnode* plisthall = InitByHall();
		LoadHall(plisthall);
		Hallnode* hallpos = FindHallbyname(plisthall, tmphallname);
		if (hallpos == NULL)
		{
			printf("没有该影厅输入错误\n");
			return;
		}
		//对比电影票房的文件，若没有该电影，新建一个
		FMSL* plistfilmsale = InitByfilmSale();
		LoadFilmSale(plistfilmsale);
		FMSL* filmsale = FindFilmSalebyname(plistfilmsale, tmpname);
		if (filmsale == NULL)
		{
			//添加新的电影票房结构体
			FMSL*newfilmsalenod=AddFilmSale(tmpname);
			InsertFilmSale(plistfilmsale, plistfilmsale, newfilmsalenod);
			SaveFilmSale(plistfilmsale);
		}
		printf("请输入该场次的价格：\n");
		scanf("%f", &tmpprice);
		//判断合理性
		//————————————————————————————————————————————————————————————————————

		tmpshowtimeinfo.number = tmpnumber;
		tmpshowtimeinfo.price = tmpprice;
		tmpshowtimeinfo.row = hallpos->hallinfo->row;
		tmpshowtimeinfo.col = hallpos->hallinfo->col;

		strcpy(tmpshowtimeinfo.NameFilm, tmpname);
		strcpy(tmpshowtimeinfo.Hall, tmphallname);
		STIO* newShowTimenod = BuynewShowTime(&tmpshowtimeinfo,hallpos);
		//将新结点插入链表
		InsertShowTime(plist, plist, newShowTimenod);
		//保存到文件
		SaveShowTime(plist);
		return;
	}
	if (tmpNumber < 0)
	{
		printf("序号无效\n");
		return;
	}
	STIO* cur = plist->next;
	while (cur != plist)
	{
		if (cnt == tmpNumber)
		{
			flag = 1;
			cnt++;
			cur = cur->next;
			
		}
		else
		{
			cnt++;
			cur = cur->next;
		}
	}
	//找到了cur
	if (flag||tmpNumber==cnt)
	{
		ShowTime tmpshowtimeinfo;

		//读入新节点的信息并且添加到cur结点之后
		//读入编号/时间等信心，并对这两个信息进行检查
		//默认座位都是初始化好的
		printf("请输入该场次的编号：\n");
		scanf("%lld", &tmpnumber);
		//对编号合法话以及是否重复进行检查
		//——————————————————————————————————————————————————————————————————
		STIO* pos = FilmFindByNumber(plist, tmpnumber);
		if (pos != NULL)
		{
			printf("输入编号重复，请重新操作\n");
		}
		printf("请输入该场次的时间：\n");
		printf("month-day-hour:minute--month-day-hour:minute\n");
		scanf("%d-%d-%d:%d--%d-%d-%d:%d", &tmpsmonth, &tmpsday, &tmpshour, &tmpsminute, &tmpfmonth, &tmpfday, &tmpfhour, &tmpfminute);
		//判断时间是否冲突
		//——————————————————————————————————————————————————————————————————
		if (CheckTime(tmpsmonth, tmpsday, tmpshour, tmpsminute) == 0 || CheckTime(tmpfmonth, tmpfday, tmpfhour, tmpfminute) == 0)
		{
			printf("时间输入格式有问题\n");
			return;
		}
		tmpshowtimeinfo.sessiom.smonth = tmpsmonth;
		tmpshowtimeinfo.sessiom.sday = tmpsday;
		tmpshowtimeinfo.sessiom.shour = tmpshour;
		tmpshowtimeinfo.sessiom.sminute = tmpsminute;
		tmpshowtimeinfo.sessiom.fmonth = tmpfmonth;
		tmpshowtimeinfo.sessiom.fday = tmpfday;
		tmpshowtimeinfo.sessiom.fhour = tmpfhour;
		tmpshowtimeinfo.sessiom.fminute = tmpfminute;
		if (CheckTimeConflict(tmpshowtimeinfo.sessiom, plist) == 0)//有冲突
		{
			printf("输入时间有冲突自动退回\n");
			return 0;
		}

		printf("请输入该场次的电影名字：\n");
		scanf("%s", tmpname);
		//对比电影介绍的的文件，若没有该电影，添加介绍
		FMIO* plistFilm = InitByFilm();
		LoadFilmInfo(plistFilm);
		FMIO* film = FindFilmbyname(plistFilm, tmpname);
		if (film == NULL)
		{
			//添加新的电影介绍
			AddFilmInfoInShowTime(tmpname);
		}
		printf("请输入该场次的影厅名字：\n");
		scanf("%s", tmphallname);
		Hallnode* plisthall = InitByHall();
		LoadHall(plisthall);
		Hallnode* hallpos = FindHallbyname(plisthall, tmphallname);
		if (hallpos == NULL)
		{
			printf("没有该影厅输入错误\n");
			return;
		}
		
		//对比电影票房的文件，若没有该电影，新建一个
		FMSL* plistfilmsale = InitByfilmSale();
		LoadFilmSale(plistfilmsale);
		FMSL* filmsale = FindFilmSalebyname(plistfilmsale, tmpname);
		if (filmsale == NULL)
		{
			//添加新的电影票房结构体
			FMSL* newfilmsalenod = AddFilmSale(tmpname);
			InsertFilmSale(plistfilmsale, plistfilmsale, newfilmsalenod);
			SaveFilmSale(plistfilmsale);
		}
		printf("请输入该场次的价格：\n");
		scanf("%f", &tmpprice);
		//判断合理性
		//————————————————————————————————————————————————————————————————————
		tmpshowtimeinfo.row = hallpos->hallinfo->row;
		tmpshowtimeinfo.col = hallpos->hallinfo->col;
		tmpshowtimeinfo.number = tmpnumber;
		tmpshowtimeinfo.price = tmpprice;
		strcpy(tmpshowtimeinfo.NameFilm, tmpname);
		strcpy(tmpshowtimeinfo.Hall, tmphallname);

		STIO* newShowTimenod = BuynewShowTime(&tmpshowtimeinfo,hallpos);
		//将新结点插入链表
		InsertShowTime(cur, plist, newShowTimenod);
		//保存到文件
		SaveShowTime(plist);
	}
	else
	{
		printf("序号无效\n");
	}
	
}

//对要删除的场次的编号信息的接收
void Popreceive()
{
	STIO* plist = InitByShowTime();
	//将文件信息读到场次的链表中
	LoadShowTime(plist);
	long long tmpnumber;


	printf("请输入要删去的场次的编号：\n");
	scanf("%lld", &tmpnumber);
	//对编号合法话以及是否重复进行检查
	//——————————————————————————————————————————————————————————————————
	//找这个场次根据编号
	STIO* pos = FilmFindByNumber(plist, tmpnumber);
	if (pos == NULL)
		return;
	PopShowTime(plist, pos);
	//更改后的文件存到链表中
	SaveShowTime(plist);
}

//对要修改的场次的编号信息的接收
void Modifyreceive()
{
	ViewShowTime();
	STIO* plist = InitByShowTime();
	//将文件信息读到场次的链表中
	LoadShowTime(plist);
	if (plist->next == plist)
	{
		printf("没有场次\n");
	}
	long long tmpnumber;
	printf("请输入要修改的场次的编号：\n");
	scanf("%lld", &tmpnumber);
	//对编号合法话以及是否重复进行检查
	//——————————————————————————————————————————————————————————————————-------------
	//找这个场次根据编号
	STIO* pos = FilmFindByNumber(plist, tmpnumber);
	if (pos == NULL)
		return;
	//输入要修改的具体是哪个信息
		char input;
		int tmp;
		do {
			ModifyControlShow();
			scanf(" %c", &input);
			if (input < '0' || input>'9')
			{
				printf("输入错误\n");
				continue;
			}
			tmp = input - '0';
			switch (tmp)
			{
			case 1://修改场次时间
				system("cls");
				printf("请输入修改后的时间：\n");
				printf("month-day-hour:minute--month-day-hour:minute\n");
				scanf("%d%d%d%d%d%d%d%d", &pos->showtimeinfo->sessiom.smonth, &pos->showtimeinfo->sessiom.sday, &pos->showtimeinfo->sessiom.shour, &pos->showtimeinfo->sessiom.sminute,& pos->showtimeinfo->sessiom.fmonth, &pos->showtimeinfo->sessiom.fday, &pos->showtimeinfo->sessiom.fhour, &pos->showtimeinfo->sessiom.fminute);
				SaveShowTime(plist);
				break;
			case 2:
				system("cls");//修改场次电影
				printf("请输入修改后的电影名称\n");
				char tmpname[15];
				scanf("%s", tmpname);

				FMIO* plistFilm = InitByFilm();
				LoadFilmInfo(plistFilm);
				FMIO* film = FindFilmbyname(plistFilm, tmpname);
				if (film == NULL)
				{
					//添加新的电影介绍
					printf("系统中没有该电影\n");
					break;
				}
				strcpy(pos->showtimeinfo->NameFilm, tmpname);
				SaveShowTime(plist);
				break;
			case 3://修改场次价格
				system("cls");
				printf("请输入修改后的价格\n");
				scanf("%f", &pos->showtimeinfo->price);
				SaveShowTime(plist);
				break;
			case 4://修改电影场次座位售卖情况
				//打印该场次座位售卖情况
				printf("座位信息：\n");
				printf("*--->未售\n#--->已售\n");
				for (int i = 1; i <= 10; i++)
				{
					for (int j = 1; j <= 15; j++)
					{
						printf("%-3c", pos->showtimeinfo->seat[i][j]);
					}
					printf("\n");
				}
				printf("--------------------------------------------------------------------------------\n");
				system("cls");
				printf("请输入要修改的座位编号\n");
				int tmprow, tmpcol;
				scanf("%d%d", &tmprow, &tmpcol);
				if (tmprow < 0 || tmprow>10 || tmpcol < 0 || tmpcol>10)
				{
					break;
				}
				printf("请输入修改后的状态 （#/*）\n");
				char pre = pos->showtimeinfo->seat[tmprow][tmpcol];
				while (getchar() != '\n');
				scanf("%c", &pos->showtimeinfo->seat[tmprow][tmpcol]);
				if (pos->showtimeinfo->seat[tmprow][tmpcol] == '#' && pre == '*')
				{
					AddFilmSaleByname(pos->showtimeinfo->NameFilm, pos);
				}
				else if (pos->showtimeinfo->seat[tmprow][tmpcol] == '*' && pre == '#')
				{
					RefundFilmSaleByname(pos->showtimeinfo->NameFilm, pos);

				}
				SaveShowTime(plist);
				break;
			case 5:
				system("cls");
				printf("请输入修改后的放映情况（1->已放映，2->未放映\n");
				scanf("%d", &pos->showtimeinfo->status);
				SaveShowTime(plist);
				break;
			case 0:
				printf("退出系统\n");
				break;
			default:
				printf("输入错误\n");
				continue;
			}
		} while (tmp);
	
}

//查看电影介绍
void ShowFilmInfo()
{
	FMIO* plist = InitByFilm();
	//将文件信息读到场次的链表中
	LoadFilmInfo(plist);
	if (plist->next == plist)
	{
		printf("抱歉该影院系统内暂时没有影片信息\n");
		return;
	}
	printf("本影院的电影有：\n");
	FMIO* cur = plist->next;
	int cnt = 1;
	while (cur != plist)
	{
		printf("%-10s\t%-30s\t%-30s\n", "序号","电影名字", "导演");
		printf("%-10d\t%-30s\t%-30s\n", cnt,cur->filminfo->name, cur->filminfo->directior);
		printf("电影内容介绍：\n");
		printf("%s\n", cur->filminfo->introduction);
		printf("\n");
		cur = cur->next;
		cnt++;
	}
}

//增加电影介绍
void AddFilmInfo()
{
	char tmpname[30];
	char tmpintroduction[150];
	char tmpdirectior[30];
	printf("请输入新录入的电影名称\n");
	scanf("%s", tmpname);
	printf("请输入新录入的电影的导演\n");
	scanf("%s", tmpdirectior);
	printf("请输入新录入的电影简单介绍\n");
	scanf("%s", tmpintroduction);
	FMIO* newFilmnode = BuynewFilm(tmpname, tmpintroduction, tmpdirectior);
	//增加到链表中
	FMIO* plist = InitByFilm();
	LoadFilmInfo(plist);
	InsertFilmInfo(plist, plist, newFilmnode);
	SaveFilmInfo(plist);
}
//增加电影介绍在添加场次时
void AddFilmInfoInShowTime(char*name)
{
	printf("该电影本影院没有录入信息，请录入信息\n");
	char tmpname[30];
	char tmpintroduction[150];
	char tmpdirectior[30];
	//printf("请输入新录入的电影名称\n");
	//scanf("%s", tmpname);
	strcpy(tmpname, name);
	printf("请输入新录入的电影的导演\n");
	scanf("%s", tmpdirectior);
	printf("请输入新录入的电影简单介绍\n");
	scanf("%s", tmpintroduction);
	FMIO* newFilmnode = BuynewFilm(tmpname, tmpintroduction, tmpdirectior);
	//增加到链表中
	FMIO* plist = InitByFilm();
	LoadFilmInfo(plist);
	InsertFilmInfo(plist, plist, newFilmnode);
	SaveFilmInfo(plist);
}

//进入购票
void Buyticket(char* id)
{
	//创建一个场次的头节点
	STIO* plist = InitByShowTime();
	//将文件信息读到场次的链表中
	LoadShowTime(plist);
	if (plist->next == plist)
	{
		return;
	}
	//显示所有场次信息；
	ViewShowTime();
	printf("请输入场次编号：\n");
	long long tmpnumber;
	scanf("%lld", &tmpnumber);
	//根据编号找到这个电影场次
	STIO* cur = FilmFindByNumber(plist, tmpnumber);
	if (cur == NULL)
		return;
	printf("请输入排号和列号\n");
	int tmprow;
	int tmpcol;
	scanf("%d%d", &tmprow, &tmpcol);
	if (tmprow < 0 || tmprow>cur->showtimeinfo->row || tmpcol < 0 || tmpcol>cur->showtimeinfo->col)
	{
		printf("没有该座位退出\n");
		return;
	}
	if (cur->showtimeinfo->seat[tmprow][tmpcol] == '#'||cur->showtimeinfo->seat[tmprow][tmpcol]=='!')
	{
		printf("该位置不可购买\n");
		return;
	}
	//更改场次的座位信息
	cur->showtimeinfo->seat[tmprow][tmpcol] = '#';
	//更改票房信息
	AddFilmSaleByname(cur->showtimeinfo->NameFilm, cur);
	//更改后在保存到文件中
	SaveShowTime(plist);
	//仅限十排十列
	//将id和座位和场次信息封装为一个结点
	personalticket newperticket;
	newperticket.row = tmprow;
	newperticket.col = tmpcol;
	newperticket.showtime = *(cur->showtimeinfo);
	strcpy(newperticket.id, id);
	PERTICIO* newperticketnode = Buynewpersonalticket(&newperticket);
	//插入到链表中
	PERTICIO* plistTicket = InitByPerTicio();
	LoadPerTickInfo(plistTicket);
	InsertTicketInfo(plistTicket, plistTicket, newperticketnode);
	//保存到文件中
	SavePerTicInfo(plistTicket);

}

//查看已购订单
//带上返回值方便查找要退票的序号
void PersonalTicketShow(char* id)
{
	PERTICIO* plist = InitByPerTicio();
	LoadPerTickInfo(plist);
	if (plist->next == plist)
	{
		printf("您目前没有购票\n");
		return;
	}
	//查找链表中id符合的并打印出来
	PERTICIO* cur = plist->next;
	printf("您所有的购票信息如下\n");
	int cnt = 1;
	while (cur != plist)
	{
		if (strcmp(cur->pertickinfo->id, id)==0)
		{
			//打印信息
			printf("序号：");
			printf("%d\n", cnt);
			
			printf("电影编号：");
			cnt++;
			printf("%lld\n", cur->pertickinfo->showtime.number);
			printf("时间：");
			printf("%-d-%-d-%-d:%-d--%-d-%-d-%-d:%-d\n",  cur->pertickinfo->showtime.sessiom.smonth, cur->pertickinfo->showtime.sessiom.sday, cur->pertickinfo->showtime.sessiom.shour, cur->pertickinfo->showtime.sessiom.sminute, cur->pertickinfo->showtime.sessiom.fmonth, cur->pertickinfo->showtime.sessiom.fday, cur->pertickinfo->showtime.sessiom.fhour, cur->pertickinfo->showtime.sessiom.fminute);
			printf("电影名称：");
			printf("%s\n", cur->pertickinfo->showtime.NameFilm);
			printf("单价：");
			printf("%f\n", cur->pertickinfo->showtime.price);
			printf("座位信息：");
			printf("%d %d\n",cur->pertickinfo->row,cur->pertickinfo->col);
			printf("电影厅：");
			printf("%s\n", cur->pertickinfo->showtime.Hall);
			printf("------------------------------------------------------------\n");
		}
		cur = cur->next;
		
	}
}
//用户退票
void ticketRefund(char*id)
{
	//显示已购的票有哪些
	PersonalTicketShow(id);
	//选择要退的场次的电影编号
	
	PERTICIO* plist = InitByPerTicio();
	LoadPerTickInfo(plist);
	if (plist->next == plist)
	{
		printf("无法退票\n");
		return;
	}
	printf("请输入您打算退的电影场次的序号：\n");
	int tmpcnt;
	int flag = 0;
	scanf("%d", &tmpcnt);
	//查找链表中id符合的并打印出来
	PERTICIO* cur = plist->next;
	int cnt = 1;
	while (cur != plist)
	{
		if (strcmp(cur->pertickinfo->id, id) == 0)
		{
			if (cnt == tmpcnt)
			{
				flag = 1;
				break;
			}
			
		}
		cnt++;
		cur = cur->next;

	}
	if (flag == 0)
	{
		printf("序号输入有误，请重新操作\n");
		return;
	}
	//查看当前场次是否放映
	//若已经放映
	if (cur->pertickinfo->showtime.status == 1)
	{
		printf("当前场次已放映，不可退票\n");
		return;
	}
	//删除cur结点
	
	PopPerTic(plist, cur);
	SavePerTicInfo(plist);

	//更改所删除的位置的场次购票信息
	//找到场次中的这个结点
	//创建一个场次的头节点
	STIO* plistShowTime = InitByShowTime();
	//将文件信息读到场次的链表中
	LoadShowTime(plistShowTime);
	STIO* posShowTime = FilmFindByNumber(plistShowTime, cur->pertickinfo->showtime.number);
	int tmprow = cur->pertickinfo->row;
	int tmpcol = cur->pertickinfo->col;

	posShowTime->showtimeinfo->seat[tmprow][tmpcol] = '*';
	SaveShowTime(plistShowTime);
	//更改票房信息
	//___________________________________________________________________________
	RefundFilmSaleByname(cur->pertickinfo->showtime.NameFilm, posShowTime);


}

//显示影院总票房情况
void FilmSaleShow()
{
	FMSL* plist = InitByfilmSale();
	LoadFilmSale(plist);
	if (plist->next == plist)
	{
		printf("暂无票房信息\n");
		return;
	}
	//查找链表中id符合的并打印出来
	FMSL* cur = plist->next;
	printf("该影院票房信息如下\n");
	int cnt = 1;
	while (cur != plist)
	{
		
		printf("电影名称：");
		printf("%s\n", cur->filmsaleinfo->filmname);
		printf("所售票房总计(张)：");
		printf("%d\n", cur->filmsaleinfo->ticketnumber);
		printf("金额总计（元）：");
		printf("%f\n", cur->filmsaleinfo->totalmoney);
		printf("------------------------------------------------------------\n");
		
		cur = cur->next;

	}
}

//添加电影票房结构体
FMSL* AddFilmSale(char*tmpname)
{
	filmsale newfilmsale;
	strcpy(newfilmsale.filmname, tmpname);
	newfilmsale.ticketnumber = 0;
	newfilmsale.totalmoney = 0;
	FMSL* newfilesalenod = BuynewfilmSalenod(&newfilmsale);
	return newfilesalenod;
}

//针对电影名字和电影场次退一张票
void RefundFilmSaleByname(char* name, STIO* pos)
{
	FMSL* plist = InitByfilmSale();
	LoadFilmSale(plist);
	FMSL* cur = FindFilmSalebyname(plist, name);
	//修改cur结点的内容
	cur->filmsaleinfo->ticketnumber--;
	cur->filmsaleinfo->totalmoney -= pos->showtimeinfo->price;
	SaveFilmSale(plist);
}
//针对电影名字和电影场次买一张票
void AddFilmSaleByname(char* name, STIO* pos)
{
	FMSL* plist = InitByfilmSale();
	LoadFilmSale(plist);
	FMSL* cur = FindFilmSalebyname(plist, name);
	//修改cur结点的内容
	cur->filmsaleinfo->ticketnumber++;
	cur->filmsaleinfo->totalmoney += pos->showtimeinfo->price;
	SaveFilmSale(plist);
}

void UserAccouttraverse()
{
	//搞个头节点
	ATIO* plist = InitByAccount();
	//加载账户文件的信息
	Load(plist);
	ATIO* cur = plist->next;
	int cnt = 1;
	while (cur != plist)
	{
		if (cur->peoinfo->identifier == 0)
		{
			printf("用户%d:\n", cnt);
			printf("id:%s\n", cur->peoinfo->id);
			printf("password:%s\n", cur->peoinfo->password);
		}
		cur = cur->next;
	}
}

//注销用户账户
void LogoutUser()
{
	ATIO* plist = InitByAccount();
	//加载账户文件的信息
	Load(plist);
	printf("输入注销账户的id\n");
	//删除该id 所有的购票信息，----------------------------------------------
	char tmpid[15];
	scanf("%s", tmpid);
	ATIO* pos = FindAccountUerName(tmpid, plist);
	PopAccount(plist, pos);
	SaveAccount(plist);

}

void AddHall()
{
	Hallnode* plist=InitByHall();
	LoadHall(plist);
	HallInfo tmp;
	printf("请输入新增影厅的名字：\n");
	scanf("%s", tmp.nameofhall);
	printf("请输入新影厅的大小(排and列）\n");
	scanf("%d%d", &tmp.row, &tmp.col);
	Hallnode*newhall=BuynewHall(&tmp);
	InsertHall(plist, plist, newhall);
	SaveHall(plist);
}
void PopHallnode()
{
	Hallnode* plisthall = InitByHall();
	LoadHall(plisthall);
	HallInfo tmp;
	printf("请输入要删去的影厅的名字：\n");
	scanf("%s", tmp.nameofhall);

	Hallnode* newhall = FindHallbyname(plisthall, tmp.nameofhall);
	//如果当前场次有该影厅播放就不可删，影厅结构体中没有也不可删
	if (newhall == NULL)
	{
		printf("没有该影厅\n");
		return;
	}
	STIO* plistshowtime = InitByShowTime();
	//将文件信息读到场次的链表中
	LoadShowTime(plistshowtime);
	STIO* posinshowtime = FilmFindByHallName(plistshowtime, tmp.nameofhall);
	if (posinshowtime)
	{
		//不可删
		printf("当前影厅排有电影场次，不可删\n");
		return;
	}
	PopHall(plisthall, newhall);
	//找到所有影厅为该影厅的场次删掉
	do {

		STIO* showtimepoped = FilmFindByHallName2(plistshowtime, tmp.nameofhall);
		if (showtimepoped == NULL)
			break;
		PopShowTime(plistshowtime, showtimepoped);

	} while (1);
	SaveShowTime(plistshowtime);
	SaveHall(plisthall);
}

void Modifyhallseat()
{
	STIO* plistshowtime = InitByShowTime();
	//将文件信息读到场次的链表中
	LoadShowTime(plistshowtime);
	//遍历影厅座位
	Hallnode* plisthall = InitByHall();
	LoadHall(plisthall);
	Hallnode* cur = plisthall->next;
	printf("所有影厅情况如下\n");
	while (cur != plisthall)
	{
		printf("影厅名：");
		printf("%s\n", cur->hallinfo->nameofhall);
		printf("座位信息：\n");
		printf("!---->故障\n@---->正常\n");
		for (int i = 0; i <= cur->hallinfo->row; i++)
		{
			for (int j = 0; j <= cur->hallinfo->col; j++)
			{
				printf("%-3c", cur->hallinfo->seat[i][j]);
			}
			printf("\n");
		}
		printf("--------------------------------------------------------------------------------\n");
		cur = cur->next;
	}
	printf("请输入您要维修的影厅的名字:\n");
	char hallname[15];
	scanf("%s", hallname);
	STIO* posinshowtime = FilmFindByHallName(plistshowtime, hallname);
	Hallnode* hallpos = FindHallbyname(plisthall, hallname);
	if (hallpos == NULL)
	{
		printf("没有该影厅\n");
		return;
	}
	printf("请输入座位号\n");
	int tmprow, tmpcol;
	scanf("%d%d", &tmprow, &tmpcol);
	char flagseat = hallpos->hallinfo->seat[tmprow][tmpcol];//记录修改前的状态
	printf("请输入修改后的状态(!//@)\n");
	//while (getchar() != '\n');
	char tmpseat;
	char ch2 = getchar();  // 实际读取的是缓冲区中残留的 '\n'
	//printf("ch2 = %c (ASCII码：%d)\n", ch2, (int)ch2);
	scanf("%c", &tmpseat);
	//printf("%c", tmpseat);
	if (tmpseat != '!' && tmpseat != '@')
	{
		printf("输入无效\n");
		return;
	}

	hallpos->hallinfo->seat[tmprow][tmpcol]=tmpseat;
	if(posinshowtime!=NULL)
	{
		if (flagseat == '@' && tmpseat == '!')
		{
			posinshowtime->showtimeinfo->seat[tmprow][tmpcol] = '!';
		}

		if (flagseat == '!' && tmpseat == '@')
		{
			posinshowtime->showtimeinfo->seat[tmprow][tmpcol] = '*';
		}
	}
	
	SaveShowTime(plistshowtime);
	SaveHall(plisthall);
}

int CheckTime(int month, int day, int hour, int minute)
{
	/*int leap = 0;
	if (year % 4 == 0 && year % 100 != 0 || year % 400 == 0)
	{
		leap = 1;
	}*/
	int staleapmonth[13] = { 0,31,29,31,30,31,30,31,31,30,31,30,31 };
	int stanotleapmonth[13] = { 0,31,28,31,30,31,30,31,31,30,31,30,31 };
	if (month < 0 || month>12)
		return 0;
	/*if (leap)
	{
		if (day<0 || day>staleapmonth[month])
			return 0;
	}
	else
	{
		if (day<0 || day>stanotleapmonth[month])
			return 0;
	}*/
	if (day<0 || day>stanotleapmonth[month])
		return 0;
	if (hour > 24 || hour < 0)
		return 0;
	if (minute > 60 || minute < 0)
		return 0;
	return 1;
}

//模糊搜素电影，先在电影介绍里面模糊搜索，在打印该电影的排片问题
void FuzzySearchFilm()
{ 
	FMIO* plistFilm = InitByFilm();
	LoadFilmInfo(plistFilm);
	printf("请输入要搜索的电影相关词汇：\n");
	char searchedword[15];
	scanf("%s", searchedword);
	int flag = 0;
	int flag2 = 0;
	FMIO* curinfilm = plistFilm->next;
	printf("电影内容相关信息：\n");
	while (curinfilm != plistFilm)
	{
		if (strstr(curinfilm->filminfo->name, searchedword) != NULL)
		{
			//打印电影介绍的信息
			flag= 1;
			printf("%-30s\t%-30s\n",  "电影名字", "导演");
			printf("%-30s\t%-30s\n",  curinfilm->filminfo->name, curinfilm->filminfo->directior);
			printf("电影内容介绍：\n");
			printf("%s\n", curinfilm->filminfo->introduction);
			printf("-----------------------------------------------------\n");
		}
		curinfilm = curinfilm->next;
	}
	if (flag == 0)
	{
		printf("暂无搜索结果\n");
	}
	printf("电影场次相关信息\n");
	STIO* plistShowTime = InitByShowTime();
	//将文件信息读到场次的链表中
	LoadShowTime(plistShowTime);
	if (plistShowTime->next == plistShowTime)
	{
		printf("没有场次信息\n");
		return;
	}
	STIO* curinshowtime = plistShowTime->next;
	while (curinshowtime != plistShowTime)
	{
		if (strstr(curinshowtime->showtimeinfo->NameFilm, searchedword) != NULL)
		{
			flag2 = 1;
			//打印电影场次信息
			printf("%-20s\t%-40s\t%-15s\t%-10s\t%-15s\n",  "电影编号", "时间", "电影名字", "单价", "影厅");
			printf("%-20lld\t%-d-%-d-%d:%d--t%-d-%-d-%d:%d\t\t%-15s\t%-10.2f\t%-15s\n", curinshowtime->showtimeinfo->number, curinshowtime->showtimeinfo->sessiom.smonth, curinshowtime->showtimeinfo->sessiom.sday, curinshowtime->showtimeinfo->sessiom.shour, curinshowtime->showtimeinfo->sessiom.sminute, curinshowtime->showtimeinfo->sessiom.fmonth, curinshowtime->showtimeinfo->sessiom.fday, curinshowtime->showtimeinfo->sessiom.fhour, curinshowtime->showtimeinfo->sessiom.fminute, curinshowtime->showtimeinfo->NameFilm, curinshowtime->showtimeinfo->price, curinshowtime->showtimeinfo->Hall);
			printf("---------------------------------------------------\n");
		}
		curinshowtime = curinshowtime->next;
	}
	if (flag2 == 0)
	{
		printf("暂无搜索结果\n");
	}
}
//计算片长的辅助函数将某个时间转化为从1月1日开始算的分钟
long long TimeToMinutes(int month,int day,int hour,int minute)
{
	int stanmonth[13] = { 0,31,28,31,30,31,30,31,31,30,31,30,31 };
	int sumday = 0;
	for (int i = month - 1; i > 0; i--)
	{
		sumday += stanmonth[i];
	}
	sumday += (day-1);
	long long summinute = sumday * 24 * 60 + hour * 60 + minute;
	return summinute;
}
//计算电影片长
int runtime(Time session)
{
	long long sminutes = TimeToMinutes(session.smonth, session.sday, session.shour, session.sminute);
	long long fminutes = TimeToMinutes(session.fmonth, session.fday, session.fhour, session.fminute);
	return (int)(fminutes - sminutes);
}
//检查时间是否有冲突
//有冲突返回0；
int CheckTimeConflict(Time session, STIO*phead)
{
	long long sminutes1 = TimeToMinutes(session.smonth, session.sday, session.shour, session.sminute);
	long long fminutes1 = TimeToMinutes(session.fmonth, session.fday, session.fhour, session.fminute);
	if (sminutes1 > fminutes1)
		return 0;
	STIO* cur = phead->next;
	int flag = 0;
	while (cur != phead)
	{
		long long sminutes2 = TimeToMinutes(cur->showtimeinfo->sessiom.smonth, cur->showtimeinfo->sessiom.sday, cur->showtimeinfo->sessiom.shour, cur->showtimeinfo->sessiom.sminute);
		long long fminutes2 = TimeToMinutes(cur->showtimeinfo->sessiom.fmonth, cur->showtimeinfo->sessiom.fday, cur->showtimeinfo->sessiom.fhour, cur->showtimeinfo->sessiom.fminute);
		if (sminutes1<fminutes2 && fminutes1>sminutes2)
		{
			flag = 1;
		}
		cur = cur->next;
	}
	if (flag == 1)
		return 0;
	else
		return 1;
}