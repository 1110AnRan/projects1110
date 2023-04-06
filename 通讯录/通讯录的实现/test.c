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
		printf("请输入要进行的操作:>");
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
			printf("退出成功！\n");
			break;
		default:
			printf("选择错误，请重新选择:>\n");
			break;
		}

	} while (input);
}

int main()
{
	test();
	return 0;
}

