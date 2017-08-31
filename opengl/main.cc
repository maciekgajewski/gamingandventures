#include "window.hh"
#include "io.hh"
#include "shader.hh"

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <iostream>

class Scene
{
public:

	void init()
	{
		// init VBO (vertex data)
		float verticesTriangle[] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.0f,  0.5f, 0.0f
		};
		float verticesRect[] = {
			 0.5f,  0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			-0.5f, -0.5f, 0.0f,
			 -0.5f,  0.5f, 0.0f
		};
		unsigned indicesRect[] = {
			0, 1, 3,
			1, 2, 3
		};

		// shaders
		shader_ = OT::Shader(OT::readFile("shaders/solid.vert"), OT::readFile("shaders/solid.frag"));

		// triangle ========
		// vertex array object
		glGenVertexArrays(1, &triangleVao_);
		glBindVertexArray(triangleVao_);

		// vertex buffer object
		glGenBuffers(1, &triangleVbo_);
		glBindBuffer(GL_ARRAY_BUFFER, triangleVbo_);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verticesTriangle), verticesTriangle, GL_STATIC_DRAW);

		// shader params (move this to draw?)
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // uses currently bound VAO
		glEnableVertexAttribArray(0);

		// cleanup
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		// rectangle ======
		glGenVertexArrays(1, &rectVao_);
		glBindVertexArray(rectVao_);

		glGenBuffers(1, &rectEao_);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rectEao_);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesRect), indicesRect, GL_STATIC_DRAW);

		glGenBuffers(1, &rectVbo_);
		glBindBuffer(GL_ARRAY_BUFFER, rectVbo_);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verticesRect), verticesRect, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // uses currently bound VAO
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void mainLoop()
	{
		// clear
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// activate shader (material)
		OT::Uniform color = shader_.GetUniform("color");
		shader_.Use();

		if (debug)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		// draw rectangle
		if (drawRect)
		{
			color.Set(glm::vec3(1.0f, 0.0f, 0.0f));

			glBindVertexArray(rectVao_);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rectEao_); // why is this needed?
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
		// draw triangle
		else
		{
			color.Set(glm::vec3(1.0f, 0.0f, 1.0f));

			glBindVertexArray(triangleVao_);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}

	}

	bool drawRect = false;
	bool debug = false;

private:

	unsigned triangleVbo_ = 0; // vertex buffer object
	unsigned triangleVao_ = 0; // vertex array object

	// rect
	unsigned rectVbo_ = 0; // vertex buffer object
	unsigned rectVao_ = 0;
	unsigned rectEao_ = 0;

	OT::Shader shader_;
};

class MainWindow : public OT::Window
{
public:

	MainWindow(Scene& scene) : OT::Window(800, 600, "Hello"), scene_(scene) {}

protected:

	void onKey(int key, int scancode, int action, int mods) override
	{
		if (key == GLFW_KEY_R)
		{
			if (action == GLFW_PRESS)
				scene_.drawRect = true;
			else if (action == GLFW_RELEASE)
				scene_.drawRect = false;
		}

		if (key == GLFW_KEY_D)
		{
			if (action == GLFW_PRESS)
				scene_.debug = true;
			else if (action == GLFW_RELEASE)
				scene_.debug = false;
		}

	}

private:

	Scene& scene_;
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
		Scene scene;

		MainWindow window(scene);

		window.makeContextCurrent();

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			throw std::runtime_error("Failed to initialize GLAD");
		}
		glViewport(0, 0, 800, 600);

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
