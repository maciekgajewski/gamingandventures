#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <iostream>

void onWindowResize(GLFWwindow* /*window*/, int width, int height)
{
	glViewport(0, 0, width, height);
	std::cout << "window resized to " << width << "x" << height << std::endl;
}

GLFWwindow* initWindow(int width, int height)
{
	GLFWwindow* window = glfwCreateWindow(width, height, "Hello", nullptr, nullptr);
	if (!window)
	{
		throw std::runtime_error("Failed to create window");
	}

	glfwSetFramebufferSizeCallback(window, onWindowResize);
	glfwMakeContextCurrent(window);
	return window;
}


void mainLoop()
{

}


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
		GLFWwindow* window = initWindow(800, 600);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			throw std::runtime_error("Failed to initialize GLAD");
		}
		glViewport(0, 0, 800, 600);

		// main loop
		while (!glfwWindowShouldClose(window))
		{
			mainLoop();

			glfwSwapBuffers(window);
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
