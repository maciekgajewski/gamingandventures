#include <horde3d/Horde3D.h>
#include <horde3d/Horde3DUtils.h>

#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>



void setupWindow(int width, int height, bool fullscreen)
{
	// Create OpenGL window
	GLFWwindow* window = glfwCreateWindow(width, height, "Hello", nullptr, nullptr);
	if (!window)
		throw std::runtime_error("Failed to create window");

	glfwMakeContextCurrent(window);


}

int main(int, char**)
{
	std::cout << "hello" << std::endl;

	char* gv = (char *)glGetString( GL_VERSION );

	if (!glfwInit())
	{
		std::cout << "failed to initialize glfw" << std::endl;
		return -1;
	}


	// make window
	GLFWwindow* window = glfwCreateWindow(800, 600, "Hello", nullptr, nullptr);
	if (!window)
	{
		std::cout << "Failed to create window" << std::endl;
		return -1;
	}

	glfwMakeContextCurrent(window);

	// init Horde
	if(!h3dInit())
	{
		h3dutDumpMessages();
		std::cout << "Failed to initialize Horde" << std::endl;
		return -1;
	}


	try
	{

		/* Loop until the user closes the window */
		int i = 0;
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			glClear(GL_COLOR_BUFFER_BIT);

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
			std::cout << "boo" << i++ << std::endl;
		}

	}
	catch(...)
	{
		glfwTerminate();
		throw;
	}

	glfwTerminate();
}
