#pragma once

#include <glm/glm.hpp>

#include <vector>

namespace Rendering {

// Per-vertex-color
class PvcMesh
{
public:

	struct Vertex
	{
		glm::vec3 pos;
		glm::vec3 color;
	};
	static_assert(sizeof(Vertex) == sizeof(float)*6, "Vertex layout wrong");

	using Face = glm::uvec3;

	template<typename VertexIt, typename FaceIt>
	PvcMesh(VertexIt vb, VertexIt ve, FaceIt fb, FaceIt fe)
		: vertices_(vb, ve)
		, faces_(fb, fe)
	{
		setup();
	}

	PvcMesh() = default;
	PvcMesh(const PvcMesh&) = delete;
	PvcMesh(PvcMesh&&) = default;

	PvcMesh& operator=(PvcMesh&&) = default;

	void draw();

private:

	void setup();

	std::vector<Vertex> vertices_;
	std::vector<Face> faces_;

	unsigned vbo_ = 0;
	unsigned vao_ = 0;
	unsigned eao_ = 0;
};

}

