#include "se/gameinstance.h"

namespace se
{
	GameInstance::GameInstance()
		: memMgr_(new managers::MemoryManager())
	{
	}
}
