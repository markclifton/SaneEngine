#ifndef ARRAY_H
#define ARRAY_H

#include <cstring>
#include <iterator>
#include <stdint.h>

#include "se/utils/dlldefines.h"
#include "se/utils/noncopyable.h"

namespace se
{
	namespace utils
	{
		class MYLIB_EXPORT Array : private NonCopyable
		{
			friend class ArrayIterator;
		public:
			class MYLIB_EXPORT ArrayIterator : public std::iterator<std::input_iterator_tag, char*>
			{
			public:
				ArrayIterator(char* pointer, Array* arr);
				ArrayIterator(const ArrayIterator& mit);
				ArrayIterator& operator++();
				ArrayIterator operator++(int);
				bool operator==(const ArrayIterator& rhs) const;
				bool operator!=(const ArrayIterator& rhs) const;
				char* operator*();
			private:
				char* current_;
				Array* a_;
			};

			Array(uint64_t bufferSizeBytes, uint64_t elementSizeBytes);
			~Array();

			void* add(uint64_t& id);
			void add(void* element, uint64_t& id);
			void remove(uint64_t id);
			void* get(uint64_t id);

			void clear();
			bool full();
			uint64_t count();

			void* operator[](uint64_t index);

			ArrayIterator begin();
			ArrayIterator end();

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