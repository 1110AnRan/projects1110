


#include<stdio.h>
#include<Windows.h>
#include<stdlib.h>
#include<time.h>
#include<conio.h>

#define ROW 29	//��Ϸ������
#define COL 20	//��Ϸ������

#define DOWN 80		//��������£����٣�
#define LEFT 75		//���������
#define RIGHT 77	//���������

#define SPACE 32	//�ո�
#define ESC 27		//ESCss

struct Face
{
	int data[ROW][COL + 10];	//���ָ��λ���Ƿ��з���
	int color[ROW][COL + 10];	//���ָ��λ�õķ�����ɫ
}face;

struct Block
{
	int space[4][4];
}block[7][4];	//�洢�������͵ķ����������̬

//���ع��
void HideCursor();

//�����ת
void CursorJump(int x, int y);

//��ʼ����Ϸ����
void InitInterface();

//��ʼ��������Ϣ
void InitBlockInfo();

//������ɫ
void color(int x);

//������(����shape�ֵĵ�from��̬��ӡ������x��yָ���Ƿ����һ�е�һ�д�ӡ��λ��)
void DrawBlock(int shape, int form, int x, int y);

//�ո񸲸�
void DrawSpace(int shape, int form, int x, int y);

//�жϺϷ���
int IsLegal(int shape, int form, int x, int y);

//�жϵ÷ֺͽ���
int JudeFunc();

//��ʼ��Ϸ
void StartGame();


//���ļ���ȡ��߷�
void ReadGrade();

//������ߵ÷�
void UpdataGrade();

int max, grade; //ȫ�ֱ���
int main()
{
#pragma warning (disable:4996) //��������
	max = 0, grade = 0; //��ʼ������
	system("title ����˹����"); //����cmd���ڵ�����
	system("mode con lines=29 cols=60"); //����cmd���ڵĴ�С
	HideCursor(); //���ع��
	ReadGrade(); //���ļ���ȡ��߷ֵ�max����	
	InitInterface(); //��ʼ������
	InitBlockInfo(); //��ʼ��������Ϣ
	srand((unsigned int)time(NULL)); //������������ɵ����
	StartGame(); //��ʼ��Ϸ
	return 0;
}


void HideCursor()
{
	CONSOLE_CURSOR_INFO curInfo;//��������Ϣ�Ľṹ��
	curInfo.dwSize = 1;			//���δ��ֵ����������Ч
	curInfo.bVisible = FALSE;	//���������Ϊ���ɼ�
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);//��ȡ����̨���
	SetConsoleCursorInfo(handle, &curInfo);//���ù����Ϣ
}

void CursorJump(int x, int y)
{
	COORD pos;					//������λ�õĽṹ�����
	pos.X = x;
	pos.Y = y;
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);//��ȡ����̨���
	SetConsoleCursorPosition(handle, pos);//���ù��λ��
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
				face.data[i][j] = 1;	//��Ǹ�λ���з���
				CursorJump(2 * j, i);
				printf("��");
			}
			else if(i == ROW - 1)
			{
				face.data[i][j] = 1;
				printf(" ��");
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
		printf("��");
	}


	CursorJump(2 * COL, 1);
	printf("��һ�����飺");

	CursorJump(2 * COL + 4, ROW - 19);
	printf("���ƣ���");

	CursorJump(2 * COL + 4, ROW - 17);
	printf("���ƣ���");

	CursorJump(2 * COL + 4, ROW - 15);
	printf("���٣���");

	CursorJump(2 * COL + 4, ROW - 13);
	printf("��ת���ո�");

	CursorJump(2 * COL + 4, ROW - 11);
	printf("��ͣ: S");

	CursorJump(2 * COL + 4, ROW - 9);
	printf("�˳�: Esc");

	CursorJump(2 * COL + 4, ROW - 7);
	printf("���¿�ʼ:R");

	CursorJump(2 * COL + 4, ROW - 5);
	printf("��߼�¼:%d", max);

	CursorJump(2 * COL + 4, ROW - 3);
	printf("��ǰ������%d", grade);
}

void InitBlockInfo()
{
	//��һ����̬
	//"T"��
	for (int i = 0; i <= 2; ++i)
	{
		block[0][0].space[1][i] = 1;
	}
	block[0][0].space[2][1] = 1;

	//"L"��
	for (int i = 1; i <= 3; ++i)
	{
		block[1][0].space[i][1] = 1;
	}
	block[1][0].space[3][2] = 1;

	//"J"��
	for (int i = 1; i <= 3; ++i)
	{
		block[2][0].space[i][2] = 1;
	}
	block[2][0].space[3][1] = 1;

	for (int i = 0; i <= 1; ++i)
	{
		//"Z"��
		block[3][0].space[1][i] = 1;
		block[3][0].space[2][i + 1] = 1;


		//"S"��
		block[4][0].space[1][i + 1] = 1;
		block[4][0].space[2][i] = 1;


		//"O"��
		block[5][0].space[1][i + 1] = 1;
		block[5][0].space[2][i + 1] = 1;
	}

	//"I"��
	for (int i = 0; i <= 3; ++i)
	{
		block[6][0].space[i][1] = 1;
	}


	//ת��Ϊ������̬
	int temp[4][4];
	for (int shape = 0; shape < 7; shape++)
	{
		for (int form = 0; form < 3; form++)
		{

			//�����shape�ֵĵ�form״̬
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
		c = 13; //��T���η�������Ϊ��ɫ
		break;
	case 1:
	case 2:
		c = 12; //��L���κ͡�J���η�������Ϊ��ɫ
		break;
	case 3:
	case 4:
		c = 10; //��Z���κ͡�S���η�������Ϊ��ɫ
		break;
	case 5:
		c = 14; //��O���η�������Ϊ��ɫ
		break;
	case 6:
		c = 11; //��I���η�������Ϊǳ��ɫ
		break;
	default:
		c = 7; //����Ĭ������Ϊ��ɫ
		break;
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c); //��ɫ����
	//ע��SetConsoleTextAttribute��һ��API��Ӧ�ó����̽ӿڣ�
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
				printf("��");
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
				printf("  ");//�����ո�
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
	//�жϵ÷�
	for (int i = ROW - 2; i > 4; i--)
	{
		int sum = 0;
		for(int j = 1;j < COL - 1;j++)
		{
			sum += face.data[i][j];//ͳ�Ƶ�i�з���ĸ���
		}
		if (sum == 0)
		{
			break;
		}
		if (sum == COL - 2)
		{
			grade += 10;//ÿ�ε÷�+10
			color(7);//������ɫΪ��ɫ
			CursorJump(2 * COL + 4, ROW - 3);//�����ת
			printf("��ǰ����: %d", grade);
			for (int j = 1; j < COL - 1; j++)
			{
				face.data[i][j] = 0;
				CursorJump(2 * j, i);
				printf("  ");
			}
			//�ѱ����������������������ƶ�
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
						printf("��");
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

	//�ж��Ƿ����
	for (int j = 1; j < COL - 1; j++)
	{
		if (face.data[1][j] == 1)	//�������з���ͽ���
		{
			Sleep(1000);
			system("cls");
			CursorJump(2 * (COL / 3), (ROW / 2) - 3);
			if (grade > max)
			{
				printf("��ϲ�������߼�¼����ǰ��ߵ÷ָ���Ϊ:%d", grade);
				UpdataGrade();
			}
			else if (grade == max)
			{
				printf("��ǰ�÷� %d ����߼�¼��ƽ", grade);
			}
			else
			{
				printf("�������ͣ���ǰ�÷�:%d������߷����:%d", grade, max - grade);
			}
			CursorJump(2 * (COL / 3), ROW / 2);
			printf("GAME OVER");
			while (1)
			{
				char ch;
				CursorJump(2 * (COL / 3), ROW / 2 + 3);
				printf("����Ҫ����һ����:yes or no?");
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
					printf("ѡ�����������ѡ��");
				}
			}

		}
	}
	return 0;
}

void StartGame()
{
	int shape = rand() % 7;
	int form = rand() % 4;//�����ȡ�������״����̬
	while (1)
	{
		int t = 0;
		int nextshape = rand() % 7;
		int nextform = rand() % 4;
		int x = COL / 2 - 2, y = 0;//�����ʼ����λ�õĺ�������
		color(nextshape);
		DrawBlock(nextshape, nextform, COL + 3, 3);//����һ��������ʾ�����Ͻ�
		while (1)
		{
			color(shape);
			DrawBlock(shape, form, x, y);
			if (t == 0)
			{
				t = 15000;//tԽС�������ٶ�Խ��
			}
			while (--t)
			{
				if (kbhit() != 0)//���̱��û�
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
					printf("  ��Ϸ����  ");
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
	FILE* pf = fopen("����˹������ߵ÷ּ�¼.txt", "r");//��ֻ���ķ�ʽ���ļ�
	if (pf == NULL)//��ʧ��
	{
		pf = fopen("����˹������ߵ÷ּ�¼.txt", "w");	//��ֻд�ķ�ʽ���ļ�
		fwrite(&grade, sizeof(int), 1, pf);
	}
	fseek(pf, 0, SEEK_SET);//ʹ�ļ�ָ��pfָ���ļ���ͷ
	fread(&max, sizeof(int), 1, pf);					//��ֻ���ķ�ʽ��ȡ�����ʷ�ֵ�max��
	fclose(pf);//�ر��ļ�
	pf = NULL;
}

void UpdataGrade()
{
	FILE* pf = fopen("����˹������ߵ÷ּ�¼.txt", "w");
	if (pf == NULL)
	{
		printf("������ߵ÷ּ�¼ʧ��\n");
		exit(0);
	}
	fwrite(&grade, sizeof(int), 1, pf);
	fclose(pf);
	pf = NULL;
}

