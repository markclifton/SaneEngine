#include "se/gameinstance.h"

#include "se/systems/renderingsystem.h"

namespace se
{
	GameInstance::GameInstance(const std::string& Title, int Width, int Height)
		: wm_(Title, Width, Height), running_(true), systemUse_(se::Level::createLevel())
	{
		systemUse_->subscribe<events::LevelEvent>(this);
		systemUse_->subscribe<events::LevelEvent>(&wm_);
	}

	void GameInstance::run()
	{
		events::LevelEvent e{ se::Level::createLevel() };
		systemUse_->emit<events::LevelEvent>(e);

		se::Entity* entity = level_->create();
		entity->assign<vertex>();

		while (running_)
		{
			level_->tick(1.f);

			//Render to Screen

			wm_.swap();
		}
	}

	void GameInstance::receive(se::Level*, const events::Shutdown&)
	{ 
		running_ = false; 
	}

	void GameInstance::receive(se::Level*, const events::LevelEvent& e)
	{
		auto oldLevel = level_;
		level_ = e.level;

		if (oldLevel)
		{
			oldLevel->unsubscribe<events::Shutdown>(this);
		}
		level_->subscribe<events::Shutdown>(this);
	}
}
