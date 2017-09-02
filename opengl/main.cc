#include "window.hh"
#include "io.hh"
#include "shader.hh"
#include "mesh.hh"

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

class Scene
{
public:

	void init()
	{
		// cube
		OT::Mesh::Vertex verticesRect[] = { // xyz  rgb
			{{0.5f,  0.5f, 0.5f},	{1.0f, 0.0f, 0.0f}},
			{{0.5f, -0.5f, 0.5f},	{0.5f, 0.5f, 0.5f}},
			{{-0.5f, -0.5f, 0.5f},	{0.0f, 1.0f, 0.0f}},
			{{-0.5f,  0.5f, 0.5f},	{0.0f, 0.0f, 1.0f}},
			{{0.5f,  0.5f, -0.5f},	{1.0f, 0.0f, 0.0f}},
			{{0.5f, -0.5f, -0.5f},	{0.5f, 0.5f, 0.5f}},
			{{-0.5f, -0.5f, -0.5f},	{0.0f, 1.0f, 0.0f}},
			{{-0.5f,  0.5f, -0.5f},	{0.0f, 0.0f, 1.0f}},
		};

		OT::Mesh::Face indicesRect[] = {
			{3, 2, 0},
			{2, 1, 0},

			{0, 1, 4},
			{1, 5, 4},

			{4, 5, 7},
			{5, 6, 7},

			{7, 6, 3},
			{6, 2, 3},

			{5, 1, 6},
			{1, 2, 6},

			{0, 4, 3},
			{4, 7, 3},
		};

		mesh_ = OT::Mesh(std::begin(verticesRect), std::end(verticesRect), std::begin(indicesRect), std::end(indicesRect));

		// shaders
		shader_ = OT::Shader(OT::readFile("shaders/pvc_trans.vert"), OT::readFile("shaders/pvc_trans.frag"));
		transformationUniform_ = shader_.GetUniform("trans");
		trans_ = glm::mat4(1.0f);
		transformationUniform_.Set(trans_);

	}

	void mainLoop()
	{
		// clear
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// activate shader (material)
		shader_.Use();
		transformationUniform_.Set(trans_);

		if (debug)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		mesh_.Draw();
	}

	void rotate()
	{
		trans_ = glm::rotate(trans_,  glm::radians(1.0f), glm::vec3(1.0, 0.0, 0.0));
		std::cout << "rorated" << std::endl;
	}

	bool debug = false;

private:

	unsigned rectVbo_ = 0; // vertex buffer object
	unsigned rectVao_ = 0;
	unsigned rectEao_ = 0;
	unsigned vertices_ = 0;

	OT::Shader shader_;
	glm::mat4 trans_;
	OT::Uniform transformationUniform_;
	OT::Mesh mesh_;
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
			if (action == GLFW_PRESS || action == GLFW_REPEAT)
				scene_.rotate();
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
