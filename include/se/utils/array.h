#ifndef ARRAY_H
#define ARRAY_H

#include <cstring>
#include <stdint.h>

#include "se/utils/dlldefines.h"

namespace se
{
	namespace utils
	{
		class MYLIB_EXPORT Array
		{
		public:
			Array(uint64_t bufferSizeBytes, uint64_t elementSizeBytes);
			~Array();

			void* addElementRaw(uint64_t& id);
			void addElement(uint64_t& id, void* element);
			void removeElement(uint64_t id);

			bool atEndOfBuffer();
			char* getElement(uint64_t id);

			void printElements();

		private:
			char* front_;
			char* back_;
			uint64_t nextID_{ 1 };
			uint64_t elementSizeBytes_;
			uint64_t bufferSizeBytes_;
		};
	}
}

#endif