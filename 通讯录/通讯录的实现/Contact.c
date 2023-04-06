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
		printf("������Ҫ���ӵ�ѧ������:>");
		scanf("%s", cur->s.name);
		printf("������Ҫ���ӵ�ѧ���Ա�:>");
		scanf("%s", cur->s.sex);
		printf("������Ҫ���ӵ�ѧ������:>");
		scanf("%d", &(cur->s.age));
		printf("������Ҫ���ӵ�ѧ���绰:>");
		scanf("%s", cur->s.tele);
		printf("������Ҫ���ӵ�ѧ����ַ:>");
		scanf("%s", cur->s.addr);
	}
	else
	{
		contact* NewStudent = BuyNewContact();
		cur->next = NewStudent;
		printf("������Ҫ���ӵ�ѧ������:>");
		scanf("%s", cur->s.name);
		printf("������Ҫ���ӵ�ѧ���Ա�:>");
		scanf("%s", cur->s.sex);
		printf("������Ҫ���ӵ�ѧ������:>");
		scanf("%d", &(cur->s.age));
		printf("������Ҫ���ӵ�ѧ���绰:>");
		scanf("%s", cur->s.tele);
		printf("������Ҫ���ӵ�ѧ����ַ:>");
		scanf("%s", cur->s.addr);
		
	}
	//if (cur == c)
	//{
	//	printf("������Ҫ���ӵ�ѧ������:>");
	//	scanf("%s", cur->s.name);
	//	printf("������Ҫ���ӵ�ѧ���Ա�:>");
	//	scanf("%s", cur->s.sex);
	//	printf("������Ҫ���ӵ�ѧ������:>");
	//	scanf("%d", &(cur->s.age));
	//	printf("������Ҫ���ӵ�ѧ���绰:>");
	//	scanf("%s", cur->s.tele);
	//	printf("������Ҫ���ӵ�ѧ����ַ:>");
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
	//		printf("������Ҫ���ӵ�ѧ������:>");
	//		scanf("%s", cur->s.name);
	//		printf("������Ҫ���ӵ�ѧ���Ա�:>");
	//		scanf("%s", cur->s.sex);
	//		printf("������Ҫ���ӵ�ѧ������:>");
	//		scanf("%d", &(cur->s.age));
	//		printf("������Ҫ���ӵ�ѧ���绰:>");
	//		scanf("%s", cur->s.tele);
	//		printf("������Ҫ���ӵ�ѧ����ַ:>");
	//		scanf("%s", cur->s.addr);
	//	}

	//}
	printf("���ӳɹ�!����н������Ĳ���!\n");
}

void ShowContact(contact** c)
{
	assert(*c);

	contact* cur = *c;
	printf("%-20s %-10s %-5s %-12s %-15s\n", "����", "�Ա�", "����", "�绰", "��ַ");
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
	printf("ɾ���ɹ�!����н������Ĳ���!\n");
}

void SearchContact(contact** c)
{
	assert(*c);
	printf("������Ҫ����ѧ��������:>");
	char SearchName[MAX_NAME] = { 0 };
	scanf("%s", SearchName);
	contact* cur = *c;
	while (strcmp(cur->s.name, SearchName) != 0)
	{
		cur = cur->next;
	}
	printf("�ò����˵���ϢΪ:\n");
	printf("%-20s %-10s %-5s %-12s %-15s\n", "����", "�Ա�", "����", "�绰", "��ַ");
	printf("%-20s %-10s %-5d %-12s %-15s\n", cur->s.name, cur->s.sex, cur->s.age, cur->s.tele, cur->s.addr);
}

void ModifyContact(contact** c)
{
	assert(*c);

	char modify[21] = { 0 };
	SearchContact(*c);

	printf("������Ҫ�޸��˵���Ϣ:>");
	scanf("%s", modify);

	if (strcmp(modify, "����")==0)
	{
		printf("�������޸�Ϊ:>");
		scanf("%s", (*c)->s.name);
	}
	else if (strcmp(modify, "�Ա�") == 0)
	{
		printf("���Ա��޸�Ϊ:>");
		scanf("%s", (*c)->s.sex);
	}
	else if (strcmp(modify, "����") == 0)
	{
		printf("�������޸�Ϊ:>");
		scanf("%d", &((*c)->s.age));
	}
	else if (strcmp(modify, "�绰") == 0)
	{
		printf("���绰�޸�Ϊ:>");
		scanf("%s", (*c)->s.tele);
	}
	else if (strcmp(modify, "��ַ") == 0)
	{
		printf("����ַ�޸�Ϊ:>");
		scanf("%s", (*c)->s.addr);
	}
	printf("�޸ĳɹ�!\n");
}

