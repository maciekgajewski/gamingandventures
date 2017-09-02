#include "window.hh"
#include "io.hh"
#include "shader.hh"
#include "mesh_utilities.hh"
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
		mesh_ = OT::buildCubeMesh();

		// shaders
		shader_ = OT::Shader(OT::readFile("shaders/pvc_trans.vert"), OT::readFile("shaders/pvc_trans.frag"));

		// model transfrmation
		transformationUniform_ = shader_.GetUniform("model");

		modelTrans_ = glm::translate(glm::mat4(1.0f), {0.0f, 0.0f, -5.0f});
		transformationUniform_.Set(modelTrans_);

		// camera transformation
		cameraTransformationUniform_ = shader_.GetUniform("camera");
		cameraTrans_ = glm::mat4(1.0);

		OT::Uniform projectionUniform = shader_.GetUniform("projection");
		projectionTrans_ = glm::perspective(glm::radians(45.0f),
			aspectRatio_,
			0.1f, // near
			1000.0f // far
			);

		glEnable(GL_DEPTH_TEST);
	}

	void mainLoop()
	{
		// clear
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// activate shader (material)
		shader_.Use();
		transformationUniform_.Set(modelTrans_);
		cameraTransformationUniform_.Set(cameraTrans_);
		shader_.GetUniform("projection").Set(projectionTrans_);

		if (debug)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		mesh_.Draw();
	}

	void rotate()
	{
		modelTrans_ = glm::rotate(modelTrans_,  glm::radians(1.0f), glm::vec3(1.0, 0.0, 0.0));
		std::cout << "rorated" << std::endl;
	}

	bool debug = false;

	void setAspectRatio(float ar) { aspectRatio_ = ar; }

private:

	OT::Shader shader_;
	glm::mat4 modelTrans_;
	glm::mat4 projectionTrans_;
	glm::mat4 cameraTrans_;
	float aspectRatio_;

	OT::Uniform transformationUniform_;
	OT::Uniform cameraTransformationUniform_;

	OT::Mesh mesh_;
};

class MainWindow : public OT::Window
{
public:

	MainWindow(Scene& scene) : OT::Window(800, 600, "Hello"), scene_(scene)
	{
		scene.setAspectRatio(800.0/600.0);
	}

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
