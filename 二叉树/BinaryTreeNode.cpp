#include"BinaryTreeNode.h"

void PrevOrder(BTNode* root)
{
	if (root == nullptr)
	{
		return;
	}

	std::cout << root->data << " ";
	PrevOrder(root->leftchild);
	PrevOrder(root->rightchild);
}

void InOrder(BTNode* root)
{
	if (root == nullptr)
	{
		return;
	}

	InOrder(root->leftchild);
	std::cout << root->data << " ";
	InOrder(root->rightchild);

}

void PostOrder(BTNode* root)
{
	if (root == nullptr)
	{
		return;
	}

	PostOrder(root->leftchild);
	PostOrder(root->rightchild);
	std::cout << root->data << " ";
}

void LevelOrder(BTNode* root)
{
	if (root == nullptr)
	{
		return;
	}
	std::queue<BTNode*> q;
	q.push(root);
	
	while (!q.empty())
	{
		BTNode* front = q.front();
		q.pop();

		std::cout << front->data << " ";
		if (front->leftchild)
		{
			q.push(front->leftchild);
		}
		if (front->rightchild)
		{
			q.push(front->rightchild);
		}
	}

}

int TreeSize(BTNode* root)
{
	return root == nullptr ? 0 : 
		TreeSize(root->leftchild) + TreeSize(root->rightchild) + 1;
}

int TreeLeafSize(BTNode* root)
{
	if (root == nullptr)
	{
		return 0;
	}
	if (root->leftchild == nullptr && root->rightchild == nullptr)
	{
		return 1;
	}
	return TreeLeafSize(root->leftchild) + TreeLeafSize(root->rightchild);
}


