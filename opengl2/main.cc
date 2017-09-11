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

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm-aabb/AABB.hpp>

#include <boost/optional.hpp>
#include <boost/filesystem.hpp>

#include <iostream>


std::ostream& operator<<(std::ostream& o, const glm::vec3& v)
{
	return o << "(" << v.x << ", " << v.y << ", " << v.z << ")";
}

void createWorld(Ecs::Ecs& database, Rendering::Renderer& renderer, RenderingSystem& renderingSystem);
void loadModel(Ecs::Ecs& database, Rendering::Renderer& renderer, RenderingSystem& renderingSystem, const std::string& path);

InputSystem* is = nullptr;
void onCursorPos(GLFWwindow* /*window*/, double xpos, double ypos)
{
	if (is)
		is->OnCursorMove(xpos, ypos);
}

float camMovement = 0.0;
float camRotation = 0.0;
void onKey(GLFWwindow* window, int key, int scancode, int action, int mods)
{

	if (action == GLFW_PRESS)
	{
		switch(key)
		{
			case GLFW_KEY_ESCAPE:
				glfwSetWindowShouldClose(window, true);
				break;

			case GLFW_KEY_W:
				camMovement = 1.0;
				break;

			case GLFW_KEY_S:
				camMovement = -1.0;
				break;

			case GLFW_KEY_A:
				camRotation = 1.0;
				break;

			case GLFW_KEY_D:
				camRotation = -1.0;
				break;

		}
	}

	if (action == GLFW_RELEASE)
	{
		switch(key)
		{
			case GLFW_KEY_S:
			case GLFW_KEY_W:
				camMovement = 0.0;
				break;
			case GLFW_KEY_A:
			case GLFW_KEY_D:
				camRotation = 0.0;
				break;
		}
	}

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

	double time = glfwGetTime();
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
		::glfwSetKeyCallback(window.win(), onKey);

		// load world
		//createWorld(database, renderer, renderingSystem);
		loadModel(database, renderer, renderingSystem, "models/wraith_raider/Wraith Raider Starship.obj");

		// main loop
		while (!window.shouldClose())
		{
			renderingSystem.Render();
			// TODO physics and others

			window.swapBuffers();
			inputSystem.PollEvents();

			// simluation
			double now = glfwGetTime();
			double dt = now - time;
			time = now;

			if (camMovement != 0.0)
			{
				// move camera up/down
				static const double ROTATION_SPEED = 0.5;
				Rendering::Camera cam = renderingSystem.getCamera();
				glm::vec3 axis = glm::vec3(-cam.getPosition().z, 0.0f, cam.getPosition().x);
				glm::vec3 position = glm::rotate(
						glm::mat4(1.0f),
						float(ROTATION_SPEED * dt * camMovement),
						axis
					) * glm::vec4(cam.getPosition(), 1.0);
				cam.setPosition(position);
				cam.lookAt(glm::vec3(0.0f));
				renderingSystem.setCamera(cam);
			}
			if (camRotation != 0.0)
			{
				// orbit camera arounf the origin
				static const double ROTATION_SPEED = 0.5;
				Rendering::Camera cam = renderingSystem.getCamera();
				glm::vec3 position = glm::rotate(
						glm::mat4(1.0f),
						float(ROTATION_SPEED * dt * camRotation),
						glm::vec3(0.0f, 1.0f, 0.0f)
					) * glm::vec4(cam.getPosition(), 1.0);
				cam.setPosition(position);
				cam.lookAt(glm::vec3(0.0f));
				renderingSystem.setCamera(cam);
			}

		}
	}
	catch(...)
	{
		glfwTerminate();
		throw;
	}

	glfwTerminate();
}

std::vector<Rendering::Mesh::Vertex> getVertices(aiMesh* mesh, glm::AABB& boundingBox)
{
	std::vector<Rendering::Mesh::Vertex> vertices;

	vertices.reserve(mesh->mNumVertices);
	for(int vi = 0; vi < mesh->mNumVertices; vi++)
	{
		Rendering::Mesh::Vertex vertex;
		aiVector3D& sourceVertex = mesh->mVertices[vi];
		vertex.pos = glm::vec3(
				sourceVertex.x,
				sourceVertex.y,
				sourceVertex.z
			);
		if (mesh->mNormals)
		{
			vertex.normal = glm::vec3(
					mesh->mNormals[vi].x,
					mesh->mNormals[vi].y,
					mesh->mNormals[vi].z
				);
		}
		aiVector3D* uvCoords = mesh->mTextureCoords[0];
		if (uvCoords)
		{
			vertex.st = glm::vec2(
					uvCoords[vi].x,
					uvCoords[vi].y
				);
		}

		vertices.push_back(vertex);
		boundingBox.extend(vertex.pos);
	} // vertices

	return vertices;
}

std::vector<Rendering::Mesh::Face> getFaces(aiMesh* mesh)
{
	std::vector<Rendering::Mesh::Face> faces;

	// copy faces
	faces.reserve(mesh->mNumFaces);
	for(int fi = 0; fi < mesh->mNumFaces; fi++)
	{
		const aiFace& aface = mesh->mFaces[fi];
		if (aface.mNumIndices != 3)
			throw std::runtime_error("I don't know how to handle face with " + std::to_string(aface.mNumIndices));

		Rendering::Mesh::Face face(
				aface.mIndices[0],
				aface.mIndices[1],
				aface.mIndices[2]
			);
		faces.push_back(face);
	} // faces

	return faces;
}


void loadModel(Ecs::Ecs& database, Rendering::Renderer& renderer, RenderingSystem& renderingSystem, const std::string& path)
{
	Assimp::Importer importer;
	boost::filesystem::path modelDir = boost::filesystem::path(path).remove_filename();

	glm::AABB boundingBox;

	const aiScene* scene = importer.ReadFile(
			path.c_str(),
			aiProcess_Triangulate
		);
//			aiProcess_CalcTangentSpace       |
//			aiProcess_Triangulate            |
//			aiProcess_JoinIdenticalVertices  |
//			aiProcess_SortByPType);
	if(!scene)
	{
		throw std::runtime_error("Error loading model from '" + path + "' : " + importer.GetErrorString());
	}

	// print some basic diagnostics
	std::cout << "Model loaded from " << path << std::endl;
	std::cout << " - meshes:    " << scene->mNumMeshes << std::endl;
	std::cout << " - materials: " << scene->mNumMaterials << std::endl;
	std::cout << " - textures:  " << scene->mNumTextures << std::endl;

	for(int mi = 0; mi < scene->mNumMeshes; mi++)
	{
		aiMesh* aMesh = scene->mMeshes[mi];
		aiMaterial* aMaterial = scene->mMaterials[aMesh->mMaterialIndex];

		aiString materialName;
		aiGetMaterialString(aMaterial, AI_MATKEY_NAME, &materialName);

		aiString texture;
		bool hasTexture = false;
		boost::filesystem::path texturePath;
		auto res = aiGetMaterialTexture(aMaterial, aiTextureType_DIFFUSE, 0, &texture);
		if (aiReturn_SUCCESS == res)
		{
			texturePath = modelDir / boost::filesystem::path(texture.C_Str());
			hasTexture = true;
		}


		std::cout << " ======================= " << std::endl;
		std::cout << "Mesh: " << aMesh->mName.C_Str() << std::endl;
		std::cout << " has normals: " << aMesh->HasNormals() << std::endl;
		//std::cout << " has text coords: " << mesh->HasTextureCoords( << std::endl;
		std::cout << " uv channels: "<< aMesh->GetNumUVChannels() << std::endl;
		std::cout << "material name: " << materialName.C_Str() << std::endl;
		if (hasTexture)
			std::cout << "texture: " << texturePath << std::endl;
		else
			std::cout << "no texture" << std::endl;

		float shinines = 0.0f;
		if (aMaterial->Get(AI_MATKEY_SHININESS, shinines) == aiReturn_SUCCESS)
		{
			std::cout << "Shinines: " << shinines << std::endl;
		}
		float shininesStrength;
		if (aMaterial->Get(AI_MATKEY_SHININESS_STRENGTH, shininesStrength) == aiReturn_SUCCESS)
		{
			std::cout << "Shinines strength: " << shininesStrength << std::endl;
		}
		float opacity = 1.0f;
		if (aMaterial->Get(AI_MATKEY_OPACITY, opacity) == aiReturn_SUCCESS)
		{
			std::cout << "Opacity: " << opacity << std::endl;
		}
		aiColor3D diffuse;
		if (aMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse) == aiReturn_SUCCESS)
		{
			std::cout << "Diffuse: " << diffuse.r << ", " << diffuse.g << ", " << diffuse.b << std::endl;
		}
		aiColor3D emmisive;
		if (aMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, emmisive) == aiReturn_SUCCESS)
		{
			std::cout << "Emmisive: " << emmisive.r << ", " << emmisive.g << ", " << emmisive.b << std::endl;
		}

		// build mesh
		std::vector<Rendering::Mesh::Vertex> vertices = getVertices(aMesh, boundingBox);
		std::vector<Rendering::Mesh::Face> faces = getFaces(aMesh);

		// get texture for mesh


		// create entity per mesh
		Ecs::EntityId entityId = database.CreateEntity(aMesh->mName.C_Str());
		// use proper material


		Rendering::Components::Material& mat = database.AddUniqueComponentToEntity<Rendering::Components::Material>(entityId);
		if (hasTexture)
		{
			mat.diffuseTexture = renderer.textures().load(texturePath.string());
		}
		else
		{
			mat.diffuseTexture = renderer.textures().getSolid({diffuse.r, diffuse.g, diffuse.b, 1.0f});
		}
		mat.shininess = shinines;
		mat.opacity = opacity;

		Rendering::Components::Transformation& tr = database.AddUniqueComponentToEntity<Rendering::Components::Transformation>(entityId);
		// mesh
		Rendering::Components::Mesh& meshComp = database.AddUniqueComponentToEntity<Rendering::Components::Mesh>(entityId);
		meshComp.mesh = new Rendering::Mesh(vertices, faces);
		// mouse
		database.AddUniqueComponentToEntity<Components::MousePickable>(entityId) = Components::MousePickable{ entityId, meshComp.mesh };
		database.AddUniqueComponentToEntity<Components::Selectable>(entityId) = Components::Selectable{ false };

	}// meshes

	std::cout << "Bounding box: " << boundingBox.getMin() << " - " << boundingBox.getMax() << std::endl;
	std::cout << "Center: " << boundingBox.getCenter() << std::endl;
	std::cout << "Longest edge: " << boundingBox.getLongestEdge() << std::endl;

	// camera
	Rendering::Camera cam;
	cam.setPosition(glm::vec3(600.0f, 200.0f, 100.0f));
	cam.lookAt(glm::vec3(0.0f, 50.0f, 0.0f)); // first ball
	renderingSystem.setCamera(cam);

	// light
	renderingSystem.SetAmbientLight(glm::vec3(0.05));
	renderingSystem.SetPointLight({600.0f, 600.0f, 600.0f}, glm::vec3(1.0));
}

void createWorld(Ecs::Ecs& database, Rendering::Renderer& renderer, RenderingSystem& renderingSystem)
{
	// ball 1
	Ecs::EntityId entityId = database.CreateEntity("Ball 1");

	Rendering::Components::Material& mat = database.AddUniqueComponentToEntity<Rendering::Components::Material>(entityId);
	mat.diffuseTexture = renderer.textures().load("textures/ball.jpg");

	Rendering::Components::Transformation& tr = database.AddUniqueComponentToEntity<Rendering::Components::Transformation>(entityId);
	tr.transformation = glm::translate(glm::mat4(1.0), glm::vec3(1.0f, 1.0f, -5.0f));

	Rendering::Components::Mesh& mesh = database.AddUniqueComponentToEntity<Rendering::Components::Mesh>(entityId);
	mesh.mesh = renderer.meshes().CreateSphere();

	database.AddUniqueComponentToEntity<Components::MousePickable>(entityId) = Components::MousePickable{ entityId, mesh.mesh };
	database.AddUniqueComponentToEntity<Components::Selectable>(entityId) = Components::Selectable{ true };

	// ball 2
	entityId = database.CreateEntity("Ball 2");

	Rendering::Components::Material& mat2 = database.AddUniqueComponentToEntity<Rendering::Components::Material>(entityId);
	mat2.diffuseTexture = renderer.textures().load("textures/ground.jpg");
	mat2.shininess = 2.0f;

	Rendering::Components::Transformation& tr2 = database.AddUniqueComponentToEntity<Rendering::Components::Transformation>(entityId);
	tr2.transformation = glm::translate(glm::mat4(1.0), glm::vec3(-2.0f, 2.0f, -12.0f));

	Rendering::Components::Mesh& mesh2 = database.AddUniqueComponentToEntity<Rendering::Components::Mesh>(entityId);
	mesh2.mesh = renderer.meshes().CreateSphere();

	database.AddUniqueComponentToEntity<Components::MousePickable>(entityId) = Components::MousePickable{ entityId, mesh2.mesh };
	database.AddUniqueComponentToEntity<Components::Selectable>(entityId) = Components::Selectable{ false };

	// ball 3
	entityId = database.CreateEntity("Ball 3");

	Rendering::Components::Material& mat3 = database.AddUniqueComponentToEntity<Rendering::Components::Material>(entityId);
	mat3.diffuseTexture = renderer.textures().load("textures/ball.jpg");

	Rendering::Components::Transformation& tr3 = database.AddUniqueComponentToEntity<Rendering::Components::Transformation>(entityId);
	tr3.transformation = glm::translate(glm::mat4(1.0), glm::vec3(-1.0f, -2.0f, -9.0f));

	Rendering::Components::Mesh& mesh3 = database.AddUniqueComponentToEntity<Rendering::Components::Mesh>(entityId);
	mesh3.mesh = renderer.meshes().CreateSphere();

	database.AddUniqueComponentToEntity<Components::MousePickable>(entityId) = Components::MousePickable{ entityId, mesh3.mesh };
	database.AddUniqueComponentToEntity<Components::Selectable>(entityId) = Components::Selectable{ false };

	// cube
	entityId = database.CreateEntity("Cube 1");

	Rendering::Components::Material& mat4 = database.AddUniqueComponentToEntity<Rendering::Components::Material>(entityId);
	mat4.diffuseTexture = renderer.textures().load("textures/ball.jpg");

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
	mesh4.mesh = renderer.meshes().CreateCube();

	database.AddUniqueComponentToEntity<Components::MousePickable>(entityId) = Components::MousePickable{ entityId, mesh4.mesh };
	database.AddUniqueComponentToEntity<Components::Selectable>(entityId) = Components::Selectable{ false };

	// camera
	Rendering::Camera cam;
	cam.setPosition(glm::vec3(0.0f));
	cam.lookAt(glm::vec3(1.0f, 1.0f, -5.0f)); // first ball
	cam.lookAt(glm::vec3(-2.0f, 2.0f, -12.0f)); // second ball
	renderingSystem.setCamera(cam);

	// light
	renderingSystem.SetAmbientLight(glm::vec3(0.05));
	renderingSystem.SetPointLight({1.0f, 5.0f, -4.0f}, glm::vec3(1.0));

}
