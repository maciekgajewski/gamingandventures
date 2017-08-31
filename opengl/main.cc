#include "window.hh"
#include "io.hh"
#include "shader.hh"

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

class Scene
{
public:

	void init()
	{
		// init VBO (vertex data)
		vertices = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.0f,  0.5f, 0.0f
		};

		// shaders
		shader_ = OT::Shader(OT::readFile("shader.vert"), OT::readFile("shader.frag"));

		// vertex array object
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		// vertex buffer object
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

		// shader params
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // uses currently bound VAO
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void mainLoop()
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader_.Use();
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

private:

	std::vector<float> vertices;
	unsigned vbo = 0; // vertex buffer object
	unsigned vao = 0; // vertex array object

	OT::Shader shader_;
};


int main()
{
	std::cout << "hello" << std::endl;


	if (!glfwInit())
	{
		std::cout << "failed to initialize glfw" << std::endl;
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	try
	{
		// make window
		OT::Window window(800, 600, "Hello");

		window.makeContextCurrent();

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			throw std::runtime_error("Failed to initialize GLAD");
		}
		glViewport(0, 0, 800, 600);

		Scene scene;

		scene.init();

		// main loop
		while (!window.shouldClose())
		{
			scene.mainLoop();

			window.swapBuffers();
			glfwPollEvents();
		}

	}
	catch(...)
	{
		glfwTerminate();
		throw;
	}

	glfwTerminate();
}
