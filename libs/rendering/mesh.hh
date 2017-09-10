#pragma once

#include <glm/glm.hpp>

#include <vector>

namespace Rendering {

// Per-vertex-normal
class Mesh
{
public:

	struct Vertex
	{
		glm::vec3 pos;
		glm::vec3 normal;
		glm::vec2 st;
	};
	static_assert(sizeof(Vertex) == sizeof(float)*8, "Vertex layout wrong");

	using Face = glm::uvec3;

	template<typename VertexIt, typename FaceIt>
	Mesh(VertexIt vb, VertexIt ve, FaceIt fb, FaceIt fe)
		: vertices_(vb, ve)
		, faces_(fb, fe)
	{
		Setup();
	}

	template<typename VertexRange, typename FaceRange>
	Mesh(VertexRange&& vs, FaceRange&& fs)
		: vertices_(std::forward<VertexRange>(vs))
		, faces_(std::forward<FaceRange>(fs))
	{
		Setup();
	}

	Mesh() = default;
	Mesh(const Mesh&) = delete;
	Mesh(Mesh&&) = default;

	Mesh& operator=(Mesh&&) = default;

	void Draw();

private:

	void Setup();

	std::vector<Vertex> vertices_;
	std::vector<Face> faces_;

	unsigned vbo_ = 0;
	unsigned vao_ = 0;
	unsigned eao_ = 0;
};

} // namespace OT

