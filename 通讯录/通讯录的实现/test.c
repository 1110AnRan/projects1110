#include "Contact.h"

enum function
{
	EXIT=0,
	ADD,
	DEL,
	SER,
	MOD,
	SOR,
	PRF
};
void menu()
{
	printf("****************************\n");
	printf("*******1.ADD    2.DEL*******\n");
	printf("*******3.SER    4.MOD*******\n");
	printf("*******5.SOR    6.PRF*******\n");
	printf("********   0.EXIT   ********\n");
	printf("****************************\n");
}

void test()
{
	contact* c = BuyNewContact();
	int input = 0;
	InitContact(&c);
	do
	{
		menu();
		printf("������Ҫ���еĲ���:>");
		scanf("%d", &input);
		switch (input)
		{
		case ADD:
			AddContact(&c);
			break;
		case DEL:
			DeleteContact(&c);
			break;
		case SER:
			SearchContact(&c);
			break;
		case MOD:
			ModifyContact(&c);
			break;
		case SOR:
			//SortContact(&c);
			break;
		case PRF:
			ShowContact(&c);
			break;
		case EXIT:
			printf("�˳��ɹ���\n");
			break;
		default:
			printf("ѡ�����������ѡ��:>\n");
			break;
		}

	} while (input);
}

int main()
{
	test();
	return 0;
}

