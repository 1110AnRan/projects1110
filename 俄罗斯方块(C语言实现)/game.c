


#include<stdio.h>
#include<Windows.h>
#include<stdlib.h>
#include<time.h>
#include<conio.h>

#define ROW 29	//游戏区行数
#define COL 20	//游戏区列数

#define DOWN 80		//方向键：下（加速）
#define LEFT 75		//方向键：左
#define RIGHT 77	//方向键：右

#define SPACE 32	//空格
#define ESC 27		//ESCss

struct Face
{
	int data[ROW][COL + 10];	//标记指定位置是否有方块
	int color[ROW][COL + 10];	//标记指定位置的方块颜色
}face;

struct Block
{
	int space[4][4];
}block[7][4];	//存储七种类型的方块的四种形态

//隐藏光标
void HideCursor();

//光标跳转
void CursorJump(int x, int y);

//初始化游戏界面
void InitInterface();

//初始化方块信息
void InitBlockInfo();

//设置颜色
void color(int x);

//画方块(将第shape种的第from形态打印出来，x和y指的是方块第一行第一列打印的位置)
void DrawBlock(int shape, int form, int x, int y);

//空格覆盖
void DrawSpace(int shape, int form, int x, int y);

//判断合法性
int IsLegal(int shape, int form, int x, int y);

//判断得分和结束
int JudeFunc();

//开始游戏
void StartGame();


//从文件读取最高分
void ReadGrade();

//更新最高得分
void UpdataGrade();

int max, grade; //全局变量
int main()
{
#pragma warning (disable:4996) //消除警告
	max = 0, grade = 0; //初始化变量
	system("title 俄罗斯方块"); //设置cmd窗口的名字
	system("mode con lines=29 cols=60"); //设置cmd窗口的大小
	HideCursor(); //隐藏光标
	ReadGrade(); //从文件读取最高分到max变量	
	InitInterface(); //初始化界面
	InitBlockInfo(); //初始化方块信息
	srand((unsigned int)time(NULL)); //设置随机数生成的起点
	StartGame(); //开始游戏
	return 0;
}


void HideCursor()
{
	CONSOLE_CURSOR_INFO curInfo;//定义光标信息的结构体
	curInfo.dwSize = 1;			//如果未赋值，则隐藏无效
	curInfo.bVisible = FALSE;	//将光标设置为不可见
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);//获取控制台句柄
	SetConsoleCursorInfo(handle, &curInfo);//设置光标信息
}

void CursorJump(int x, int y)
{
	COORD pos;					//定义光标位置的结构体变量
	pos.X = x;
	pos.Y = y;
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);//获取控制台句柄
	SetConsoleCursorPosition(handle, pos);//设置光标位置
}

void InitInterface()
{
	color(7);
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL + 10; j++)
		{
			if (j == 0 || j == COL - 1 || j == COL + 9)
			{
				face.data[i][j] = 1;	//标记该位置有方块
				CursorJump(2 * j, i);
				printf("■");
			}
			else if(i == ROW - 1)
			{
				face.data[i][j] = 1;
				printf(" ■");
			}
			else
			{
				face.data[i][j] = 0;
			}
		}
	}


	for (int i = COL; i < COL + 10; i++)
	{
		face.data[8][i] = 1;
		CursorJump(2 * i, 8);
		printf("■");
	}


	CursorJump(2 * COL, 1);
	printf("下一个方块：");

	CursorJump(2 * COL + 4, ROW - 19);
	printf("左移：←");

	CursorJump(2 * COL + 4, ROW - 17);
	printf("右移：→");

	CursorJump(2 * COL + 4, ROW - 15);
	printf("加速：↓");

	CursorJump(2 * COL + 4, ROW - 13);
	printf("旋转：空格");

	CursorJump(2 * COL + 4, ROW - 11);
	printf("暂停: S");

	CursorJump(2 * COL + 4, ROW - 9);
	printf("退出: Esc");

	CursorJump(2 * COL + 4, ROW - 7);
	printf("重新开始:R");

	CursorJump(2 * COL + 4, ROW - 5);
	printf("最高纪录:%d", max);

	CursorJump(2 * COL + 4, ROW - 3);
	printf("当前分数：%d", grade);
}

void InitBlockInfo()
{
	//第一种形态
	//"T"形
	for (int i = 0; i <= 2; ++i)
	{
		block[0][0].space[1][i] = 1;
	}
	block[0][0].space[2][1] = 1;

	//"L"形
	for (int i = 1; i <= 3; ++i)
	{
		block[1][0].space[i][1] = 1;
	}
	block[1][0].space[3][2] = 1;

	//"J"形
	for (int i = 1; i <= 3; ++i)
	{
		block[2][0].space[i][2] = 1;
	}
	block[2][0].space[3][1] = 1;

	for (int i = 0; i <= 1; ++i)
	{
		//"Z"形
		block[3][0].space[1][i] = 1;
		block[3][0].space[2][i + 1] = 1;


		//"S"形
		block[4][0].space[1][i + 1] = 1;
		block[4][0].space[2][i] = 1;


		//"O"形
		block[5][0].space[1][i + 1] = 1;
		block[5][0].space[2][i + 1] = 1;
	}

	//"I"形
	for (int i = 0; i <= 3; ++i)
	{
		block[6][0].space[i][1] = 1;
	}


	//转换为其他形态
	int temp[4][4];
	for (int shape = 0; shape < 7; shape++)
	{
		for (int form = 0; form < 3; form++)
		{

			//保存第shape种的第form状态
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					temp[i][j] = block[shape][form].space[i][j];
				}
			}

			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					block[shape][form + 1].space[i][j] = temp[3 - j][i];
				}
			}
		}
	}
}

void color(int c)
{
	switch (c)
	{
	case 0:
		c = 13; //“T”形方块设置为紫色
		break;
	case 1:
	case 2:
		c = 12; //“L”形和“J”形方块设置为红色
		break;
	case 3:
	case 4:
		c = 10; //“Z”形和“S”形方块设置为绿色
		break;
	case 5:
		c = 14; //“O”形方块设置为黄色
		break;
	case 6:
		c = 11; //“I”形方块设置为浅蓝色
		break;
	default:
		c = 7; //其他默认设置为白色
		break;
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c); //颜色设置
	//注：SetConsoleTextAttribute是一个API（应用程序编程接口）
}

void DrawBlock(int shape, int form, int x, int y)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (block[shape][form].space[i][j] == 1)
			{
				CursorJump(2 * (x + j), y + i);
				printf("■");
			}
		}
	}
}

void DrawSpace(int shape, int form, int x, int y)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (block[shape][form].space[i][j] == 1)
			{
				CursorJump(2 * (x + j), y + i);
				printf("  ");//两个空格
			}
		}
	}
}

int IsLegal(int shape, int form, int x, int y)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if ((block[shape][form].space[i][j] == 1) && (face.data[y + i][x + j] == 1))
			{
				return 0;
			}
		}
	}
	return 1;
}

int JudeFunc()
{
	//判断得分
	for (int i = ROW - 2; i > 4; i--)
	{
		int sum = 0;
		for(int j = 1;j < COL - 1;j++)
		{
			sum += face.data[i][j];//统计第i行方块的个数
		}
		if (sum == 0)
		{
			break;
		}
		if (sum == COL - 2)
		{
			grade += 10;//每次得分+10
			color(7);//设置颜色为白色
			CursorJump(2 * COL + 4, ROW - 3);//光标跳转
			printf("当前分数: %d", grade);
			for (int j = 1; j < COL - 1; j++)
			{
				face.data[i][j] = 0;
				CursorJump(2 * j, i);
				printf("  ");
			}
			//把被清除行上面的行整体向下移动
			for (int m = i; m > 1; m--)
			{
				sum = 0;
				for (int n = 1; n < COL - 1; n++)
				{
					sum += face.data[m - 1][n];
					face.data[m][n] = face.data[m - 1][n];
					face.color[m][n] = face.color[m - 1][n];
					if (face.data[m][n] == 1)
					{
						CursorJump(2 * n, m);
						color(face.color[m][n]);
						printf("■");
					}
					else
					{
						CursorJump(2 * n, m);
						printf("  ");
					}
				}
				if (sum == 0)
					return 1;
			}
		}

	}

	//判断是否结束
	for (int j = 1; j < COL - 1; j++)
	{
		if (face.data[1][j] == 1)	//最上面有方块就结束
		{
			Sleep(1000);
			system("cls");
			CursorJump(2 * (COL / 3), (ROW / 2) - 3);
			if (grade > max)
			{
				printf("恭喜你打破最高记录，当前最高得分更新为:%d", grade);
				UpdataGrade();
			}
			else if (grade == max)
			{
				printf("当前得分 %d 与最高记录持平", grade);
			}
			else
			{
				printf("继续加油！当前得分:%d，与最高分相差:%d", grade, max - grade);
			}
			CursorJump(2 * (COL / 3), ROW / 2);
			printf("GAME OVER");
			while (1)
			{
				char ch;
				CursorJump(2 * (COL / 3), ROW / 2 + 3);
				printf("你想要再来一局吗？:yes or no?");
				scanf("%c", &ch);
				if (ch == 'y' || ch == 'Y')
				{
					system("cls");
					main();
				}
				else if (ch == 'n' || ch == 'N')
				{
					CursorJump(2 * (COL / 3), ROW / 2 + 5);
					exit(0);
				}
				else
				{
					CursorJump(2 * (COL / 3), ROW / 2 + 4);
					printf("选择错误，请重新选择");
				}
			}

		}
	}
	return 0;
}

void StartGame()
{
	int shape = rand() % 7;
	int form = rand() % 4;//随机获取方块的形状和形态
	while (1)
	{
		int t = 0;
		int nextshape = rand() % 7;
		int nextform = rand() % 4;
		int x = COL / 2 - 2, y = 0;//方块初始下落位置的横纵坐标
		color(nextshape);
		DrawBlock(nextshape, nextform, COL + 3, 3);//将下一个方块显示在右上角
		while (1)
		{
			color(shape);
			DrawBlock(shape, form, x, y);
			if (t == 0)
			{
				t = 15000;//t越小，下落速度越快
			}
			while (--t)
			{
				if (kbhit() != 0)//键盘被敲击
				{
					break;
				}
			}
			if (t == 0)
			{
				if (IsLegal(shape, form, x, y + 1) == 0)
				{
					for (int i = 0; i < 4; i++)
					{
						for (int j = 0; j < 4; j++)
						{
							if (block[shape][form].space[i][j] == 1)
							{
								face.data[y + i][x + j] = 1;
								face.color[y + i][x + j] = shape;
							}
						}
					}
					while (JudeFunc());
					break;
				}
				else
				{
					DrawSpace(shape, form, x, y);
					y++;
				}
			}
			else
			{
				char ch = getch();
				switch (ch)
				{
				case DOWN:
					if (IsLegal(shape, form, x, y + 1) == 1)
					{
						DrawSpace(shape, form, x, y);
						++y;
					}
					break;
				case LEFT:
					if (IsLegal(shape, form, x - 1, y) == 1)
					{
						DrawSpace(shape, form, x, y);
						--x;
					}
					break;
				case RIGHT:
					if (IsLegal(shape, form, x + 1, y) == 1)
					{
						DrawSpace(shape, form, x, y);
						++x;
					}
					break;
				case SPACE:
					if (IsLegal(shape, (form + 1) % 4, x, y + 1) == 1)
					{
						DrawSpace(shape, form, x, y);
						++y;
						form = (form + 1) % 4;
					}
					break;
				case ESC:
					system("cls");
					color(7);
					CursorJump(COL, ROW / 2);
					printf("  游戏结束  ");
					CursorJump(COL, ROW / 2 + 2);
					exit(0);
				case 's':
				case 'S':
					system("pause>nul");
					break;
				case 'r':
				case 'R':
					system("cls");
					main();
				}
			}
		}
		shape = nextshape, form = nextform;
		DrawSpace(nextshape, nextform, COL + 3, 3);
	}
}


void ReadGrade()
{
	FILE* pf = fopen("俄罗斯方块最高得分记录.txt", "r");//以只读的方式打开文件
	if (pf == NULL)//打开失败
	{
		pf = fopen("俄罗斯方块最高得分记录.txt", "w");	//以只写的方式打开文件
		fwrite(&grade, sizeof(int), 1, pf);
	}
	fseek(pf, 0, SEEK_SET);//使文件指针pf指向文件开头
	fread(&max, sizeof(int), 1, pf);					//以只读的方式读取最高历史分到max中
	fclose(pf);//关闭文件
	pf = NULL;
}

void UpdataGrade()
{
	FILE* pf = fopen("俄罗斯方块最高得分记录.txt", "w");
	if (pf == NULL)
	{
		printf("保存最高得分记录失败\n");
		exit(0);
	}
	fwrite(&grade, sizeof(int), 1, pf);
	fclose(pf);
	pf = NULL;
}

