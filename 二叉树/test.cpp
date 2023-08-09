#include"BinaryTreeNode.h"

int main()
{
	BTNode* A = new BTNode();
	A->data = 'A';
	A->leftchild = nullptr;
	A->rightchild = nullptr;

	BTNode* B = new BTNode();
	B->data = 'B';
	B->leftchild = nullptr;
	B->rightchild = nullptr;

	BTNode* C = new BTNode();
	C->data = 'C';
	C->leftchild = nullptr;
	C->rightchild = nullptr;

	BTNode* D = new BTNode();
	D->data = 'D';
	D->leftchild = nullptr;
	D->rightchild = nullptr;

	BTNode* E = new BTNode();
	E->data = 'E';
	E->leftchild = nullptr;
	E->rightchild = nullptr;

	A->leftchild = B;
	A->rightchild = C;
	B->leftchild = D;
	B->rightchild = E;

	PrevOrder(A);
	std::cout << std::endl;

	InOrder(A);
	std::cout << std::endl;

	PostOrder(A);
	std::cout << std::endl;

	LevelOrder(A);
	std::cout << std::endl;

	std::cout << "TreeSize:" << TreeSize(A) << std::endl;
	std::cout << "TreeSize:" << TreeSize(B) << std::endl;

	std::cout << "TreeLeafSize:" << TreeLeafSize(A) << std::endl;
	std::cout << "TreeLeafSize:" << TreeLeafSize(B) << std::endl;
	return 0;
}