#include <iostream>
#include <Windows.h>

#include "se/utils/array.h"

struct TestStruct
{
	uint64_t id{ 1 };
	char x{ 'x' }, y{ 'y' }, z{ 'z' };
};

int main()
{
	se::utils::Array a(100, sizeof(TestStruct));

	uint64_t i1, i2, i3;

	TestStruct t1;
	a.addElement(i1, &t1);
	
	TestStruct t2;
	a.addElement(i2, &t2);
	
	TestStruct t3;
	a.addElement(i3, &t3);
	
	a.removeElement(i1);
	a.printElements();

	system("pause");

	return 0;
}