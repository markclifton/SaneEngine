#include <iostream>
#include <Windows.h>

#include "se/utils/array.h"
#include "se/utils/mmanager.h"

#include "se/ecs/testsystem.h"

int main()
{
	se::ecs::TestSystem tsys;

	se::utils::MemoryManager manager;
	uint64_t id = manager.create(100, sizeof(se::ecs::TestComponent));
	auto a = manager.get(id);
	
	se::ecs::TestComponent t1, t2, t3;
	uint64_t i1, i2, i3;
	a->addElement(&t1, i1);
	a->addElement(&t2, i2);
	a->addElement(&t3, i3);

	for (int i = 0; i < 10'000'000; i++)
	{
		tsys.process(std::vector<se::utils::Array*>{a});
	}

	std::cout << "Count: " << a->count() << std::endl;
	for (int i = 0; i < a->count(); i++)
	{
		auto temp = (se::ecs::TestComponent*)(*a)[i];
		std::cout << "TestComponent: " << temp << " " << temp->x << std::endl;
	}

	system("pause");

	return 0;
}