#ifndef ISYSTEM_H
#define ISYSTEM_H

#include <vector>

#include "se/utils/array.h"
#include "se/utils/dlldefines.h"
#include "se/utils/noncopyable.h"

namespace se
{
	namespace ecs
	{
		class MYLIB_EXPORT ISystem
		{
		public:
			virtual void process(std::vector<se::utils::Array*> components) = 0;
		};
	}
}
#endif