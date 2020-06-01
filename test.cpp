#include <stdio.h>
#include <string.h>
//不完全的程序
int endswith(const char*, const char*);
void pluralWord(char*);

int main()
{
    //测试用代码
    char word1[10];
    scanf("%s", word1);
    pluralWord(word1);
    printf("%s\n",word1);
    return 0;
}

//将字符串更改
void pluralWord(char* str)
{
    if(endswith(str, "s") || endswith(str, "x") || endswith(str, "ch") || endswith(str, "sh") || endswith(str, "o"))
        strcat(str, "es");
    else if(endswith(str, "y"))
    {
        while(*str)
            str++;
        *--str = 'i';
        strcat(str, "es");
    }
    else
        strcat(str, "s");
}

//判断某字符串是否以某字符串结尾
int endswith(const char *str, const char *endword)
{
	int Slen = strlen(str);
	int Elen = strlen(endword);
	if(Slen >= Elen)//结尾字符串必然比目标字符串小
	{
		str += Slen - Elen;//移动字符串指针到要判断的位置，比如判断"Chainsx"是否以"nsx"结尾，指针移动到倒数第一个n的位置
		while(*endword)//在endword没有到达结尾时
		{
			if(*str++ == *endword++)//开始逐个判断是否相等
				continue;
			return 0;
		}
		return 1;
	}
	return 0;
}
//ByHanxven20200531GUET1900301037
//至于怎么区分元音字母和辅音字母，英语渣表示你这是在难为我胖虎
