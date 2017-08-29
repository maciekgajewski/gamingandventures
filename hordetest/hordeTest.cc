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


		// init Horde
		if(!h3dInit())
		{
			throw std::runtime_error("Failed to initialize Horde");
		}


		H3DRes pipeline = h3dAddResource( H3DResTypes::Pipeline, "forward.pipeline.xml", 0 );
		H3DRes sphere = h3dAddResource( H3DResTypes::SceneGraph, "sphere.scene.xml", 0 );



		if (!h3dutLoadResourcesFromDisk("resources"))
		{
			throw std::runtime_error("failed to load resources");
		}

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
		h3dutDumpMessages();
		glfwTerminate();
		throw;
	}

	glfwTerminate();
}
