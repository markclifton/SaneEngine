#include <iostream>

#include "se/gameinstance.h"
#include "se/managers/windowmanager.h"

#include "se/ecs/ecs.h"

#ifdef WIN32
#include <Windows.h>
#endif

/*
class TestGame : public se::GameInstance
{
public:
	TestGame()
	{
		setup();
	}

	~TestGame()
	{
		teardown();
	}

	void setup() 
	{
		//uint64_t id = memMgr_->create(1000, sizeof(se::ecs::TestComponent)); //This should be RAII
		//auto testCompArray = memMgr_->get(id);
	};
	
	void teardown() 
	{
	}

	//se::ecs::TestSystem testSystem;
};*/

int main()
{
#ifdef WIN32
	FreeConsole();
#endif

//	TestGame testGame;
	se::managers::WindowManager wm{ "Hello, World!", 640, 480 };
	wm.run();
	return 0;
}