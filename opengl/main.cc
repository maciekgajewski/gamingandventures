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
		float verticesRect[] = { // xyz  rgb
			 0.5f,  0.5f, 0.0f,		1.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f,		0.5f, 0.5f, 0.5f,
			-0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,
			 -0.5f,  0.5f, 0.0f,	0.0f, 0.0f, 1.0f,
		};
		unsigned indicesRect[] = {
			0, 1, 3,
			1, 2, 3
		};
		vertices_ = 6;

		// shaders
		shader_ = OT::Shader(OT::readFile("shaders/per_vertex_color.vert"), OT::readFile("shaders/per_vertex_color.frag"));

		// rectangle ======
		glGenVertexArrays(1, &rectVao_);
		glBindVertexArray(rectVao_);

		glGenBuffers(1, &rectEao_);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rectEao_);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesRect), indicesRect, GL_STATIC_DRAW);

		glGenBuffers(1, &rectVbo_);
		glBindBuffer(GL_ARRAY_BUFFER, rectVbo_);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verticesRect), verticesRect, GL_STATIC_DRAW);

		// first param: xyz
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); // uses currently bound VAO
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

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
		shader_.Use();

		if (debug)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

//		if (drawRect)
//			color.Set(glm::vec3(1.0f, 0.0f, 0.0f));
//		else
//			color.Set(glm::vec3(0.0f, 1.0f, 1.0f));

		glBindVertexArray(rectVao_);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rectEao_); // why is this needed?
		glDrawElements(GL_TRIANGLES, vertices_, GL_UNSIGNED_INT, 0);

	}

	bool drawRect = false;
	bool debug = false;

private:

	unsigned rectVbo_ = 0; // vertex buffer object
	unsigned rectVao_ = 0;
	unsigned rectEao_ = 0;
	unsigned vertices_ = 0;

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
