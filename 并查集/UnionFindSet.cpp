#include"UnionFindSet.h"


void TestUFS()
{
	UnionFindSet u(10);
	u.Union(0, 6);
	u.Union(7, 6);
	u.Union(7, 8);

	u.Union(1, 4);
	u.Union(4, 9);

	u.Union(2, 3);
	u.Union(2, 5);

	std::cout << u.SetCount() << std::endl;
}


int main()
{
	TestUFS();
	return 0;
}