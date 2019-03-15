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
			Array();
			Array(uint64_t bufferSizeBytes, uint64_t elementSizeBytes);
			~Array();

			void* addElementRaw(uint64_t& id);
			void addElement(void* element, uint64_t& id);
			void removeElement(uint64_t id);

			void clear();

			bool atEndOfBuffer();
			char* getElement(uint64_t id);

			uint64_t count();
			void* getAtIndex(uint64_t index);

			char* operator[](int i)
			{
				return front_ + (sizeof(uint64_t) + elementSizeBytes_) * i + sizeof(uint64_t);
			}

		private:
			char* front_;
			char* back_;
			uint64_t nextID_{ 0 };
			uint64_t elementSizeBytes_;
			uint64_t bufferSizeBytes_;
		};
	}
}
#endif