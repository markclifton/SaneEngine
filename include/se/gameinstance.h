#ifndef GAMEINSTANCE_H
#define GAMEINSTANCE_H

#include <memory>

#include "se/utils/dlldefines.h"
#include "se/utils/noncopyable.h"

namespace se
{
	class MYLIB_EXPORT GameInstance : private NonCopyable
	{
	public:
		GameInstance();
		virtual ~GameInstance() = default;

	private:
	};
}

#endif