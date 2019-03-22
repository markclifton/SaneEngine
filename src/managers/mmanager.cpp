#include "se/managers/mmanager.h"

namespace se
{
	namespace managers
	{
		MemoryManager::MemoryManager()
		{
		}

		MemoryManager::~MemoryManager()
		{
			for (auto& a : arrays_)
			{
				delete a.second;
			}
			arrays_.clear();
		}

		utils::Array* MemoryManager::get(uint64_t id)
		{
			if (arrays_.count(id) != 0)
			{
				return arrays_[id];
			}
			return nullptr;
		}
		
		uint64_t MemoryManager::create(uint64_t bufferSizeBytes, uint64_t elementSizeBytes) 
		{
			uint64_t id = nextID_++;
			arrays_[id] = new utils::Array(bufferSizeBytes, elementSizeBytes);
			return id;
		}
		
		void MemoryManager::remove(uint64_t id) 
		{
			if (arrays_.count(id) != 0)
			{
				delete arrays_[id];
				arrays_.erase(id);
			}
		}
	}
}
