#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<windows.h>
//增加文件读取功能 
//edit by Hanxven 20200305
void draw_map();//绘制地图的函数
void getkey();//用于接受按键的函数 
int Move(int);//用于判定移动是否可行 
int finish();//判定是否为过关
void copyt_map();//从库中读取地图
void FileRead(); //从文件中读取地图
void putin(FILE*,char*);//自定义输入函数

HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);//创建句柄 
COORD pos;//位置句柄 


char ch;//临时变量，存储按下的按键
int ext=0;//判断R是否按下以确定是否重置本关
int self=0;//指定当前运行模式是否为从文件读取的关卡
int posX=1;//所在位置变量，表示当前*的位置X轴，缺省值为1
int posY=1;//同上 ，Y轴
int level=0; //当前是第几关
int count=0;//走了多少步统计 
//int initPOS[3][2]={{1,1},{4,2},{1,3}};//每一关初始Y,X坐标 ，已废弃
void FindInit();//寻找初始坐标 ，取代上一行的初始坐标，这个函数直接从地图中读取*的位置
FILE *pf;//文件指针，指向已经读取的文件：local.txt
 
//设定地图 ，拟设计三张地图 
char map_source[3][21][21]={
{//20*20的地图 ，预设目前三关
		{"###############"},
		{"#          Ox #"},
		{"#  O          #"},
		{"#             #"},
		{"#     x       #"},
		{"#     O       #"},
		{"#           * #"},
		{"#             #"},
		{"#      xO     #"},
		{"#             #"},
		{"#   Ox        #"},
		{"#             #"},
		{"#     xO      #"},
		{"#            x#"},
		{"###############"},
		{"第一关送分题"} 
},
{
		{" ######"},
		{" #    #"},
		{"##x##O##"},
		{"# xxO  #"},
		{"# *#O  #"},
		{"#    ###"},
		{"######"},
		{"本关稍微动脑"}
		
},
{//800步才能过关
	{"  ####"},
	{"  #* #### 233"},
	{" ##  O  # 233"},
	{" # @x@  #"},
	{"## @ @ ##"},
	{"#  @ @ #"},
	{"#  @@@ #"},
	{"##     #"},
	{" ####  #"},
	{"    ####"},
	{"过关送你米10"} 

},
	
	};
char map[21][21];//地图大小20*20，多出的留给'\0'
	
int main()
{
//	int level_select;//选关部分，因关卡数不多，没有写的必要，已搁置
	system("title 推尼玛的箱子");//程序标题
	system("mode con: cols=40 lines=25");//设定控制台大小
	//欢迎界面
	printf("推箱子之FOR LJC特制版（难度略微调整）\n全关过送你小米10\n"); 
	printf("规则：\n"); 
	printf("#是墙壁\nO是箱子\n@是推进去的箱子\nx是箱子位点\n");
	printf("R键重新本关卡\nWASD移动\n祝你玩的开心\n本游戏共设计三关\n请先切换到英文输入\n");
	printf("\n按F读取local.txt文件自定义关卡(最大20*20限制)，语法规则如上\n其他任意键游玩自带关卡");
	ch=getch();//接受键盘输入，存储到ch中
	if(ch=='F' || ch=='f')//判断是否进入读取文件的模式
	{
		if((pf=fopen("local.txt","r"))==NULL)
		{
			printf("\n出现错误，文件可能不存在\n");
			getch();//用于暂停，后同
			return 0; //退出程序
		}
		self=1; //表示进入文件读取模式
	} 
//	level_select=getch(); //搁置
//	if(level_select>=0 && level_select<=2) level=level_select;//搁置
	while(1)
	{//无限循环
		system("cls");//清屏
		count=0; //每一关或者重开都要重新计数
		if(self==0) 
		{//如果不是文件模式（即只进行预设关卡）
			copyt_map();//从库中读取地图
			FindInit();//得到*的坐标
		}
		else
		{
			FileRead();//从文件中读取地图
			FindInit();//同上
		}
		while(1)
		{
			draw_map();//绘制当前地图
			getkey();//得到按键
			if(ext==1)//判断是否重开
			{
				ext=0;break;
			}
			if(finish())
			{//判断是否通关，判定标准：地图上没有未推进的箱子
				draw_map();//绘制出最后一帧
				if(self==1)
				{//文件中的关卡完成
					printf("\n自定义关卡完成。任意键继续\n");
					getch();
					return 0;
				}
				printf(" \n\n过关了。任意键继续");
				getch();
				if(level<3 && self!=1)
				{//关卡数目前仅有3个
					level++;//进入第二关
				}
				else
				{//全关完成
					printf(" 地址给我，我送你小米10");
					getch();
					return 0;
				}
				break;
			}	
		}
	}
	
	return 0;
}

void getkey()
{//程序臃肿警告
	//获得移动按键 
	int ret; //确定按键代号
	while(1)
	{
		ch=getch();
		if(ch=='w'||ch=='W')//大小写都要支持
		{
			ret=1;//确定行动代号，1代表上
			if(Move(ret)) posY--;//只有上移可行，才会变动*的坐标
			break;
		}
		else if(ch=='s'||ch=='S')//同上
		{
			ret=2;
			if(Move(ret)) posY++;
			break;
		} 
		else if(ch=='a'||ch=='A')
		{
			ret=3;
			if(Move(ret)) posX--;
			break;
		}
		else if(ch=='d'||ch=='D')
		{
			ret=4;
			if(Move(ret)) posX++;
			break;
		} 
		else if(ch=='r'||ch=='R')
		{
			ext=1;//重开局设定
			break;
		} 
	}	 	
}

void draw_map()
{
	pos.X=0;//将光标移动到左上角
	pos.Y=0;
	SetConsoleCursorPosition(handle, pos);
	for(int i=0;i<20;i++)//绘制当前地图
	{
		printf("%s\n",map[i]);
	}
	//再移动光标到*的位置，绘制*
	pos.X=posX;
	pos.Y=posY; 
	SetConsoleCursorPosition(handle, pos);
	printf("*"); 
	
	//移动光标到下面，先抹除原来信息
	pos.X=0;
	pos.Y=17;
	SetConsoleCursorPosition(handle, pos);
	printf("              "); 
	//再输出数据：当前位置，已用步数
	pos.X=0;
	pos.Y=17;
	SetConsoleCursorPosition(handle, pos);
	printf("X:%d,Y:%d\n",posX,posY); 
	printf("Step(s)used:%d",count);
} 


int Move(int ret)
{//主要函数，判定移动是否可行

	if(ret==1)//上移动
	{
		if(map[posY-1][posX]=='#') //如果要移动的方向是墙，则不可移动
			return 0;
		else if(map[posY-1][posX]==' ' || map[posY-1][posX]=='x')//如果要移动的方向是空白或者箱子位点，则可以移动，并计步数
		{
			count++;return 1;
		}
		else if(map[posY-1][posX]=='O' && map[posY-2][posX]!='#' && map[posY-2][posX]!='O' && map[posY-2][posX]!='@')//如果正在推箱子，那么只有要推的方向没有墙或者其他箱子
		{
			if(map[posY-2][posX]=='x') //如果是点位 ，箱子O变为@ 
			{
				map[posY-2][posX]='@';
				map[posY-1][posX]=' ';
				count++;
				return 1; 
			}
			map[posY-2][posX]='O';//不是点位，就仅仅是推动 
			map[posY-1][posX]=' ';
			count++;
			return 1;
		}
		else if(map[posY-1][posX]=='@' && map[posY-2][posX]!='#' && map[posY-2][posX]!='O' && map[posY-2][posX]!='@')//如果是把箱子推出点位，要判断是否被堵住
		{
			if(map[posY-2][posX]=='x')//如果是从一个点位推到另一个点位，则将脚后刷新为点位
			{
				map[posY-2][posX]='@';
				map[posY-1][posX]='x';	
				count++;return 1; 
			}
			map[posY-2][posX]='O';//将箱子从位点推出，后面刷新x 
			map[posY-1][posX]='x';
			count++;
			return 1; 
		} 
	}
	else if(ret==2)//同上
	{
		if(map[posY+1][posX]=='#') 
			return 0;
		else if(map[posY+1][posX]==' ' || map[posY+1][posX]=='x')
		{
			count++;return 1;
		}
		else if(map[posY+1][posX]=='O' && map[posY+2][posX]!='#' && map[posY+2][posX]!='O'&& map[posY+2][posX]!='@')
		{
			if(map[posY+2][posX]=='x') 
			{
				map[posY+2][posX]='@';
				map[posY+1][posX]=' ';
				count++;
				return 1;
			}
			map[posY+2][posX]='O';
			map[posY+1][posX]=' ';
			count++;
			return 1;
		}
		else if(map[posY+1][posX]=='@' && map[posY+2][posX]!='#' && map[posY+2][posX]!='O' && map[posY+2][posX]!='@')
		{
			if(map[posY+2][posX]=='x')
			{
				map[posY+2][posX]='@';
				map[posY+1][posX]='x';	
				count++;return 1; 
			}
			map[posY+2][posX]='O';
			map[posY+1][posX]='x';
			count++;
			return 1;
		}
	}
	else if(ret==3)//左 
	{
		if(map[posY][posX-1]=='#') 
			return 0;
		else if(map[posY][posX-1]==' ' || map[posY][posX-1]=='x')
		{
			count++;return 1;
		}
		else if(map[posY][posX-1]=='O' && map[posY][posX-2]!='#' && map[posY][posX-2]!='O' && map[posY][posX-2]!='@')
		{
			if(map[posY][posX-2]=='x')
			{
				map[posY][posX-2]='@';
				map[posY][posX-1]=' ';
				count++;
				return 1;
			}
			map[posY][posX-2]='O';
			map[posY][posX-1]=' ';
			count++;
			return 1;
		}
		else if(map[posY][posX-1]=='@' && map[posY][posX-2]!='#' && map[posY][posX-2]!='O' && map[posY][posX-2]!='@')
		{
			if(map[posY][posX-2]=='x')
			{
				map[posY][posX-2]='@'; 
				map[posY][posX-1]='x';	
				count++;return 1; 
			}
			map[posY][posX-2]='O';
			map[posY][posX-1]='x';
			count++;
			return 1;	
		}
	}
	else if(ret==4)//右 
	{
		if(map[posY][posX+1]=='#') 
			return 0;
		else if(map[posY][posX+1]==' ' || map[posY][posX+1]=='x')
		{
			count++;return 1;
		}
		else if(map[posY][posX+1]=='O' && map[posY][posX+2]!='#' && map[posY][posX+2]!='O' && map[posY][posX+2]!='@')
		{
			if(map[posY][posX+2]=='x')
			{
				map[posY][posX+2]='@';
				map[posY][posX+1]=' ';
				count++;
				return 1;	
			}
			map[posY][posX+2]='O';
			map[posY][posX+1]=' ';
			count++;
			return 1;
		}
		else if(map[posY][posX+1]=='@' && map[posY][posX+2]!='#' && map[posY][posX+2]!='O' && map[posY][posX+2]!='@')
		{
			if(map[posY][posX+2]=='x')
			{
				map[posY][posX+2]='@';
				map[posY][posX+1]='x';	
				count++;return 1; 
			}
			map[posY][posX+2]='O';
			map[posY][posX+1]='x';
			count++;
			return 1;
		}
	}
	return 0;
}

int finish()
{
	
	//判定标准：无O.
	//逐个读取
	for(int i=0;i<20;i++)
	{
		for(int j=0;j<20;j++)
		{
			if(map[i][j]==0)
				break;
			if(map[i][j]=='O')
			{
				return 0;
			}
		}
	} 
	return 1; 
} 

void copyt_map()
{
	//将库地图拷贝到当前地图中

	 for(int i=0;i<20;i++)
	 {
	 	for(int j=0;j<20;j++)
	 	{
	 		map[i][j]=map_source[level][i][j];
		}
	 }
}

void FileRead()
{//从文件中读取地图
//putin(poniter,array)函数输入文件指针以及数组，将文件中的一行（不包括换行）读入到数组中
	for(int i=0;i<21;i++)
	{
		putin(pf,map[i]);
	}
	rewind(pf);//每次读完后，文件指针归位以便下次读取
} 

void putin(FILE *P,char*tmp)
{
	//该函数读取一行设置
	int i=0;
	char cha;//临时数组
	cha=fgetc(P);//读取一个字符
	while(cha!='\n' && cha!=EOF)//在没有遇到换行或者文件结尾
	{
		tmp[i++]=cha;//读取到数组中
		cha=fgetc(P);
	} 
	return;
}
void FindInit()
{//寻找初始坐标
	for(int i=0;i<21;i++)
	{
		for(int j=0;j<21;j++)
		{
			if(map[i][j]=='*')
			{
				//找到后记录坐标，并填空格在坐标位置
				//(*在游戏中是动态的，并不存在与当前地图中)
				posY=i;
				posX=j;
				map[i][j]=' ';
				return;
			}
		}
	}
}
//注释结束。