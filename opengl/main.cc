#include "window.hh"
#include "io.hh"

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

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

		// shaders
		vertexShader = loadShader("shader.vert", GL_VERTEX_SHADER);
		fragmentShader = loadShader("shader.frag", GL_FRAGMENT_SHADER);
	}

	void mainLoop()
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

private:

	unsigned loadShader(const std::string& sourcePath, unsigned shaderType)
	{
		std::string code = OT::readFile(sourcePath);

		unsigned shaderId = glCreateShader(shaderType);
		const char* codePtr = code.c_str();
		glShaderSource(shaderId, 1, &codePtr, nullptr);
		glCompileShader(shaderId);

		int  success;
		char infoLog[512];
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
		if(!success)
		{
			glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
			std::cout << "Error compiling shader " << std::endl;
			std::cout << sourcePath << " " << infoLog << std::endl;
			throw std::runtime_error("Error compiling shader");
		}

		return shaderId;
	}

	std::vector<float> vertices;
	unsigned vbo = 0;
	unsigned vertexShader = 0;
	unsigned fragmentShader = 0;

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
