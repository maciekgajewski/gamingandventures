// (C) 2017 Maciej Gajewski
#include "main_window.hh"
#include "rendering_system.hh"
#include "mesh_utilities.hh"

#include <ecs/ecs.hh>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>

#include <boost/optional.hpp>

#include <iostream>


void createWorld(Ecs::Ecs& database, RenderingSystem& renderer);

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
		Ecs::Ecs database;
		RenderingSystem renderingSystem;

		MainWindow window;
		window.makeContextCurrent();

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			throw std::runtime_error("Failed to initialize GLAD");
		}
		glViewport(0, 0, 800, 600);


		renderingSystem.Init(database);
		renderingSystem.SetCameraAspectRatio(800.0 / 600.0);

		// load world
		createWorld(database, renderingSystem);

		// main loop
		while (!window.shouldClose())
		{
			renderingSystem.Render(database);
			// TODO physics and others

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

void createWorld(Ecs::Ecs& database, RenderingSystem& renderer)
{
	// ball 1
	Ecs::EntityId ballId = database.CreateEntity("Ball 1");

	Components::Material& mat = database.AddUniqueComponentToEntity<Components::Material>(ballId);
	mat.diffuseTextureId = renderer.textureStore.LoadTexture("textures/ball.jpg");

	Components::Transformation& tr = database.AddUniqueComponentToEntity<Components::Transformation>(ballId);
	tr.transformation = glm::translate(glm::mat4(1.0), glm::vec3(1.0f, 1.0f, -5.0f));

	Components::Mesh& mesh = database.AddUniqueComponentToEntity<Components::Mesh>(ballId);
	mesh.meshId = renderer.meshStore.GetOrGenerate("sphere", [&] () { return OT::buildSphereMesh(); });

	// ball 2
	ballId = database.CreateEntity("Ball 2");

	Components::Material& mat2 = database.AddUniqueComponentToEntity<Components::Material>(ballId);
	mat2.diffuseTextureId = renderer.textureStore.LoadTexture("textures/ground.jpg");

	Components::Transformation& tr2 = database.AddUniqueComponentToEntity<Components::Transformation>(ballId);
	tr2.transformation = glm::translate(glm::mat4(1.0), glm::vec3(-2.0f, 2.0f, -12.0f));

	Components::Mesh& mesh2 = database.AddUniqueComponentToEntity<Components::Mesh>(ballId);
	mesh2.meshId = renderer.meshStore.GetOrGenerate("sphere", [&] () { return OT::buildSphereMesh(); });
}
