#ifndef EVENTS_H
#define EVENTS_H

#include "se/ecs/ecs.h"

namespace se
{
	class Level;
	class Entity;

	template<typename T>
	class ComponentHandle;
}

namespace se
{
	namespace events
	{
		struct Shutdown
		{
		};

		struct LevelEvent
		{
			se::Level* level;
		};

		struct OnEntityCreated
		{
			Entity* entity;
		};

		struct OnEntityDestroyed
		{
			Entity* entity;
		};

		template<typename T>
		struct OnComponentAssigned
		{
			Entity* entity;
			ComponentHandle<T> component;
		};

		template<typename T>
		struct OnComponentRemoved
		{
			Entity* entity;
			ComponentHandle<T> component;
		};
	}
}

#endif