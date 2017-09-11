// (C) 2017 Maciej Gajewski
#pragma once

#include <glm/glm.hpp>

// rendering components
// TODO I'm not sure if this belongs here
namespace Rendering {

class Mesh;
class Texture;

namespace Components {


// Required for rendering
struct Material
{
	Texture* diffuseTexture = nullptr;
	float shininess = 0.5f;
	float opacity = 1.0f;
};

struct Mesh
{
	Rendering::Mesh* mesh = nullptr;
};

struct Transformation
{
	glm::mat4 transformation = glm::mat4(1.0f);
};



}} // namespace Rendering

