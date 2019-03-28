#ifndef GAMEINSTANCE_H
#define GAMEINSTANCE_H

#include <memory>

#include "se/utils/dlldefines.h"
#include "se/utils/noncopyable.h"
#include "se/managers/windowmanager.h"

#include "se/ecs/ecs.h"
#include "se/events/general.h"

namespace se
{
	class MYLIB_EXPORT GameInstance : private NonCopyable, public se::EventSubscriber<events::Shutdown>, public se::EventSubscriber<events::LevelEvent>
	{
	public:
		GameInstance(const std::string& Title, int Width, int Height);
		virtual ~GameInstance() = default;

		virtual void run();

	protected:
		virtual void receive(se::Level*, const events::Shutdown&);
		virtual void receive(se::Level*, const events::LevelEvent& e);

		managers::WindowManager wm_;
		bool running_{ false };
		se::Level* systemUse_{ nullptr };
		se::Level* level_{ nullptr };
	};
}

#endif