#include <iostream>
#include <Windows.h>

#include "se/utils/array.h"
#include "se/utils/mmanager.h"

#include "se/ecs/testsystem.h"

int main()
{
	se::ecs::TestSystem testSystem;
	se::utils::MemoryManager manager;
	uint64_t id = manager.create(1000, sizeof(se::ecs::TestComponent));
	auto testCompArray = manager.get(id);

	for (int i = 0; i < 10; i++)
	{
		uint64_t i1;
		if (void* address = testCompArray->add(i1))
		{
			new(address) se::ecs::TestComponent();
		}
	}

	for (int i = 0; i < 100; i++)
	{
		testSystem.process( { testCompArray } );
	}

	std::cout << "Count: " << testCompArray->count() << std::endl;
	
	auto current = testCompArray->begin();
	while(current != testCompArray->end())
	{
		auto temp = (se::ecs::TestComponent*)(*current);
		std::cout << "TestComponent: " << temp << " " << temp->x << std::endl;
		current++;
	}

	system("pause");

	return 0;
}