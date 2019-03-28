#ifndef RENDERINGSYS_H
#define RENDERINGSYS_H

#include "se/ecs/ecs.h"

#include <glm/glm.hpp>

struct vertex
{
	glm::vec3 pos;
	glm::vec3 color;
};

//Entity has components

class RenderingSystem : public se::EntitySystem
{
public:
	void tick(se::Level* level, float deltaTime) override
	{
		level->each<vertex>([&](se::Entity* ent, se::ComponentHandle<vertex> component) {
			//std::vector<ComponentHandle<vertex>> vertices = ent->getAll<vertex>();
		});
	}
};

#endif