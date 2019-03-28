#include <iostream>

#include "se/gameinstance.h"

#include "se/ecs/ecs.h"
#include "se/events/input.h"

#ifdef WIN32
#include <Windows.h>
#endif

//class TestSystem : public se::EntitySystem
//{
//public:
//	TestSystem() = default; 
//	~TestSystem() = default;
//
//	void tick(se::Level* level, float deltaTime) override
//	{
//		level->each<testComp>([&](se::Entity* ent, se::ComponentHandle<testComp> component) {
//			component->x += 1;
//		});
//	}
//};

int main()
{
#ifdef WIN32
	//FreeConsole();
#endif

	se::GameInstance instance("Game Instance", 640, 480);
	instance.run();
	return 0;
}