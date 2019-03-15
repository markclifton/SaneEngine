#include "se/ecs/testsystem.h"

namespace se
{
	namespace ecs
	{
		void TestSystem::process(std::vector<se::utils::Array*> components)
		{
			if (!components.empty())
			{
				for (auto& a : components)
				{
					for (int i = 0; i < a->count(); i++)
					{
						auto comp = (TestComponent*)(*a)[i];
						comp->x++;
					}
				}
			}
		}
	}
}