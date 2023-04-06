#pragma once


#define _CRT_SECURE_NO_WARNINGS 1

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define MAX_NAME 20
#define MAX_SEX 10
#define MAX_TELE 12
#define MAX_ADDR 15


typedef struct Student
{
	char name[MAX_NAME];
	char sex[MAX_SEX];
	int age;
	char tele[MAX_TELE];
	char addr[MAX_ADDR];
}Student;

typedef struct Contact
{
	Student s;
	struct Contact* next;
}contact;


//��ʼ��ͨѶ¼
void InitContact(contact** c);

//����һ����ѧ����Ϣ
contact* BuyNewContact();

//����ѧ��
void AddContact(contact** c);

//��ӡѧ����Ϣ
void ShowContact(contact** c);

//ɾ��ѧ��
void DeleteContact(contact** c);

//����ѧ��
void SearchContact(contact** c);

//�޸�ѧ����Ϣ
void ModifyContact(contact** c);

//��ѧ����������
//void SortContact(contact* c);