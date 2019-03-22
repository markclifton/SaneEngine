#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include "gl.h"
#include <GLFW/glfw3.h>
#include <memory>
#include <string>

#include "se/utils/dlldefines.h"
#include "se/utils/noncopyable.h"

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
		class MYLIB_EXPORT WindowManager : private NonCopyable
		{
			std::unique_ptr<GLFWwindow, glfwWindowDeleter> window_;

			std::string title_;
			int w_, h_;
		public:
			WindowManager(const std::string& title, int width, int height);
			~WindowManager();

			void run(); //this is temp

			bool create();
			void destroy();

			void show();
			void hide();
		};
	}
}
#endif