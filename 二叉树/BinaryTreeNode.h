#pragma once
#include<bits/stdc++.h>
typedef char BTNodeData;

typedef struct BinaryTreeNode
{
	struct BinaryTreeNode* leftchild;
	struct BinaryTreeNode* rightchild;
	BTNodeData data;
}BTNode;

//ǰ�����
void PrevOrder(BTNode* root);

//�������
void InOrder(BTNode* root);

//�������
void PostOrder(BTNode* root);

//�������
void LevelOrder(BTNode* root);

//��rootΪ���Ľ��ĸ���
int TreeSize(BTNode* root);

//������rootΪ����Ҷ�ӽ��ĸ���
int TreeLeafSize(BTNode* root);