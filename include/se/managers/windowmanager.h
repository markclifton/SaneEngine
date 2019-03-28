#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include "gl.h"
#include <GLFW/glfw3.h>
#include <memory>
#include <string>

#include "se/utils/dlldefines.h"
#include "se/utils/noncopyable.h"
#include "se/ecs/ecs.h"

#include "se/events/general.h"
#include "se/events/input.h"

namespace
{
	struct glfwWindowDeleter
	{
		void operator()(GLFWwindow* ptr)
		{
			glfwTerminate();
			glfwDestroyWindow(ptr);
		}
	};
}

namespace se
{
	namespace managers
	{
		class MYLIB_EXPORT WindowManager : private NonCopyable, public se::EventSubscriber<events::LevelEvent>
		{
			std::unique_ptr<GLFWwindow, glfwWindowDeleter> window_;

			std::string title_;
			int w_, h_;

			se::Level* level_{ nullptr };
			
			GLuint vao_{ 0 };
		public:
			WindowManager(const std::string& title, int width, int height);
			~WindowManager();

			void swap();

			bool create();
			void destroy();

			void show();
			void hide();

			void keyHandler(int key, int scancode, int action, int mods);
			void mouseHandler(double xpos, double ypos);
			void mouseHandler(int button, int action, int mods);
			void closeHandler();

			void receive(se::Level* Level, const events::LevelEvent& e) { level_ = e.level; }
		};
	}
}
#endif