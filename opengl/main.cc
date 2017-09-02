#include "window.hh"
#include "io.hh"
#include "shader.hh"
#include "mesh_utilities.hh"
#include "mesh.hh"
#include "single_point_light_solid_phong_material.hh"

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>

#include <boost/optional.hpp>

#include <iostream>

class Scene
{
public:

	void init()
	{
		material_.emplace();
		mesh_ = OT::buildCubeMesh();

		// model transfrmation
		transformationUniform_ = material_->GetShader().GetUniform("model");

		modelTrans_ = glm::translate(glm::mat4(1.0f), {0.0f, 0.0f, -5.0f});
		transformationUniform_.Set(modelTrans_);

		// camera transformation
		cameraTransformationUniform_ = material_->GetShader().GetUniform("camera");
		cameraTrans_ = glm::mat4(1.0);

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

		// activate material
		material_->Use();
		material_->SetAmbientLight(glm::vec3(ambientLight_));
		material_->SetLightColor({1.0f, 1.0f, 1.0f});
		material_->SetLightPos({10.0f, 20.0f, -4.0f});
//		material_->SetLightAttenuation({1.0f, 0.0f, 0.0f}); // no attenuation

		material_->SetColor({1.0f, 0.5f, 0.5f});

		transformationUniform_.Set(modelTrans_);
		cameraTransformationUniform_.Set(cameraTrans_);

		material_->GetShader().GetUniform("projection").Set(projectionTrans_);

		if (debug)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		mesh_.Draw();
	}

	void rotate(float angle, const glm::vec3& axis)
	{
		//modelTrans_ = glm::rotate(modelTrans_,  glm::radians(1.0f), glm::vec3(1.0, 0.0, 0.0));
		modelTrans_ = glm::rotate(modelTrans_,  angle, axis);
		std::cout << "rorated" << std::endl;
	}

	bool debug = false;

	void setAspectRatio(float ar) { aspectRatio_ = ar; }

	void increaseAmbient()
	{
		if (ambientLight_ < 1.0f)
		{
			ambientLight_ += 0.05f;
			std::cout << "ambient light: " << ambientLight_ << std::endl;
		}
	}

	void decreaseAmbient()
	{
		if (ambientLight_ > 0.0f)
		{
			ambientLight_ -= 0.05f;
			std::cout << "ambient light: " << ambientLight_ << std::endl;
		}
	}

private:

	glm::mat4 modelTrans_;
	glm::mat4 projectionTrans_;
	glm::mat4 cameraTrans_;

	float aspectRatio_;

	OT::Uniform transformationUniform_;
	OT::Uniform cameraTransformationUniform_;

	OT::Mesh mesh_;

	boost::optional<OT::SinglePointLightSolidPhongMaterial> material_;
	float ambientLight_ = 0.1f;
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
		// on pressed actions
		if (action == GLFW_PRESS || action == GLFW_REPEAT)
		{
			static const float ROTATION_SPEED = 0.03;
			switch (key)
			{
				// rotation control
				// WSAD
				case GLFW_KEY_W:
					scene_.rotate(ROTATION_SPEED, {1.0f, 0.0f, 0.0f});
					break;
				case GLFW_KEY_S:
					scene_.rotate(-ROTATION_SPEED, {1.0f, 0.0f, 0.0f});
					break;
				case GLFW_KEY_A:
					scene_.rotate(ROTATION_SPEED, {0.0f, 1.0f, 0.0f});
					break;
				case GLFW_KEY_D:
					scene_.rotate(-ROTATION_SPEED, {0.0f, 1.0f, 0.0f});
					break;

				// light control
				// R-F - ambient light
				case GLFW_KEY_R:
					scene_.increaseAmbient();
					break;
				case GLFW_KEY_F:
					scene_.decreaseAmbient();
					break;

			}
		}

		if (key == GLFW_KEY_P)
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
