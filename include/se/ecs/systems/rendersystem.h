#ifndef RENDERSYS_H
#define RENDERSYS_H

#include "glm/vec3.h"

#include "se/ecs/isystem.h"


// Batch Render
	// Vertices
	// Indices

// Mesh Render
// Instanced Render

typedef int Indice;

namespace se
{
	namespace ecs
	{
		struct vertex
		{
			glm::vec3 pos{ 0, 0, 0 };
			glm::vec3 color{ 1, 1, 1 };
		};
	
		class MYLIB_EXPORT RenderSystem : public ISystem
		{
		public:
			void process(std::vector<se::utils::Array*> components) {}
		};
	}
}
#endif