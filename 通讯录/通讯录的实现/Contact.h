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


//初始化通讯录
void InitContact(contact** c);

//创建一个新学生信息
contact* BuyNewContact();

//增加学生
void AddContact(contact** c);

//打印学生信息
void ShowContact(contact** c);

//删除学生
void DeleteContact(contact** c);

//查找学生
void SearchContact(contact** c);

//修改学生信息
void ModifyContact(contact** c);

//对学生进行排序
//void SortContact(contact* c);