#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<windows.h>
//�����ļ���ȡ���� 
//edit by Hanxven 20200305
void draw_map();//���Ƶ�ͼ�ĺ���
void getkey();//���ڽ��ܰ����ĺ��� 
int Move(int);//�����ж��ƶ��Ƿ���� 
int finish();//�ж��Ƿ�Ϊ����
void copyt_map();//�ӿ��ж�ȡ��ͼ
void FileRead(); //���ļ��ж�ȡ��ͼ
void putin(FILE*,char*);//�Զ������뺯��

HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);//������� 
COORD pos;//λ�þ�� 


char ch;//��ʱ�������洢���µİ���
int ext=0;//�ж�R�Ƿ�����ȷ���Ƿ����ñ���
int self=0;//ָ����ǰ����ģʽ�Ƿ�Ϊ���ļ���ȡ�Ĺؿ�
int posX=1;//����λ�ñ�������ʾ��ǰ*��λ��X�ᣬȱʡֵΪ1
int posY=1;//ͬ�� ��Y��
int level=0; //��ǰ�ǵڼ���
int count=0;//���˶��ٲ�ͳ�� 
//int initPOS[3][2]={{1,1},{4,2},{1,3}};//ÿһ�س�ʼY,X���� ���ѷ���
void FindInit();//Ѱ�ҳ�ʼ���� ��ȡ����һ�еĳ�ʼ���꣬�������ֱ�Ӵӵ�ͼ�ж�ȡ*��λ��
FILE *pf;//�ļ�ָ�룬ָ���Ѿ���ȡ���ļ���local.txt
 
//�趨��ͼ ����������ŵ�ͼ 
char map_source[3][21][21]={
{//20*20�ĵ�ͼ ��Ԥ��Ŀǰ����
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
		{"��һ���ͷ���"} 
},
{
		{" ######"},
		{" #    #"},
		{"##x##O##"},
		{"# xxO  #"},
		{"# *#O  #"},
		{"#    ###"},
		{"######"},
		{"������΢����"}
		
},
{//800�����ܹ���
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
	{"����������10"} 

},
	
	};
char map[21][21];//��ͼ��С20*20�����������'\0'
	
int main()
{
//	int level_select;//ѡ�ز��֣���ؿ������࣬û��д�ı�Ҫ���Ѹ���
	system("title �����������");//�������
	system("mode con: cols=40 lines=25");//�趨����̨��С
	//��ӭ����
	printf("������֮FOR LJC���ư棨�Ѷ���΢������\nȫ�ع�����С��10\n"); 
	printf("����\n"); 
	printf("#��ǽ��\nO������\n@���ƽ�ȥ������\nx������λ��\n");
	printf("R�����±��ؿ�\nWASD�ƶ�\nף����Ŀ���\n����Ϸ���������\n�����л���Ӣ������\n");
	printf("\n��F��ȡlocal.txt�ļ��Զ���ؿ�(���20*20����)���﷨��������\n��������������Դ��ؿ�");
	ch=getch();//���ܼ������룬�洢��ch��
	if(ch=='F' || ch=='f')//�ж��Ƿ�����ȡ�ļ���ģʽ
	{
		if((pf=fopen("local.txt","r"))==NULL)
		{
			printf("\n���ִ����ļ����ܲ�����\n");
			getch();//������ͣ����ͬ
			return 0; //�˳�����
		}
		self=1; //��ʾ�����ļ���ȡģʽ
	} 
//	level_select=getch(); //����
//	if(level_select>=0 && level_select<=2) level=level_select;//����
	while(1)
	{//����ѭ��
		system("cls");//����
		count=0; //ÿһ�ػ����ؿ���Ҫ���¼���
		if(self==0) 
		{//��������ļ�ģʽ����ֻ����Ԥ��ؿ���
			copyt_map();//�ӿ��ж�ȡ��ͼ
			FindInit();//�õ�*������
		}
		else
		{
			FileRead();//���ļ��ж�ȡ��ͼ
			FindInit();//ͬ��
		}
		while(1)
		{
			draw_map();//���Ƶ�ǰ��ͼ
			getkey();//�õ�����
			if(ext==1)//�ж��Ƿ��ؿ�
			{
				ext=0;break;
			}
			if(finish())
			{//�ж��Ƿ�ͨ�أ��ж���׼����ͼ��û��δ�ƽ�������
				draw_map();//���Ƴ����һ֡
				if(self==1)
				{//�ļ��еĹؿ����
					printf("\n�Զ���ؿ���ɡ����������\n");
					getch();
					return 0;
				}
				printf(" \n\n�����ˡ����������");
				getch();
				if(level<3 && self!=1)
				{//�ؿ���Ŀǰ����3��
					level++;//����ڶ���
				}
				else
				{//ȫ�����
					printf(" ��ַ���ң�������С��10");
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
{//����ӷ�׾���
	//����ƶ����� 
	int ret; //ȷ����������
	while(1)
	{
		ch=getch();
		if(ch=='w'||ch=='W')//��Сд��Ҫ֧��
		{
			ret=1;//ȷ���ж����ţ�1������
			if(Move(ret)) posY--;//ֻ�����ƿ��У��Ż�䶯*������
			break;
		}
		else if(ch=='s'||ch=='S')//ͬ��
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
			ext=1;//�ؿ����趨
			break;
		} 
	}	 	
}

void draw_map()
{
	pos.X=0;//������ƶ������Ͻ�
	pos.Y=0;
	SetConsoleCursorPosition(handle, pos);
	for(int i=0;i<20;i++)//���Ƶ�ǰ��ͼ
	{
		printf("%s\n",map[i]);
	}
	//���ƶ���굽*��λ�ã�����*
	pos.X=posX;
	pos.Y=posY; 
	SetConsoleCursorPosition(handle, pos);
	printf("*"); 
	
	//�ƶ���굽���棬��Ĩ��ԭ����Ϣ
	pos.X=0;
	pos.Y=17;
	SetConsoleCursorPosition(handle, pos);
	printf("              "); 
	//��������ݣ���ǰλ�ã����ò���
	pos.X=0;
	pos.Y=17;
	SetConsoleCursorPosition(handle, pos);
	printf("X:%d,Y:%d\n",posX,posY); 
	printf("Step(s)used:%d",count);
} 


int Move(int ret)
{//��Ҫ�������ж��ƶ��Ƿ����

	if(ret==1)//���ƶ�
	{
		if(map[posY-1][posX]=='#') //���Ҫ�ƶ��ķ�����ǽ���򲻿��ƶ�
			return 0;
		else if(map[posY-1][posX]==' ' || map[posY-1][posX]=='x')//���Ҫ�ƶ��ķ����ǿհ׻�������λ�㣬������ƶ������Ʋ���
		{
			count++;return 1;
		}
		else if(map[posY-1][posX]=='O' && map[posY-2][posX]!='#' && map[posY-2][posX]!='O' && map[posY-2][posX]!='@')//������������ӣ���ôֻ��Ҫ�Ƶķ���û��ǽ������������
		{
			if(map[posY-2][posX]=='x') //����ǵ�λ ������O��Ϊ@ 
			{
				map[posY-2][posX]='@';
				map[posY-1][posX]=' ';
				count++;
				return 1; 
			}
			map[posY-2][posX]='O';//���ǵ�λ���ͽ������ƶ� 
			map[posY-1][posX]=' ';
			count++;
			return 1;
		}
		else if(map[posY-1][posX]=='@' && map[posY-2][posX]!='#' && map[posY-2][posX]!='O' && map[posY-2][posX]!='@')//����ǰ������Ƴ���λ��Ҫ�ж��Ƿ񱻶�ס
		{
			if(map[posY-2][posX]=='x')//����Ǵ�һ����λ�Ƶ���һ����λ���򽫽ź�ˢ��Ϊ��λ
			{
				map[posY-2][posX]='@';
				map[posY-1][posX]='x';	
				count++;return 1; 
			}
			map[posY-2][posX]='O';//�����Ӵ�λ���Ƴ�������ˢ��x 
			map[posY-1][posX]='x';
			count++;
			return 1; 
		} 
	}
	else if(ret==2)//ͬ��
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
	else if(ret==3)//�� 
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
	else if(ret==4)//�� 
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
	
	//�ж���׼����O.
	//�����ȡ
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
	//�����ͼ��������ǰ��ͼ��

	 for(int i=0;i<20;i++)
	 {
	 	for(int j=0;j<20;j++)
	 	{
	 		map[i][j]=map_source[level][i][j];
		}
	 }
}

void FileRead()
{//���ļ��ж�ȡ��ͼ
//putin(poniter,array)���������ļ�ָ���Լ����飬���ļ��е�һ�У����������У����뵽������
	for(int i=0;i<21;i++)
	{
		putin(pf,map[i]);
	}
	rewind(pf);//ÿ�ζ�����ļ�ָ���λ�Ա��´ζ�ȡ
} 

void putin(FILE *P,char*tmp)
{
	//�ú�����ȡһ������
	int i=0;
	char cha;//��ʱ����
	cha=fgetc(P);//��ȡһ���ַ�
	while(cha!='\n' && cha!=EOF)//��û���������л����ļ���β
	{
		tmp[i++]=cha;//��ȡ��������
		cha=fgetc(P);
	} 
	return;
}
void FindInit()
{//Ѱ�ҳ�ʼ����
	for(int i=0;i<21;i++)
	{
		for(int j=0;j<21;j++)
		{
			if(map[i][j]=='*')
			{
				//�ҵ����¼���꣬����ո�������λ��
				//(*����Ϸ���Ƕ�̬�ģ����������뵱ǰ��ͼ��)
				posY=i;
				posX=j;
				map[i][j]=' ';
				return;
			}
		}
	}
}
//ע�ͽ�����