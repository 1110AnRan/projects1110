#pragma once
#define _CRT_SECURE_NO_WARNINGS 1
//�����㷨��ʵ��
#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<string.h>

// ����ʮ�������
void CreatRandomNumbers(int* a);
// ���Ը��������㷨��Ч��
void TestOP();

// ��ӡ����
void PrintArr(int* a, int n);

// ��������
void Swap(int* a1, int* a2);

// ��������
void InsertSort(int* a, int n);

// ϣ������
void ShellSort(int* a, int n);

// ������
void HeapSort(int* a, int n);

// ����
void AdjustDown(int* a, int n, int root);

// ѡ������
void SelectSort(int* a, int n);

// ��������(���ַ���)
// ��һ�֣���ӷ�
int QuickSortPart1(int* a, int left, int right);

//�ڶ��֣�ǰ��ָ�뷨
int QuickSortPart2(int* a, int left, int right);

//�����֣�ǰ��ָ�뷨
int QuickSortPart3(int* a, int left, int right);

//�Ż�������ȡ��
int GetMidIndex(int* a, int left, int right);

void QuickSort(int* a, int left, int right);

//�鲢����
void MergeSort(int* a, int n);

void _MergeSort(int* a, int left, int right, int* tmp);

//ð������
void BubbleSort(int* a, int n);

//��������
void CountSort(int* a, int n);