#include<stdio.h>
#include<string.h>
#include <stdlib.h>

//ini��ʽ���£� 
//[ID] 
//name=person
//age=number
//good=1 or 0
//gen=0 or 1
//������Ŀ�Զ��壬��д��ר�ú��� 
//�ֺ�ע������
//��������

//����ո����� 
//atoi���� ���ַ���ת������ֵ 
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
void list();//�պ����������պ���� 
PERSON person[256];
int main()
{
	FILE *p;//ָ��INI�ļ�
	initialization();
	char tmp[100];
	char tmp2[50];
	char head[100];
	int i,j,k;
	int num=-1;
	int age,gen,good;
	char name[50];
	//ÿ�ζ�ȡһ��
	if((p=fopen("test.ini","r"))==NULL)
	{
		printf("test.ini������.");
		return 0;
	}
	printf("��ȡ������£�\n"); 
	while(!feof(p))
	{

		//����,�ӵ�һ����Ϊ�ո�Ŀ�ʼ 
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
			head[k]=0; //�ֶ����� 
			strcpy(person[num].ID,head);
		//	printf("%s\n",head);//debug 
		}
		//----------------------------

		else 
		{
			//��ȡ��һ����
			for(k=0;tmp[i]!='=';i++)
			{
				 
				head[k++]=tmp[i];
			}	
			head[k]=0;	
		//	printf("%s=",head);
			//�ڶ����� 
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
	//���
	if(num==-1) 
	{
		printf("�ļ�Ϊ���ļ�.\n");
		system("pause"); 
		return 0; 
	} 	
	for(int c=0;c<=num;c++)
	{
		
		printf("------------\n"); 
		printf("ID:%s\n",person[c].ID);
		printf("����:%s\n",person[c].name);
		if(person[c].gen!=-1) 
		{
			if(person[c].gen==0)
				printf("�Ա���\n"); 
			else
				printf("�Ա�Ů\n"); 
		}
		if(person[c].good!=-1)
		{
			if(person[c].good==1)
				printf("����״̬������\n"); 
			else
				printf("����״̬����\n");
		}
		if(person[c].age!=-1)
			printf("���䣺%d\n",person[c].age);
	}
	
	system("pause"); 
	fclose(p); 
	return 0;
} 

void putin(FILE *p,char*str)
{
	//���벿�֣������ո񣬺��Կ��У�����ע�� 
	int i=0;
	char ch;
		ch=fgetc(p);
	while(ch==' ') 
		ch=fgetc(p);
	if(ch=='\n' || ch==EOF || ch==';')
		skip=1; //��һ���ǿո�������� 
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
	//�������� 
} 
