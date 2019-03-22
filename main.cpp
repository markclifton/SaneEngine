#include <iostream>

#include "se/utils/array.h"
#include "se/utils/mmanager.h"

#include "se/ecs/testsystem.h"
#include "se/ecs/ievent.h"
#include "se/gameinstance.h"

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
};

#include "glad/glad.h"
#include <GLFW/glfw3.h>

int main()
{
	TestGame testGame;

	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();


	std::cin.get();
	return 0;
}