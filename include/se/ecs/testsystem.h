#ifndef TESTSYSTEM_H
#define TESTSYSTEM_H

#include "se/ecs/isystem.h"
#include "se/utils/array.h"
#include "se/utils/dlldefines.h"

namespace se
{
	namespace ecs
	{
		struct TestComponent
		{
			int x{ 0 };
		};

		class MYLIB_EXPORT TestSystem : public ISystem
		{
		public:
			void process(std::vector<utils::Array*> components) override;
		};
	}
}
#endif