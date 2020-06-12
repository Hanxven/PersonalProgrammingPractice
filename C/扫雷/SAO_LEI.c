#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

/*-------------------------------*
 *                               *
 *    Hanxven20200611:扫雷       *
 *    这是我第一次用这种风       *
 *    格的注释。                 *
 *                               *
 *-------------------------------*/

 /*------------------------------*
 *                               *
 *   定义一个结构体，成员包括:   *
 *   data: 内容，为0-9，以及雷   *
 *   isVisible: 是否已经揭示     *
 *   flag: 是否插了旗子          *
 *                               *
 *   size_ln, size_col:          *
 *   指示地图大小，人为设定边界  *
 *   顺便一提，地图是从数组下标  *
 *   1开始的,周围的一圈设定为0,  *
 *   可以减少程序中多余的边界判  *
 *   定。                        *
 *                               *
 *   flag:                       *
 *   当前还剩下的旗子            *
 *                               *
 *   handle:                     *
 *   用于存储控制台窗口的句柄    *
 *                               *
 *-------------------------------*/

typedef struct
{
    char data;
    char isVisible;
    char flag;
}grid;

grid map[50][50];//定义地图大小，这里为最大48*48
int size_ln = 12, size_col = 15;//定义了地图大小，范围最大48*48(预留一行列)
HANDLE handle;//存储窗口句柄
int mine = 15;//地图的雷
int flag;

void printMap(int, int, int);
void movepos(int ,int);
void setcolor(int ,int);
void hidecursor();
void initmap();
void setmap(int);
void reveal(int, int);
void revealall(int);
int tryit(int, int, char);
int win();
void justJoke();

int main(int argc, char* argv[])
{
    if(argc == 4)
    {
        mine = atoi(argv[1]);
        size_ln = atoi(argv[2]);
        size_col = atoi(argv[3]);
    }

    printf("************************************************\n");
    printf("#          游戏规则: WSAD控制上下左右,         #\n");
    printf("#          游戏规则: WSAD控制上下左右,         #\n");
    printf("#          R为打开当前格子，F为立Flag,         #\n");
    printf("#                  C为翻格子.                  #\n");

    if(argc != 4)
    {
        printf("#              支持命令行参数，如：            #\n");
        printf("#              saolei.exe 15 13 12             #\n");
        printf("#              表示15个雷,高13宽12             #\n");
    }

    printf("#                 任意键，继续.                #\n");
    printf("************************************************\n");
    getch();
    system("cls");



    //初始化
    handle = GetStdHandle(STD_OUTPUT_HANDLE);
    int cur_posX = 0, cur_posY = 0;
    char ch;
    hidecursor();
    flag = mine;
    setmap(mine);
    printMap(cur_posX, cur_posY, 0);
    int failed = 0;


    while(1)
    {
        while(1)
        {
            ch = getch();
            switch(ch)
            {
            case 'w':
            case 'W':
                if(cur_posY >= 1)
                    cur_posY--;
                else
                    continue;
                goto print;
            case 'a':
            case 'A':
                if(cur_posX >= 1)
                    cur_posX--;
                else
                    continue;
                goto print;
            case 's':
            case 'S':
                if(cur_posY < size_ln - 1)
                    cur_posY++;
                else
                    continue;
                goto print;
            case 'd':
            case 'D':
                if(cur_posX < size_col - 1)
                    cur_posX++;
                else
                    continue;
                goto print;
            case 'r':
            case 'R':
                if(!map[cur_posY + 1][cur_posX + 1].isVisible && !map[cur_posY + 1][cur_posX + 1].flag)
                {
                    if(map[cur_posY + 1][cur_posX + 1].data == 'M')
                    {
                        failed = 1;
                        goto print;
                    }
                    else if(map[cur_posY + 1][cur_posX + 1].data == 0)
                        reveal(cur_posY + 1, cur_posX + 1);
                    else
                        map[cur_posY + 1][cur_posX + 1].isVisible = 1;
                }
                goto print;
            case 'f':
            case 'F':
                if(!map[cur_posY + 1][cur_posX + 1].isVisible)
                {
                    if(map[cur_posY + 1][cur_posX + 1].flag)
                    {
                        map[cur_posY + 1][cur_posX + 1].flag = 0;
                        flag++;
                    }
                    else
                    {
                        if(flag)
                        {
                            map[cur_posY + 1][cur_posX + 1].flag = 1;
                            flag--;
                        }
                    }
                }
                goto print;
            case 'c':
            case 'C':
                if(map[cur_posY + 1][cur_posX + 1].data >= 1 && map[cur_posY + 1][cur_posX + 1].data <= 8 && map[cur_posY + 1][cur_posX + 1].isVisible && !map[cur_posY + 1][cur_posX + 1].flag)
                {
                    if(tryit(cur_posY + 1, cur_posX + 1, map[cur_posY + 1][cur_posX + 1].data))
                        failed = 1;
                }
                goto print;
            default:
                continue;
            }
        }

    print:

        if(failed)
        {
            printMap(cur_posX, cur_posY, -1);
            break;
        }
        if(win())
        {
            printMap(cur_posX, cur_posY, 1);
            break;
        }
        printMap(cur_posX, cur_posY, 0);
    }


    getch();
    return 0;

}

/*-------------------------------*
 *                               *
 *    打印地图，每个格子用中括   *
 *    号括起来，并且会高亮显示   *
 *    当前选定的格子。如果格子   *
 *    的值为0,那么输出一个空格   *
 *    只有在格子被揭示之后，即   *
 *    isVisible=1,才可以显示，   *
 *    否则一律显示为问号。       *
 *    被插了旗子就显示为F        *
 *                               *
 *-------------------------------*/

void printMap(int cur_posX, int cur_posY, int status)
{
    if(status == 1)
    {
        movepos(0, size_ln + 1);
        printf("You WIN. GOOD! [^_^]");
        revealall(1);
    }
    else if(status == -1)
    {
        movepos(0, size_ln + 1);
        printf("You LOST, FW. [*_*]");
        revealall(0);
    }

    int ln, col;
    cur_posX *= 3;
    movepos(0, 0);
    for(ln = 1; ln <= size_ln; ln++)
    {
        for(col = 1; col <= size_col; col++)
        {
            if(map[ln][col].data == 'M' && map[ln][col].isVisible)
            {
                setcolor(4, 6);
            }
            else if(map[ln][col].data == 1 && map[ln][col].isVisible)
            {
                setcolor(14, 0);
            }
            else if(map[ln][col].data == 2 && map[ln][col].isVisible)
            {
                setcolor(10, 0);
            }
            else if(map[ln][col].data == 3 && map[ln][col].isVisible)
            {
                setcolor(12, 0);
            }
            else if(map[ln][col].data == 4 && map[ln][col].isVisible)
            {
                setcolor(13, 0);
            }
            else if(map[ln][col].data == 5 && map[ln][col].isVisible)
            {
                setcolor(6, 0);
            }
            else if(map[ln][col].data == 6 && map[ln][col].isVisible)
            {
                setcolor(5, 0);
            }
            else if(map[ln][col].data == 7 && map[ln][col].isVisible)
            {
                setcolor(1, 0);
            }
            else if(map[ln][col].data == 8 && map[ln][col].isVisible)
            {
                setcolor(15, 0);
            }
            else if(map[ln][col].data == 0 && map[ln][col].isVisible)
            {
                setcolor(8, 0);
            }
            else if(map[ln][col].flag)
            {
                setcolor(11, 0);
            }
            putchar('[');
            if(!map[ln][col].isVisible)
            {
                if(map[ln][col].flag)
                    putchar('F');
                else
                    putchar('?');
            }
            else if(map[ln][col].data == 'M')
            {
                putchar('M');
            }
            else if(!map[ln][col].data)
                putchar(' ');
            else
                putchar(map[ln][col].data + '0');
            putchar(']');
            setcolor(7, 0);
        }
        putchar('\n');
    }
    printf("[Flag(s) remain:%3d]  [X:%3d|Y:%3d]", flag, cur_posX / 3, cur_posY);

    setcolor(15, 13);
    movepos(cur_posX, cur_posY);
    putchar('[');
    movepos(cur_posX + 2, cur_posY);
    putchar(']');
    setcolor(7, 0);

    if(status)
        movepos(0, size_ln + 2);
}

//移动光标函数，移动控制台输出位置
void movepos(int x,int y)
{
    COORD pos;
	pos.X = x;//相对位置
	pos.Y = y;
	SetConsoleCursorPosition(handle, pos);
}

/*-------------------------------*
 *                               *
 *   该函数设置控制台的输出颜    *
 *   色，参数为前景色、背景色    *
 *   具体的如下：                *
 *   0 透明无色   8  灰色        *
 *   1 蓝色       9  更浅的蓝色  *
 *   2 深绿色     10 浅绿色      *
 *   3 稍浅的蓝色 11 最浅的蓝色  *
 *   4 红色       12 粉红色      *
 *   5 深紫色     13 紫色        *
 *   6 土黄色     14 肉色        *
 *   7 白色(默认) 15 亮白色      *
 *                               *
 *-------------------------------*/
void setcolor(int foreground, int background)
{
    SetConsoleTextAttribute(handle, foreground | (background << 4));
}

/*-------------------------------*
 *                               *
 *    该函数将光标搞消失。       *
 *    闪烁的光标是真JB烦         *
 *                               *
 *-------------------------------*/
void hidecursor()
{
	CONSOLE_CURSOR_INFO cursor;
	cursor.bVisible = FALSE;
	cursor.dwSize = sizeof(cursor);
	SetConsoleCursorInfo(handle, &cursor);
}


/*-------------------------------*
 *                               *
 *    该函数将地图全部填充为0,   *
 *    初始化整个地图，在setmap   *
 *    函数中使用。               *
 *                               *
 *-------------------------------*/
void initmap()
{
    int ln, col;
    for(ln = 0; ln < 50; ln++)
    {
        for(col = 0; col < 50; col++)
        {
            map[ln][col].data = 0;
            map[ln][col].isVisible = 0;
            map[ln][col].flag = 0;
        }
    }
}


/*-------------------------------*
 *                               *
 *    该函数将会重置地图，同时   *
 *    埋雷，雷的数量取决于参数   *
 *    count,请注意，这个值不能   *
 *    超过地图总大小。           *
 *    附: 随机数生成范围为1到x   *
 *    x为地图边界                *
 *    埋雷结束后，每一个方块会   *
 *    根据周围雷的数自动填充数   *
 *    字。                       *
 *                               *
 *-------------------------------*/
void setmap(int count)
{
    int exist(int x, int y)
    {
        if(map[x][y].data == 'M')
            return 1;
        return 0;
    }

    initmap();
    int already_set = 0;
    int x, y;
    srand((unsigned)time(NULL));
    while(already_set != count)
    {
        x = rand() % size_ln + 1;
        y = rand() % size_col + 1;
        if(exist(x, y))
        {
            continue;
        }
        else
        {
            map[x][y].data = 'M';
            already_set++;
        }
    }

    int mine;
    for(x = 1; x <= size_ln; x++)
    {
        for(y = 1; y <= size_col; y++)
        {
            mine = 0;
            if(map[x][y].data == 'M')
                continue;
            if(map[x-1][y-1].data == 'M')
                mine++;
            if(map[x][y-1].data == 'M')
                mine++;
            if(map[x+1][y-1].data == 'M')
                mine++;
            if(map[x-1][y].data == 'M')
                mine++;
            if(map[x+1][y].data == 'M')
                mine++;
            if(map[x-1][y+1].data == 'M')
                mine++;
            if(map[x][y+1].data == 'M')
                mine++;
            if(map[x+1][y+1].data == 'M')
                mine++;
            map[x][y].data = mine;
        }
    }
}

/*-------------------------------*
 *                               *
 *    给定一个位置，本函数将会   *
 *    以该位置为起点递归揭示所   *
 *    有周围的 "0"方块。         *
 *                               *
 *-------------------------------*/
void reveal(int posX, int posY)
{
    //到达边界自动退出递归
    if(posX == 0 || posY == 0 || posY == size_col + 1 || posX == size_ln + 1)
        return;
    //向八个方向依次寻找，如果是空位置，继续递归，如果是数字1~9，揭示并停止递归
    if(map[posX][posY].data == 0 && !map[posX][posY].isVisible && !map[posX][posY].flag)
    {
        map[posX][posY].isVisible = 1;
        reveal(posX - 1, posY - 1);//左上
        reveal(posX, posY - 1);//上
        reveal(posX + 1, posY - 1);//右上
        reveal(posX + 1, posY);//左
        reveal(posX - 1, posY);//右
        reveal(posX - 1, posY + 1);//左下
        reveal(posX, posY + 1);//下
        reveal(posX + 1, posY + 1);//右下
    }
    else
    {
        if(!map[posX][posY].flag)
            map[posX][posY].isVisible = 1;
    }
}

/*-------------------------------*
 *                               *
 *    判断当前是否已经游戏胜利   *
 *    胜利判定如下:              *
 *    1.Flag数量等于未揭示方格   *
 *    的数量。                   *
 *    ==地图上只剩下已插旗方格   *
 *                               *
 *-------------------------------*/
int win()
{
    int x, y;
    int flaged = 0;
    int invisible = 0;
    for(x = 1; x <= size_ln; x++)
    {
        for(y = 1; y <= size_col; y++)
        {
            if(map[x][y].data == 'F')
                flaged++;
            if(map[x][y].isVisible == 0)
                invisible++;
        }
    }
    if(invisible == mine)
    {
        flag = 0;
        return 1;
    }
    return 0;

}

/*-------------------------------*
 *                               *
 *    揭示全图的格子，接收参数   *
 *    win.如果为1,则将所有的雷   *
 *    区插旗，为0,什么也不做。   *
 *                               *
 *-------------------------------*/
void revealall(int win)
{
    int x, y;
    for(x = 1; x <= size_ln; x++)
    {
        for(y = 1; y <= size_col; y++)
        {
            if(map[x][y].data == 'M' && win)
                map[x][y].flag = 1;
            else
                map[x][y].isVisible = 1;
        }
    }
}

/*-------------------------------*
 *                               *
 *    翻牌函数，对应扫雷的左     *
 *    右键一块按的功能。         *
 *                               *
 *-------------------------------*/

int tryit(int posX, int posY, char data)
{
    //八个方向单独考虑
    int flags = 0;
    int havemine = 0;
    if(map[posX-1][posY-1].flag)
        flags++;
    if(map[posX][posY-1].flag)
        flags++;
    if(map[posX+1][posY-1].flag)
        flags++;
    if(map[posX-1][posY].flag)
        flags++;
    if(map[posX+1][posY].flag)
        flags++;
    if(map[posX-1][posY+1].flag)
        flags++;
    if(map[posX][posY+1].flag)
        flags++;
    if(map[posX+1][posY+1].flag)
        flags++;
    //8个方向都要考虑，NM炸了哦
    if(data <= flags)
    {
        if(!map[posX-1][posY-1].isVisible && !map[posX-1][posY-1].flag)
        {
            if(map[posX-1][posY-1].data == 'M')
                havemine = 1;
            if(map[posX-1][posY-1].data == 0)
                reveal(posX - 1, posY - 1);
            map[posX-1][posY-1].isVisible = 1;
        }
        if(!map[posX][posY-1].isVisible && !map[posX][posY-1].flag)
        {
            if(map[posX][posY-1].data == 'M')
                havemine = 1;
            if(map[posX][posY-1].data == 0)
                reveal(posX, posY - 1);
            map[posX][posY-1].isVisible = 1;
        }
        if(!map[posX+1][posY-1].isVisible && !map[posX+1][posY-1].flag)
        {
            if(map[posX+1][posY-1].data == 'M')
                havemine = 1;
            if(map[posX+1][posY-1].data == 0)
                reveal(posX + 1, posY + 1);
            map[posX+1][posY-1].isVisible = 1;
        }
        if(!map[posX-1][posY].isVisible && !map[posX-1][posY].flag)
        {
            if(map[posX-1][posY].data == 'M')
                havemine = 1;
            if(map[posX-1][posY].data == 0)
                reveal(posX - 1, posY);
            map[posX-1][posY].isVisible = 1;
        }
        if(!map[posX+1][posY].isVisible && !map[posX+1][posY].flag)
        {
            if(map[posX+1][posY].data == 'M')
                havemine = 1;
            if(map[posX+1][posY].data == 0)
                reveal(posX + 1, posY);
            map[posX+1][posY].isVisible = 1;
        }
        if(!map[posX-1][posY+1].isVisible && !map[posX-1][posY+1].flag)
        {
            if(map[posX-1][posY+1].data == 'M')
                havemine = 1;
            if(map[posX-1][posY+1].data == 0)
                reveal(posX - 1, posY + 1);
            map[posX-1][posY+1].isVisible = 1;
        }
        if(!map[posX][posY+1].isVisible && !map[posX][posY+1].flag)
        {
            if(map[posX][posY+1].data == 'M')
                havemine = 1;
            if(map[posX][posY+1].data == 0)
                reveal(posX, posY + 1);
            map[posX][posY+1].isVisible = 1;
        }
        if(!map[posX+1][posY+1].isVisible && !map[posX+1][posY+1].flag)
        {
            if(map[posX+1][posY+1].data == 'M')
                havemine = 1;
            if(map[posX+1][posY+1].data == 0)
                reveal(posX + 1, posY + 1);
            map[posX+1][posY+1].isVisible = 1;
        }
        if(havemine)
            return 1;
        else
            return 0;
    }
    return 0;
}

/*-------------------------------*
 *                               *
 *    太艹了，每次都要判定八个   *
 *    方向，代码量直接就上去了   *
 *    WDNMD。                    *
 *                               *
 *-------------------------------*/
void justJoke()
{
    printf("你好，这里是HanxvenMarvels！\n");
}
