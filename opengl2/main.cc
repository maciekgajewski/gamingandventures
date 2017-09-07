// (C) 2017 Maciej Gajewski
#include "main_window.hh"

#include "rendering_system.hh"

//#include <rendering/rendering_system.hh>
#include <rendering/mesh_utilities.hh>
#include <rendering/renderer.hh>
#include <rendering/components.hh>


#include <ecs/ecs.hh>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>

#include <boost/optional.hpp>

#include <iostream>


void createWorld(Ecs::Ecs& database, Rendering::Renderer& renderer, RenderingSystem& renderingSystem);

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

		MainWindow window;
		window.makeContextCurrent();

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			throw std::runtime_error("Failed to initialize GLAD");
		}

		Rendering::Renderer renderer;
		RenderingSystem renderingSystem(renderer, database);

		renderingSystem.SetViewport(0, 0, 800, 600);

		renderingSystem.Init();

		// load world
		createWorld(database, renderer, renderingSystem);

		// main loop
		while (!window.shouldClose())
		{
			renderingSystem.Render();
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

void createWorld(Ecs::Ecs& database, Rendering::Renderer& renderer, RenderingSystem& renderingSystem)
{
	// ball 1
	Ecs::EntityId ballId = database.CreateEntity("Ball 1");

	Rendering::Components::Material& mat = database.AddUniqueComponentToEntity<Rendering::Components::Material>(ballId);
	mat.diffuseTexture = renderer.Textures().Load("textures/ball.jpg");

	Rendering::Components::Transformation& tr = database.AddUniqueComponentToEntity<Rendering::Components::Transformation>(ballId);
	tr.transformation = glm::translate(glm::mat4(1.0), glm::vec3(1.0f, 1.0f, -5.0f));

	Rendering::Components::Mesh& mesh = database.AddUniqueComponentToEntity<Rendering::Components::Mesh>(ballId);
	mesh.mesh = renderer.Meshes().CreateSphere();

	// ball 2
	ballId = database.CreateEntity("Ball 2");

	Rendering::Components::Material& mat2 = database.AddUniqueComponentToEntity<Rendering::Components::Material>(ballId);
	mat2.diffuseTexture = renderer.Textures().Load("textures/ground.jpg");
	mat2.shininess = 2.0f;

	Rendering::Components::Transformation& tr2 = database.AddUniqueComponentToEntity<Rendering::Components::Transformation>(ballId);
	tr2.transformation = glm::translate(glm::mat4(1.0), glm::vec3(-2.0f, 2.0f, -12.0f));

	Rendering::Components::Mesh& mesh2 = database.AddUniqueComponentToEntity<Rendering::Components::Mesh>(ballId);
	mesh2.mesh = renderer.Meshes().CreateSphere();

	// ball 3
	ballId = database.CreateEntity("Ball 3");

	Rendering::Components::Material& mat3 = database.AddUniqueComponentToEntity<Rendering::Components::Material>(ballId);
	mat3.diffuseTexture = renderer.Textures().Load("textures/ball.jpg");

	Rendering::Components::Transformation& tr3 = database.AddUniqueComponentToEntity<Rendering::Components::Transformation>(ballId);
	tr3.transformation = glm::translate(glm::mat4(1.0), glm::vec3(-1.0f, -2.0f, -9.0f));

	Rendering::Components::Mesh& mesh3 = database.AddUniqueComponentToEntity<Rendering::Components::Mesh>(ballId);
	mesh3.mesh = renderer.Meshes().CreateSphere();

	// camera
	Rendering::Camera cam;
	cam.SetPosition(glm::vec3(0.0f));
	cam.LookAt(glm::vec3(1.0f, 1.0f, -5.0f)); // first ball
	cam.LookAt(glm::vec3(-2.0f, 2.0f, -12.0f)); // second ball
	renderingSystem.SetCamera(cam);

	// light
	renderingSystem.SetAmbientLight(glm::vec3(0.05));
	renderingSystem.SetPointLight({1.0f, 5.0f, -4.0f}, glm::vec3(1.0));

}
