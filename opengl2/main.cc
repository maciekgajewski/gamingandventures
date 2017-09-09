// (C) 2017 Maciej Gajewski
#include "main_window.hh"

#include "rendering_system.hh"
#include "input_system.hh"
#include "components.hh"

#include <rendering/mesh_utilities.hh>
#include <rendering/renderer.hh>
#include <rendering/components.hh>


#include <ecs/ecs.hh>

#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>

#include <boost/optional.hpp>

#include <iostream>


void createWorld(Ecs::Ecs& database, Rendering::Renderer& renderer, RenderingSystem& renderingSystem);

InputSystem* is = nullptr;

void onCursorPos(GLFWwindow* /*window*/, double xpos, double ypos)
{
	if (is)
		is->OnCursorMove(xpos, ypos);
}

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

		renderingSystem.Init();
		renderingSystem.SetViewport(0, 0, 800, 600);


		InputSystem inputSystem(renderingSystem, database);
		inputSystem.Init();
		inputSystem.SetViewport(0, 0, 800, 600);
		is = &inputSystem;

		// intercept input
		::glfwSetCursorPosCallback(window.win(), onCursorPos);


		// load world
		createWorld(database, renderer, renderingSystem);

		renderingSystem.RenderToFile();

		// main loop
		while (!window.shouldClose())
		{
			renderingSystem.Render();
			// TODO physics and others

			window.swapBuffers();
			inputSystem.PollEvents();
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
	Ecs::EntityId entityId = database.CreateEntity("Ball 1");

	Rendering::Components::Material& mat = database.AddUniqueComponentToEntity<Rendering::Components::Material>(entityId);
	mat.diffuseTexture = renderer.Textures().Load("textures/ball.jpg");

	Rendering::Components::Transformation& tr = database.AddUniqueComponentToEntity<Rendering::Components::Transformation>(entityId);
	tr.transformation = glm::translate(glm::mat4(1.0), glm::vec3(1.0f, 1.0f, -5.0f));

	Rendering::Components::Mesh& mesh = database.AddUniqueComponentToEntity<Rendering::Components::Mesh>(entityId);
	mesh.mesh = renderer.Meshes().CreateSphere();

	database.AddUniqueComponentToEntity<Components::MousePickable>(entityId) = { 0x00ff0000, mesh.mesh };

	// ball 2
	entityId = database.CreateEntity("Ball 2");

	Rendering::Components::Material& mat2 = database.AddUniqueComponentToEntity<Rendering::Components::Material>(entityId);
	mat2.diffuseTexture = renderer.Textures().Load("textures/ground.jpg");
	mat2.shininess = 2.0f;

	Rendering::Components::Transformation& tr2 = database.AddUniqueComponentToEntity<Rendering::Components::Transformation>(entityId);
	tr2.transformation = glm::translate(glm::mat4(1.0), glm::vec3(-2.0f, 2.0f, -12.0f));

	Rendering::Components::Mesh& mesh2 = database.AddUniqueComponentToEntity<Rendering::Components::Mesh>(entityId);
	mesh2.mesh = renderer.Meshes().CreateSphere();

	database.AddUniqueComponentToEntity<Components::MousePickable>(entityId) = { 0x00ffff00, mesh2.mesh };

	// ball 3
	entityId = database.CreateEntity("Ball 3");

	Rendering::Components::Material& mat3 = database.AddUniqueComponentToEntity<Rendering::Components::Material>(entityId);
	mat3.diffuseTexture = renderer.Textures().Load("textures/ball.jpg");

	Rendering::Components::Transformation& tr3 = database.AddUniqueComponentToEntity<Rendering::Components::Transformation>(entityId);
	tr3.transformation = glm::translate(glm::mat4(1.0), glm::vec3(-1.0f, -2.0f, -9.0f));

	Rendering::Components::Mesh& mesh3 = database.AddUniqueComponentToEntity<Rendering::Components::Mesh>(entityId);
	mesh3.mesh = renderer.Meshes().CreateSphere();

	database.AddUniqueComponentToEntity<Components::MousePickable>(entityId) = { 0x0000ff00, mesh3.mesh };

	// cube
	entityId = database.CreateEntity("Cube 1");

	Rendering::Components::Material& mat4 = database.AddUniqueComponentToEntity<Rendering::Components::Material>(entityId);
	mat4.diffuseTexture = renderer.Textures().Load("textures/ball.jpg");

	Rendering::Components::Transformation& tr4 = database.AddUniqueComponentToEntity<Rendering::Components::Transformation>(entityId);
	tr4.transformation =
		glm::rotate(
			glm::scale(
				glm::translate(glm::mat4(1.0), glm::vec3(-1.0f, 1.0f, -15.0f)),
				glm::vec3(2.0)
			),
			-0.4f,
			glm::vec3(0.0f, 1.0f, -0.5f)
		);


	Rendering::Components::Mesh& mesh4 = database.AddUniqueComponentToEntity<Rendering::Components::Mesh>(entityId);
	mesh4.mesh = renderer.Meshes().CreateCube();

	database.AddUniqueComponentToEntity<Components::MousePickable>(entityId) = { 0x0000ffff, mesh4.mesh };

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
