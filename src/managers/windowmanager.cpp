#include "se/managers/windowmanager.h"

namespace se
{
	namespace managers
	{
		WindowManager::WindowManager(const std::string& title, int width, int height)
			: title_(title), w_(width), h_(height)
		{
			create();
			show();
		}
		
		WindowManager::~WindowManager()
		{
			hide();
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

			glfwMakeContextCurrent(window_.get());
		
			if (!gladLoadGL(glfwGetProcAddress))
			{
				return false;
			}

			return true;
		}

		void WindowManager::run()
		{
			char const * VertexShaderSource = R"GLSL(

		#version 130

		in vec2 position;

		void main()

		{

			gl_Position = vec4(position, 0.0, 1.0);

		}

	)GLSL";



			char const * FragmentShaderSource = R"GLSL(

		#version 130

		out vec4 outColor;

		void main()

		{

			outColor = vec4(1.0, 1.0, 1.0, 1.0);

		}

	)GLSL";

			GLfloat const Vertices[] = {
				0.0f, 0.5f,
				0.5f, -0.5f,
				-0.5f, -0.5f
			};

			GLuint const Elements[] = {
				0, 1, 2
			};

			GLuint VAO;
			(glGenVertexArrays(1, &VAO));
			(glBindVertexArray(VAO));

			GLuint VBO;
			(glGenBuffers(1, &VBO));
			(glBindBuffer(GL_ARRAY_BUFFER, VBO));
			(glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW));
			(glBindBuffer(GL_ARRAY_BUFFER, 0));

			GLuint EBO;
			(glGenBuffers(1, &EBO));
			(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
			(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Elements), Elements, GL_STATIC_DRAW));

			GLint Compiled;
			GLuint VertexShader = glCreateShader(GL_VERTEX_SHADER);
			(glShaderSource(VertexShader, 1, &VertexShaderSource, NULL));
			(glCompileShader(VertexShader));
			(glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &Compiled));
			if (!Compiled)
			{

			}

			GLuint FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
			(glShaderSource(FragmentShader, 1, &FragmentShaderSource, NULL));
			(glCompileShader(FragmentShader));
			(glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &Compiled));

			if (!Compiled)
			{

			}

			GLuint ShaderProgram = glCreateProgram();

			(glAttachShader(ShaderProgram, VertexShader));
			(glAttachShader(ShaderProgram, FragmentShader));
			(glBindFragDataLocation(ShaderProgram, 0, "outColor"));
			(glLinkProgram(ShaderProgram));
			(glUseProgram(ShaderProgram));

			GLint PositionAttribute = (glGetAttribLocation(ShaderProgram, "position"));

			(glEnableVertexAttribArray(PositionAttribute));

			(glBindBuffer(GL_ARRAY_BUFFER, VBO));

			(glVertexAttribPointer(PositionAttribute, 2, GL_FLOAT, GL_FALSE, 0, 0));

			(glBindBuffer(GL_ARRAY_BUFFER, 0));

			while (!glfwWindowShouldClose(window_.get()))
			{
				(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
				(glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0));
				glfwSwapBuffers(window_.get());

				glfwPollEvents();
			}

			(glDeleteProgram(ShaderProgram));
			(glDeleteShader(FragmentShader));
			(glDeleteShader(VertexShader));

			(glDeleteBuffers(1, &EBO));
			(glDeleteBuffers(1, &VBO));
			(glDeleteVertexArrays(1, &VAO));
		}

		void WindowManager::destroy()
		{
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
	}
}