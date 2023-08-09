#pragma once
#include<bits/stdc++.h>
typedef char BTNodeData;

typedef struct BinaryTreeNode
{
	struct BinaryTreeNode* leftchild;
	struct BinaryTreeNode* rightchild;
	BTNodeData data;
}BTNode;

//前序遍历
void PrevOrder(BTNode* root);

//中序遍历
void InOrder(BTNode* root);

//后序遍历
void PostOrder(BTNode* root);

//层序遍历
void LevelOrder(BTNode* root);

//以root为根的结点的个数
int TreeSize(BTNode* root);

//计算以root为根的叶子结点的个数
int TreeLeafSize(BTNode* root);