#include "Contact.h"

void InitContact(contact** c)
{
	assert(*c);
	(*c)->next = NULL;
}

contact* BuyNewContact()
{
	contact* NewStudent = (contact*)malloc(sizeof(contact));
	if (NewStudent == NULL)
	{
		printf("Malloc Failed\n");
		exit(-1);
	}
	NewStudent->next = NULL;
	return NewStudent;
}
void AddContact(contact** c)
{
	assert(*c);

	contact* cur = *c;
	while (cur->next)
	{
		cur = cur->next;
	}
	if (cur == *c)
	{
		printf("请输入要增加的学生姓名:>");
		scanf("%s", cur->s.name);
		printf("请输入要增加的学生性别:>");
		scanf("%s", cur->s.sex);
		printf("请输入要增加的学生年龄:>");
		scanf("%d", &(cur->s.age));
		printf("请输入要增加的学生电话:>");
		scanf("%s", cur->s.tele);
		printf("请输入要增加的学生地址:>");
		scanf("%s", cur->s.addr);
	}
	else
	{
		contact* NewStudent = BuyNewContact();
		cur->next = NewStudent;
		printf("请输入要增加的学生姓名:>");
		scanf("%s", cur->s.name);
		printf("请输入要增加的学生性别:>");
		scanf("%s", cur->s.sex);
		printf("请输入要增加的学生年龄:>");
		scanf("%d", &(cur->s.age));
		printf("请输入要增加的学生电话:>");
		scanf("%s", cur->s.tele);
		printf("请输入要增加的学生地址:>");
		scanf("%s", cur->s.addr);
		
	}
	//if (cur == c)
	//{
	//	printf("请输入要增加的学生姓名:>");
	//	scanf("%s", cur->s.name);
	//	printf("请输入要增加的学生性别:>");
	//	scanf("%s", cur->s.sex);
	//	printf("请输入要增加的学生年龄:>");
	//	scanf("%d", &(cur->s.age));
	//	printf("请输入要增加的学生电话:>");
	//	scanf("%s", cur->s.tele);
	//	printf("请输入要增加的学生地址:>");
	//	scanf("%s", cur->s.addr);
	//}
	//else
	//{
	//	contact* NewStudent = (contact*)malloc(sizeof(contact));
	//	if (NewStudent == NULL)
	//	{
	//		printf("Malloc Failed\n");
	//		exit(-1);
	//	}
	//	else
	//	{
	//		NewStudent->next = NULL;
	//		cur->next = NewStudent;
	//		cur = cur->next;
	//		printf("请输入要增加的学生姓名:>");
	//		scanf("%s", cur->s.name);
	//		printf("请输入要增加的学生性别:>");
	//		scanf("%s", cur->s.sex);
	//		printf("请输入要增加的学生年龄:>");
	//		scanf("%d", &(cur->s.age));
	//		printf("请输入要增加的学生电话:>");
	//		scanf("%s", cur->s.tele);
	//		printf("请输入要增加的学生地址:>");
	//		scanf("%s", cur->s.addr);
	//	}

	//}
	printf("增加成功!请进行接下来的操作!\n");
}

void ShowContact(contact** c)
{
	assert(*c);

	contact* cur = *c;
	printf("%-20s %-10s %-5s %-12s %-15s\n", "姓名", "性别", "年龄", "电话", "地址");
	while (cur)
	{
		printf("%-20s %-10s %-5d %-12s %-15s\n", cur->s.name, cur->s.sex, cur->s.age, cur->s.tele, cur->s.addr);
		cur = cur->next;
	}

}

void DeleteContact(contact** c)
{
	assert(*c);
	if ((*c)->next == NULL)
	{
		free(*c);
		*c = NULL;
	}
	else
	{
		contact* cur = c;
		contact* prev = NULL;
		while (cur->next)
		{
			prev = cur;
			cur = cur->next;
		}
		free(cur);
		cur = NULL;
		prev->next = NULL;
	}
	printf("删除成功!请进行接下来的操作!\n");
}

void SearchContact(contact** c)
{
	assert(*c);
	printf("请输入要查找学生的姓名:>");
	char SearchName[MAX_NAME] = { 0 };
	scanf("%s", SearchName);
	contact* cur = *c;
	while (strcmp(cur->s.name, SearchName) != 0)
	{
		cur = cur->next;
	}
	printf("该查找人的信息为:\n");
	printf("%-20s %-10s %-5s %-12s %-15s\n", "姓名", "性别", "年龄", "电话", "地址");
	printf("%-20s %-10s %-5d %-12s %-15s\n", cur->s.name, cur->s.sex, cur->s.age, cur->s.tele, cur->s.addr);
}

void ModifyContact(contact** c)
{
	assert(*c);

	char modify[21] = { 0 };
	SearchContact(*c);

	printf("请输入要修改人的信息:>");
	scanf("%s", modify);

	if (strcmp(modify, "姓名")==0)
	{
		printf("将姓名修改为:>");
		scanf("%s", (*c)->s.name);
	}
	else if (strcmp(modify, "性别") == 0)
	{
		printf("将性别修改为:>");
		scanf("%s", (*c)->s.sex);
	}
	else if (strcmp(modify, "年龄") == 0)
	{
		printf("将年龄修改为:>");
		scanf("%d", &((*c)->s.age));
	}
	else if (strcmp(modify, "电话") == 0)
	{
		printf("将电话修改为:>");
		scanf("%s", (*c)->s.tele);
	}
	else if (strcmp(modify, "地址") == 0)
	{
		printf("将地址修改为:>");
		scanf("%s", (*c)->s.addr);
	}
	printf("修改成功!\n");
}

