#include "game.h"

//�˵�
void menu()
{
	printf("******************************\n");
	printf("*******1.play       0.exit****\n");
	printf("******************************\n");
}

//gameʵ�ֺ���
void game()
{
	char ret = 0;
	//���������Ϣ
	char board[ROW][COL] = { 0 };
	//��ʼ������
	InitBoard(board, ROW, COL);
	//��ӡ����
	DidplayBoard(board, ROW, COL);
	while (1)
	{
		//�������
		PlayerMove(board,ROW,COL);
		DidplayBoard(board, ROW, COL);
		//�ж�����Ƿ�Ӯ
		//��������
		ret = IsWin(board, ROW, COL);
		if (ret != 'C')
		{
			break;
		}
		ComputerMove(board,ROW,COL);
		DidplayBoard(board, ROW, COL);
		//�жϵ����Ƿ�Ӯ
		ret = IsWin(board, ROW, COL);
		if (ret != 'C')
		{
			break;
		}

	}
	if (ret == '#')
	{
		printf("���Ӯ��\n");
	}
	else if (ret == '*')
	{
		printf("����Ӯ�������Ŭ��\n");
	}
	else
	{
		printf("ƽ�֣�\n");
	}

}
void test()
{
	int input = 0;
	srand((unsigned int)time(NULL));
	do
	{
		menu();
		printf("��ѡ��:>");
		scanf("%d", &input);
		switch (input)
		{
		case 1:
			game();
			break;
		case 0:
			printf("�˳���Ϸ�ɹ���\n");
			break;
		default:
			printf("ѡ�����������ѡ��\n");
			break;
		}
		
	} while (input);
}


int main()
{
	test();
	return 0;
}