#include "se/gameinstance.h"

#include <iostream>
template<typename T>
class TestEventHandler : public se::EventSubscriber<T>
{
public:
	void receive(se::Level* Level, const T& e) {
		if(e.action == 0) std::cout << "Received event! " << char('a' + e.key - 'a') << "\n"; ;
	}
};

namespace se
{
	GameInstance::GameInstance(const std::string& Title, int Width, int Height)
		: wm(Title, Width, Height), running_(true), systemUse_(se::Level::createLevel())
	{
		systemUse_->subscribe<events::LevelEvent>(this);
		systemUse_->subscribe<events::LevelEvent>(&wm);
	}

	void GameInstance::run()
	{
		events::LevelEvent e{ se::Level::createLevel() };
		systemUse_->emit<events::LevelEvent>(e);

		TestEventHandler<events::KeyEvent> testHandler;
		level_->subscribe<events::KeyEvent>(&testHandler);
		
		while (running_)
		{
			//Tick Systems

			//Render to Screen

			//Input

			wm.run(); //TODO: WM shouldn't have a run loop
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
