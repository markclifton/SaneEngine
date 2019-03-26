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

struct testComp
{
	int x = 0;
};

class TestSystem : public se::EntitySystem
{
public:
	TestSystem() = default; 
	~TestSystem() = default;

	void tick(se::World* world, float deltaTime) override
	{
		world->each<testComp>([&](se::Entity* ent, se::ComponentHandle<testComp> component) {
			component->x += 1;
		});
	}
};

int main()
{
#ifdef WIN32
	//FreeConsole();
#endif

	//Create World
	auto world = se::World::createWorld();

	//Create System
	auto sys = new TestSystem();
	world->registerSystem(sys);
	
	//Create Object
	auto obj = world->create();
	obj->assign<testComp>();

	//Update Systems
	for (int i = 0; i < 100; i++)
	{
		world->tick(1.f);
	}

	//Print Results
	auto comp = obj->get<testComp>();
	std::cout << comp.get().x << "\n";

//	TestGame testGame;
	se::managers::WindowManager wm{ "Hello, World!", 640, 480 };
	wm.run();
	return 0;
}