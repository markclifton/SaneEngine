#include "se/utils/array.h"
#include <stdio.h>
namespace se
{
	namespace utils
	{
		Array::ArrayIterator::ArrayIterator(char* pointer, Array* arr) 
			: current_(pointer), a_(arr) 
		{}

		Array::ArrayIterator::ArrayIterator(const Array::ArrayIterator& mit)
		{
			if (this != &mit)
			{
				current_ = mit.current_;
				a_ = mit.a_;
			}
		}

		Array::ArrayIterator& Array::ArrayIterator::operator++()
		{
			if (current_ != *a_->end())
			{
				current_ += a_->stepSize();
			}
			return *this;
		}

		Array::ArrayIterator Array::ArrayIterator::operator++(int) 
		{ 
			Array::ArrayIterator tmp(*this); 
			operator++(); 
			return tmp; 
		}

		bool Array::ArrayIterator::operator==(const Array::ArrayIterator& rhs) const 
		{ 
			return current_ == rhs.current_; 
		}

		bool Array::ArrayIterator::operator!=(const Array::ArrayIterator& rhs) const 
		{ 
			return current_ != rhs.current_; 
		}

		char* Array::ArrayIterator::operator*() 
		{ 
			return current_; 
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

		void* Array::add(uint64_t& id)
		{
			id = nextID_++;

			if (full())
			{
				printf("End of buffer\n");
				return nullptr;
			}

			memcpy(back_, &id, sizeof(uint64_t));
			back_ += sizeof(uint64_t);

			void* address = (void*)back_;
			back_ += elementSizeBytes_;

			return address;
		}

		void Array::add(void* element, uint64_t& id)
		{
			void* address = add(id);
			if (address)
			{
				char* newElement = new(address)char[elementSizeBytes_];
				memcpy(newElement, element, elementSizeBytes_);
			}
		}

		void Array::remove(uint64_t id)
		{
			char* currentElement = front_;
			while (currentElement < back_)
			{
				if ((uint64_t)*currentElement == id)
				{
					back_ -= stepSize();
					memcpy(currentElement, back_, stepSize());
					return;
				}

				currentElement += stepSize();
			}
		}

		void* Array::get(uint64_t id)
		{
			char* currentElement = front_;
			while (currentElement < back_)
			{
				if ((uint64_t)*currentElement == id)
				{
					currentElement += sizeof(uint64_t);
					return currentElement;
				}

				currentElement += stepSize();
			}
			return nullptr;
		}

		void Array::clear()
		{
			back_ = front_;
		}

		bool Array::full()
		{
			return back_ >= front_ + bufferSizeBytes_;
		}

		uint64_t Array::count()
		{
			return (back_ - front_) / stepSize();
		}

		void* Array::operator[](uint64_t index)
		{
			return front_ + stepSize() * index + sizeof(uint64_t);
		}

		Array::ArrayIterator Array::begin()
		{
			return { front_ + sizeof(uint64_t), this };
		}

		Array::ArrayIterator Array::end()
		{
			return { back_ + sizeof(uint64_t), this };
		}

		uint64_t Array::stepSize()
		{
			return sizeof(uint64_t) + elementSizeBytes_;
		}
	}
}