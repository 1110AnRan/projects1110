#include"Sort.h"

void CreatRandomNumbers(int* a)
{
	srand(time(NULL));
	for (int i = 0; i < 10; i++)
	{
		a[i] = rand() % 100 + 1;
	}
}

void TestOP()
{
	srand(time(0));
	const int N = 10000000;
	int* a1 = (int*)malloc(sizeof(int) * N);
	int* a2 = (int*)malloc(sizeof(int) * N);
	int* a3 = (int*)malloc(sizeof(int) * N);
	int* a4 = (int*)malloc(sizeof(int) * N);
	int* a5 = (int*)malloc(sizeof(int) * N);
	int* a6 = (int*)malloc(sizeof(int) * N);

	for (int i = 0; i < N; ++i)
	{
		a1[i] = rand();
		a2[i] = a1[i];
		a3[i] = a1[i];
		a4[i] = a1[i];
		a5[i] = a1[i];
		a6[i] = a1[i];

	}

	int begin1 = clock();
	//InsertSort(a1, N);
	int end1 = clock();

	int begin2 = clock();
	ShellSort(a2, N);
	int end2 = clock();

	int begin3 = clock();
	//SelectSort(a3, N);
	int end3 = clock();

	int begin4 = clock();
	HeapSort(a4, N);
	int end4 = clock();

	int begin5 = clock();
	QuickSort(a5, 0, N - 1);
	int end5 = clock();

	int begin6 = clock();
	//MergeSort(a6, N);
	int end6 = clock();

	printf("InsertSort:%d\n", end1 - begin1);
	printf("ShellSort:%d\n", end2 - begin2);
	//printf("SelectSort:%d\n", end3 - begin3);
	printf("HeapSort:%d\n", end4 - begin4);
	printf("QuickSort:%d\n", end5 - begin5);
	//printf("MergeSort:%d\n", end6 - begin6);

	free(a1);
	free(a2);
	free(a3);
	free(a4);
	free(a5);
	free(a6);
}

void PrintArr(int* a, int n)
{
	int i = 0;
	for (i = 0; i < n; i++)
	{
		printf("%d ", a[i]);
	}
	printf("\n");
}

void InsertSort(int* a, int n)
{
	//默认[0,end]都为有序，把a[end+1]插入并保持[0,end + 1]为升序

	//最好的情况，时间复杂度为O(N);
	//最坏的情况，时间复杂度为O(N*N);
	for (int i = 0; i < n - 1; i++)
	{
		int end = i;
		int tmp = a[end + 1];
		while (end >= 0)
		{
			if (a[end] > tmp)
			{
				a[end + 1] = a[end];
				end--;
			}
			else
			{
				break;
			}
		}
		a[end + 1] = tmp;
	}
}

//把数组a分为几组进行预排序，使这几组变为升序
void ShellSort(int* a, int n)
{
	int gap = n;
	while (gap > 1)
	{
		//gap = gap / 3 + 1;时间复杂度为O(log3N*N);
		gap = gap / 2;//两种情况都可，这一种情况时间复杂度为O(logN);

		//与插入排序同理
		for (int i = 0; i < n - gap; i++)
		{
			int end = i;
			int tmp = a[end + gap];
			while (end >= 0)
			{
				if (tmp < a[end])
				{
					a[end + gap] = a[end];
					end -= gap;
				}
				else
				{
					break;
				}
			}
			a[end + gap] = tmp;
		}
	}
}

//首先要建堆，大堆或小堆
//若要升序，则要建大堆
//小堆会破坏堆的数据结构

//交换函数
void Swap(int* a1, int* a2)
{
	int temp = *a1;
	*a1 = *a2;
	*a2 = temp;
}

//建堆
void AdjustDown(int* a, int n, int root)
{
	int parent = root;
	int child = parent * 2 + 1;
	while (child < n)
	{
		if (child + 1 < n && a[child] < a[child + 1])	//child+1 < size 确保右孩子存在，
		{
			child += 1;										//右孩子大于左孩子，让下标指向右孩子
		}

		if (a[parent] < a[child])
		{
			Swap(&a[parent], &a[child]);					//交换父节点和孩子节点中值较大的位置
			parent = child;
			child = parent * 2 + 1;
		}
		else
		{
			break;
		}
	}
}

void HeapSort(int* a, int n)
{
	//先建大堆
	for (int i = n - 1 - 1; i >= 0; i--)
	{
		AdjustDown(a, n, i);
	}

	//把最大的放到最后，并把最后一个不看做堆里面的数据
	int end = n;
	while (end > 0)
	{
		Swap(&a[0], &a[end - 1]);
		AdjustDown(a, end - 1, 0);
		end--;
	}
}

void SelectSort(int* a, int n)
{
	int min = 0;
	for (int i = 0; i < n; i++)
	{
		min = i;
		for (int j = i + 1; j < n; j++)
		{
			if (a[min] > a[j])
			{
				min = j;					//选出数据最小的下标
			}
		}
		Swap(&a[i], &a[min]);				//把它和下标为0,1,2,3...位置进行交换
	}
}

int GetMidIndex(int* a, int left, int right)
{
	int mid = (right + left) / 2;
	if (a[left] > a[mid])
	{
		if (a[mid] > a[right])
		{
			return mid;
		}
		else
		{
			if (a[left] > a[right])
			{
				return right;
			}
			else
			{
				return left;
			}
		}
	}
	else
	{
		if (a[left] > a[right])
		{
			return left;
		}
		else
		{
			if (a[mid] > a[right])
			{
				return right;
			}
			else
			{
				return mid;
			}
		}
	}
}

int QuickSortPart1(int* a, int left, int right)
{
	int Index = GetMidIndex(a, left, right);
	Swap(&a[left], &a[Index]);

	int begin = left, end = right;
	int pivot = begin;
	int key = a[pivot];

	while (begin < end)
	{
		//先从右开始找，找到比key值小的值
		while (begin < end && a[end] >= key)
		{
			end--;
		}
		//把找到的值往坑位放，然后找到的值形成新的坑位
		a[pivot] = a[end];
		pivot = end;

		//再先从左开始找，找到比key值大的值
		while (begin < end && a[begin] <= key)
		{
			begin++;
		}
		//把找到的值往坑位放，然后找到的值形成新的坑位
		a[pivot] = a[begin];
		pivot = begin;
	}

	pivot = begin;
	a[pivot] = key;

	return pivot;
}

int QuickSortPart2(int* a, int left, int right)
{
	int Index = GetMidIndex(a, left, right);
	Swap(&a[left], &a[Index]);

	int begin = left, end = right;
	int keyi = begin;

	while (begin < end)
	{
		//找小
		while (begin < end && a[end] >= a[keyi])
		{
			end--;
		}

		//找大
		while (begin < end && a[begin] <= a[keyi])
		{
			begin++;
		}

		Swap(&a[begin], &a[end]);
	}

	Swap(&a[begin], &a[keyi]);

	return begin;
}


int QuickSortPart3(int* a, int left, int right)
{
	int Index = GetMidIndex(a, left, right);
	Swap(&a[left], &a[Index]);

	int keyi = left;
	int prev = left, cur = left + 1;
	while (cur <= right)
	{
		if (a[cur] < a[keyi] && ++prev != cur)
		{
			Swap(&a[prev], &a[cur]);
		}
		cur++;
	}
	Swap(&a[prev], &a[keyi]);
	return prev;
}

void QuickSort(int* a, int left, int right)
{
	if (left >= right)
		return;

	int keyIndex = QuickSortPart1(a, left, right);
	/*int keyIndex = QuickSortPart2(a, left, right);*/
	/*int keyIndex = QuickSortPart3(a, left, right);*/

	//QuickSort(a, left, keyIndex - 1);
	//QuickSort(a, keyIndex + 1, right);
	
	//当递归到最后几层，元素个数少但个数多，就用插入排序
	if (keyIndex - 1 - left > 10)
	{
		QuickSort(a, left, keyIndex - 1);
	}
	else
	{
		InsertSort(a + left, keyIndex - left);
	}

	if (right - keyIndex - 1 > 10)
	{
		QuickSort(a, keyIndex + 1, right);
	}
	else
	{
		InsertSort(a + keyIndex + 1, right - keyIndex);
	}
}

void MergeSort(int* a, int n)
{
	int* tmp = new int[n];
	_MergeSort(a, 0, n - 1, tmp);//归并排序的子函数
	delete[]tmp;
}

void _MergeSort(int* a, int left, int right, int* tmp)
{
	if (left >= right)
		return;
	//首先分区间
	int mid = (right + left) >> 1;

	//再分
	_MergeSort(a, left, mid, tmp);
	_MergeSort(a, mid+1, right, tmp);

	//对分段区间的数进行排序
	int begin1 = left, end1 = mid;
	int begin2 = mid + 1, end2 = right;
	int index = left;
	while (begin1 <= end1 && begin2 <= end2)
	{
		if (a[begin1] < a[begin2])
		{
			tmp[index++] = a[begin1++];
		}
		else
		{
			tmp[index++] = a[begin2++];
		}
	}

	while (begin1 <= end1)
	{
		tmp[index++] = a[begin1++];
	}

	while (begin2 <= end2)
	{
		tmp[index++] = a[begin2++];
	}
	for (int i = left; i <= right; i++)
	{
		a[i] = tmp[i];
	}

}

void BubbleSort(int* a, int n)
{
	for (int i = 0; i < n; i++)
	{
		int flag = 0;
		for (int j = 0; j < n - i - 1; j++)
		{
			if (a[j] > a[j + 1])
			{
				Swap(&a[j], &a[j + 1]);
				flag = 1;
			}
		}
		if (flag == 0)
		{
			break;
		}
	}
}

void CountSort(int* a, int n)
{
	int min = a[0], max = a[0];
	for (int i = 0; i < n; i++)
	{
		if (a[i] > max)
		{
			max = a[i];
		}
		if (a[i] < min)
		{
			min = a[i];
		}
	}
	int gap = max - min;
	int* tmp = (int*)malloc(sizeof(int) * (min + gap));
	if (tmp != nullptr)
	{
		memset(tmp, 0, sizeof(int) * (gap + min));
		for (int i = 0; i < n; i++)
		{
			tmp[a[i] - min]++;
		}
		int j = 0;
		for (int i = 0; i < gap + min; i++)
		{
			while (tmp[i])
			{
				a[j++] = i + min;
				tmp[i]--;
			}
		}
	}
	free(tmp);
	tmp = NULL;
}
