#include "se/utils/array.h"

namespace se
{
	namespace utils
	{
		Array::Array()
		{
		}

		Array::Array(uint64_t bufferSizeBytes, uint64_t elementSizeBytes)
			: front_(new char[bufferSizeBytes])
			, back_(front_)
			, elementSizeBytes_(elementSizeBytes)
			, bufferSizeBytes_(bufferSizeBytes)
		{
		}

		Array::~Array()
		{
			delete[] front_;
		}

		void* Array::addElementRaw(uint64_t& id)
		{
			id = nextID_++;

			if (atEndOfBuffer())
			{
				return nullptr;
			}

			memcpy(back_, &id, sizeof(uint64_t));
			back_ += sizeof(uint64_t);

			void* address = (void*)back_;
			back_ += elementSizeBytes_;

			return address;
		}

		void Array::addElement(void* element, uint64_t& id)
		{
			void* address = addElementRaw(id);
			if (address)
			{
				char* newElement = new(address)char[elementSizeBytes_];
				memcpy(newElement, element, elementSizeBytes_);
			}
		}

		void Array::removeElement(uint64_t id)
		{
			char* currentElement = front_;
			while (currentElement < back_)
			{
				if ((uint64_t)*currentElement == id)
				{
					back_ -= (elementSizeBytes_ + sizeof(uint64_t));
					memcpy(currentElement, back_, elementSizeBytes_ + sizeof(uint64_t));
					return;
				}

				currentElement += elementSizeBytes_ + sizeof(uint64_t);
			}
		}

		void Array::clear()
		{
			back_ = front_;
		}

		bool Array::atEndOfBuffer()
		{
			char* nextElement = back_;
			nextElement += elementSizeBytes_ + sizeof(uint64_t);
			return nextElement > front_ + bufferSizeBytes_;
		}

		char* Array::getElement(uint64_t id)
		{
			char* currentElement = front_;
			while (currentElement < back_)
			{
				if ((uint64_t)*currentElement == id)
				{
					currentElement += sizeof(uint64_t);
					return currentElement;
				}

				currentElement += elementSizeBytes_ + sizeof(uint64_t);
			}
			return nullptr;
		}

		uint64_t Array::count()
		{
			return (back_ - front_) / (elementSizeBytes_ + sizeof(uint64_t));
		}

		void* Array::getAtIndex(uint64_t index)
		{
			return front_ + (sizeof(uint64_t) + elementSizeBytes_) * index + sizeof(uint64_t);
		}
	}
}