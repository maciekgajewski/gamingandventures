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

		// shaders
		vertexShader = loadShader("shader.vert", GL_VERTEX_SHADER);
		fragmentShader = loadShader("shader.frag", GL_FRAGMENT_SHADER);
		shaderProgram = linkShaders(vertexShader, fragmentShader);


		glDeleteShader(vertexShader); vertexShader = 0;
		glDeleteShader(fragmentShader); fragmentShader = 0;


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

		glUseProgram(shaderProgram);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);
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

	unsigned linkShaders(unsigned vertexShader, unsigned fragmentShader)
	{
		unsigned sp = glCreateProgram();
		glAttachShader(sp, vertexShader);
		glAttachShader(sp, fragmentShader);
		glLinkProgram(sp);

		int  success;
		char infoLog[512];
		glGetProgramiv(sp, GL_LINK_STATUS, &success);
		if(!success) {
			glGetProgramInfoLog(sp, 512, NULL, infoLog);
			std::cout << "Error linking shaders" << std::endl;
			std::cout << infoLog << std::endl;
			throw std::runtime_error("Error linking shaders");
		}
		return sp;
	}

	std::vector<float> vertices;
	unsigned vbo = 0; // vertex buffer object
	unsigned vao = 0; // vertex array object

	unsigned vertexShader = 0;
	unsigned fragmentShader = 0;
	unsigned shaderProgram = 0;

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
