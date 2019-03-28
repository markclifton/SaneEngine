#include "se/managers/windowmanager.h"

namespace
{
	void key_forwarder(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		auto windowManager = static_cast<se::managers::WindowManager*>(glfwGetWindowUserPointer(window));
		windowManager->keyHandler(key, scancode, action, mods);
	}

	void cursor_forwarder(GLFWwindow* window, double xpos, double ypos)
	{
		auto windowManager = static_cast<se::managers::WindowManager*>(glfwGetWindowUserPointer(window));
		windowManager->mouseHandler(xpos, ypos);
	}

	void mouse_forwarder(GLFWwindow* window, int button, int action, int mods)
	{
		auto windowManager = static_cast<se::managers::WindowManager*>(glfwGetWindowUserPointer(window));
		windowManager->mouseHandler(button, action, mods);
	}

	void close_forwarder(GLFWwindow* window)
	{
		auto windowManager = static_cast<se::managers::WindowManager*>(glfwGetWindowUserPointer(window));
		windowManager->closeHandler();
	}
}

namespace se
{
	namespace managers
	{
		WindowManager::WindowManager(const std::string& title, int width, int height)
			: title_(title), w_(width), h_(height)
		{
			create();
		}
		
		WindowManager::~WindowManager()
		{
			destroy();
		}

		bool WindowManager::create()
		{
			if (!glfwInit())
				return false;

			window_.reset(glfwCreateWindow(w_, h_, title_.c_str(), NULL, NULL));
			if (!window_.get())
			{
				glfwTerminate();
				return false;
			}

			//GLFW Callbacks
			glfwSetWindowUserPointer(window_.get(), this);
			glfwSetKeyCallback(window_.get(), key_forwarder);
			glfwSetCursorPosCallback(window_.get(), cursor_forwarder);
			glfwSetMouseButtonCallback(window_.get(), mouse_forwarder);
			glfwSetWindowCloseCallback(window_.get(), close_forwarder);

			glfwMakeContextCurrent(window_.get());
		
			if (!gladLoadGL(glfwGetProcAddress))
			{
				return false;
			}

			glGenVertexArrays(1, &vao_);
			glBindVertexArray(vao_);

			return true;
		}

		void WindowManager::swap()
		{
			glfwSwapBuffers(window_.get());
			glfwPollEvents();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		void WindowManager::destroy()
		{
			glDeleteVertexArrays(1, &vao_);
			window_ = nullptr;
		}

		void WindowManager::show()
		{
			glfwShowWindow(window_.get());
		}
		
		void WindowManager::hide()
		{
			glfwHideWindow(window_.get());
		}

		void WindowManager::keyHandler(int key, int scancode, int action, int mods)
		{
			if (level_ == nullptr) return;
			events::KeyEvent e{ key, scancode, action, mods };
			level_->emit<events::KeyEvent>(e);
		}

		void WindowManager::mouseHandler(double xpos, double ypos)
		{
			if (level_ == nullptr) return;
			events::MouseMoveEvent e{ xpos, ypos };
			level_->emit<events::MouseMoveEvent>(e);
		}

		void WindowManager::mouseHandler(int button, int action, int mods)
		{
			if (level_ == nullptr) return;
			events::MousePressEvent e{ button, action, mods };
			level_->emit<events::MousePressEvent>(e);
		}

		void WindowManager::closeHandler()
		{
			glfwSetWindowShouldClose(window_.get(), GLFW_TRUE);
			if (level_ == nullptr) return;
			events::Shutdown e;
			level_->emit<events::Shutdown>(e);

		}
	}
}