#include<stdio.h>
#include<string.h>
#include <stdlib.h>

//ini格式如下： 
//[ID] 
//name=person
//age=number
//good=1 or 0
//gen=0 or 1
//其他条目自定义，可写成专用函数 
//分号注释允许
//空行允许

//多余空格允许 
//atoi函数 将字符串转化成数值 
struct PERSON
{
	char ID[50];
	char name[50];
	int gen;
	int good;
	int age;
};
	int skip=0; 
void putin(FILE *,char*);
void initialization();
void list();//空函数，用于日后更改 
PERSON person[256];
int main()
{
	FILE *p;//指向INI文件
	initialization();
	char tmp[100];
	char tmp2[50];
	char head[100];
	int i,j,k;
	int num=-1;
	int age,gen,good;
	char name[50];
	//每次读取一行
	if((p=fopen("test.ini","r"))==NULL)
	{
		printf("test.ini不存在.");
		return 0;
	}
	printf("读取结果如下：\n"); 
	while(!feof(p))
	{

		//分析,从第一个不为空格的开始 
		putin(p,tmp);
		i=0;
		while(tmp[i]==' ')
			i++; 
		//---------------------------
		if(skip==1)
		{
			skip=0;
			continue; 
		}
		else if(tmp[i]=='[')
		{
			num++;
			for(i++,k=0;tmp[i]!=']';i++)
			{
				head[k++]=tmp[i];
			}
			head[k]=0; //手动补零 
			strcpy(person[num].ID,head);
		//	printf("%s\n",head);//debug 
		}
		//----------------------------

		else 
		{
			//读取第一部分
			for(k=0;tmp[i]!='=';i++)
			{
				 
				head[k++]=tmp[i];
			}	
			head[k]=0;	
		//	printf("%s=",head);
			//第二部分 
			for(k=0,i++;tmp[i]!=0;i++)
			{
				if(tmp[i]==';') break;
				tmp2[k++]=tmp[i];
			}
			tmp2[k]=0;
		//	printf("%s=",head);
			if(!strcmp(head,"age"))
			{
				person[num].age=atoi(tmp2);
			} 
			else if(!strcmp(head,"gen"))
			{
				person[num].gen=atoi(tmp2);
			}
			else if(!strcmp(head,"good"))
			{
				person[num].good=atoi(tmp2);
			}
			else if(!strcmp(head,"name"))
			{
				strcpy(person[num].name,tmp2);
			}
		//	printf("%s\n",tmp2);
		}
		
		
		
	}
	//输出
	if(num==-1) 
	{
		printf("文件为空文件.\n");
		system("pause"); 
		return 0; 
	} 	
	for(int c=0;c<=num;c++)
	{
		
		printf("------------\n"); 
		printf("ID:%s\n",person[c].ID);
		printf("姓名:%s\n",person[c].name);
		if(person[c].gen!=-1) 
		{
			if(person[c].gen==0)
				printf("性别：男\n"); 
			else
				printf("性别：女\n"); 
		}
		if(person[c].good!=-1)
		{
			if(person[c].good==1)
				printf("健康状态：良好\n"); 
			else
				printf("健康状态：差\n");
		}
		if(person[c].age!=-1)
			printf("年龄：%d\n",person[c].age);
	}
	
	system("pause"); 
	fclose(p); 
	return 0;
} 

void putin(FILE *p,char*str)
{
	//输入部分，跳过空格，忽略空行，允许注释 
	int i=0;
	char ch;
		ch=fgetc(p);
	while(ch==' ') 
		ch=fgetc(p);
	if(ch=='\n' || ch==EOF || ch==';')
		skip=1; //第一个非空格空行跳过 
	while(ch!='\n' && ch!=EOF)
	{

		if(ch!=' ')
		{
			str[i++]=ch;	
		}
		str[i]=0;
		ch=fgetc(p);
	}
}
void initialization()
{
	for(int i=0;i<5;i++)
	{
		person[i].age=-1;
		strcpy(person[i].name,"Unknown");
		person[i].good=-1;
		person[i].gen=-1;
	}
}
void list()
{
	//后续更改 
} 
