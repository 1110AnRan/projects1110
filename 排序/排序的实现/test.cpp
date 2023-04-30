#include"Sort.h"

//≤‚ ‘≤Â»Î≈≈–Ú
void TestInsertSort()
{
	int a[] = { 49, 38, 65, 97, 76, 13, 27, 49, 13, 27, 49 };
	//CreatRandomNumbers(a);
	InsertSort(a, sizeof(a) / sizeof(int));
	PrintArr(a, sizeof(a) / sizeof(int));
}

//≤‚ ‘œ£∂˚≈≈–Ú
void TestShellSort()
{
	int a[] = { 49, 38, 65, 97, 76, 13, 27, 49, 13, 27, 49 };
	//CreatRandomNumbers(a);
	ShellSort(a, sizeof(a) / sizeof(int));
	PrintArr(a, sizeof(a) / sizeof(int));
}

//≤‚ ‘∂—≈≈–Ú
void TestHeapSort()
{
	int a[] = { 49, 38, 65, 97, 76, 13, 27, 49, 13, 27, 49 };
	//CreatRandomNumbers(a);
	HeapSort(a, sizeof(a) / sizeof(int));
	PrintArr(a, sizeof(a) / sizeof(int));
}

//≤‚ ‘—°‘Ò≈≈–Ú
void TestSelectSort()
{
	int a[] = { 49, 38, 65, 97, 76, 13, 27, 49, 13, 27, 49 };
	//CreatRandomNumbers(a);
	//SelectSort(a, sizeof(a) / sizeof(int));
	PrintArr(a, sizeof(a) / sizeof(int));
}

//≤‚ ‘øÏÀŸ≈≈–Ú
void TestQuickSort()
{
	int a[] = { 49, 38, 65, 97, 76, 13, 27, 49, 13, 27, 49 };
	//CreatRandomNumbers(a);
	QuickSort(a, 0, sizeof(a) / sizeof(int) - 1);
	PrintArr(a, sizeof(a) / sizeof(int));
}

//≤‚ ‘πÈ≤¢≈≈–Ú
void TestMergeSort()
{
	int a[] = { 49, 38, 65, 97, 76, 13, 27, 49, 13, 27, 49 };
	//CreatRandomNumbers(a);
	MergeSort(a, sizeof(a) / sizeof(int));
	PrintArr(a, sizeof(a) / sizeof(int));
}

//≤‚ ‘√∞≈›≈≈–Ú
void TestBubbleSort()
{
	int a[] = { 49, 38, 65, 97, 76, 13, 27, 49, 13, 27, 49 };
	//CreatRandomNumbers(a);
	BubbleSort(a, sizeof(a) / sizeof(int));
	PrintArr(a, sizeof(a) / sizeof(int));
}

//≤‚ ‘º∆ ˝≈≈–Ú
void TestCountSort()
{
	int a[] = { 49, 38, 65, 97, 76, 13, 27, 49, 13, 27, 49 };
	//int a[] = { 1,3,5,6,10,8,7,2,4,9 };
	//CreatRandomNumbers(a);
	CountSort(a, sizeof(a) / sizeof(int));
	PrintArr(a, sizeof(a) / sizeof(int));
}

int main()
{
	//TestInsertSort();
	//TestShellSort();
	//TestHeapSort();
	//TestSelectSort();
	//TestQuickSort();
	//TestMergeSort();
	TestCountSort();
	//TestOP();
	return 0;
}