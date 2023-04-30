#pragma once
#define _CRT_SECURE_NO_WARNINGS 1
//排序算法的实现
#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<string.h>

// 生成十个随机数
void CreatRandomNumbers(int* a);
// 测试各种排序算法的效率
void TestOP();

// 打印数组
void PrintArr(int* a, int n);

// 交换两数
void Swap(int* a1, int* a2);

// 插入排序
void InsertSort(int* a, int n);

// 希尔排序
void ShellSort(int* a, int n);

// 堆排序
void HeapSort(int* a, int n);

// 建堆
void AdjustDown(int* a, int n, int root);

// 选择排序
void SelectSort(int* a, int n);

// 快速排序(三种方法)
// 第一种，填坑法
int QuickSortPart1(int* a, int left, int right);

//第二种，前后指针法
int QuickSortPart2(int* a, int left, int right);

//第三种，前后指针法
int QuickSortPart3(int* a, int left, int right);

//优化，三数取中
int GetMidIndex(int* a, int left, int right);

void QuickSort(int* a, int left, int right);

//归并排序
void MergeSort(int* a, int n);

void _MergeSort(int* a, int left, int right, int* tmp);

//冒泡排序
void BubbleSort(int* a, int n);

//计数排序
void CountSort(int* a, int n);