#ifndef MMANAGER_H
#define MMANAGER_H

#include <map>
#include <stdint.h>

#include "se/utils/array.h"
#include "se/utils/dlldefines.h"
#include "se/utils/noncopyable.h"

namespace se
{
	namespace managers
	{
		class MYLIB_EXPORT MemoryManager : private NonCopyable
		{
		public:
			MemoryManager();
			~MemoryManager();
		
			utils::Array* get(uint64_t id);
			uint64_t create(uint64_t bufferSizeBytes, uint64_t elementSizeBytes);
			void remove(uint64_t id);


		private:
			uint64_t nextID_{ 0 };
			std::map<uint64_t, utils::Array*> arrays_;
		};
	}
}

#endif