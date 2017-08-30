#include <GLFW/glfw3.h>

#include <iostream>

int main()
{
	std::cout << "hello" << std::endl;


	if (!glfwInit())
	{
		std::cout << "failed to initialize glfw" << std::endl;
		return -1;
	}
	try
	{
		// make window
		GLFWwindow* window = glfwCreateWindow(800, 600, "Hello", nullptr, nullptr);
		if (!window)
		{
			throw std::runtime_error("Failed to create window");
		}

		glfwMakeContextCurrent(window);

		while (!glfwWindowShouldClose(window))
		{
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
