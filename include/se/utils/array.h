#ifndef ARRAY_H
#define ARRAY_H

#include <cstring>
#include <stdint.h>

#include "se/utils/dlldefines.h"

#include "se/utils/noncopyable.h"

namespace se
{
	namespace utils
	{
		class MYLIB_EXPORT Array : private NonCopyable
		{
		public:
			Array(uint64_t bufferSizeBytes, uint64_t elementSizeBytes);
			~Array();

			void* addElementRaw(uint64_t& id);
			void addElement(void* element, uint64_t& id);
			void removeElement(uint64_t id);
			void* getElement(uint64_t id);

			void clear();
			bool full();
			uint64_t count();

			void* operator[](uint64_t index);

		private:
			char* front_;
			char* back_;
			uint64_t nextID_{ 0 };
			uint64_t elementSizeBytes_;
			uint64_t bufferSizeBytes_;

			uint64_t stepSize();
		};
	}
}
#endif