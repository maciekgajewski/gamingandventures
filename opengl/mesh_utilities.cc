#include "mesh_utilities.hh"

#include "mesh.hh"
#include "pvc_mesh.hh"

#include <vector>
#include <cmath>

namespace OT
{

PvcMesh buildCubePvcMesh()
{
	    // cube
		PvcMesh::Vertex vertices[] = { // xyz  rgb
		    {{0.5f,  0.5f, 0.5f},	{1.0f, 0.0f, 0.0f}},
		    {{0.5f, -0.5f, 0.5f},	{0.5f, 0.5f, 0.5f}},
		    {{-0.5f, -0.5f, 0.5f},	{0.0f, 1.0f, 0.0f}},
		    {{-0.5f,  0.5f, 0.5f},	{0.0f, 0.0f, 1.0f}},
		    {{0.5f,  0.5f, -0.5f},	{1.0f, 0.0f, 0.0f}},
		    {{0.5f, -0.5f, -0.5f},	{0.5f, 0.5f, 0.5f}},
		    {{-0.5f, -0.5f, -0.5f},	{0.0f, 1.0f, 0.0f}},
		    {{-0.5f,  0.5f, -0.5f},	{0.0f, 0.0f, 1.0f}},
		};

		PvcMesh::Face faces[] = {
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

	return PvcMesh(std::begin(vertices), std::end(vertices), std::begin(faces), std::end(faces));
}

Mesh buildCubeMesh()
{
	Mesh::Vertex vertices[] = { // xyz nnn
		{{-0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}},
		{{ 0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}},
		{{ 0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}},
		{{ 0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}},
		{{-0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}},
		{{-0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}},

		{{-0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}},
		{{ 0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}},
		{{ 0.5f,  0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}},
		{{ 0.5f,  0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}},
		{{-0.5f,  0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}},
		{{-0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}},

		{{-0.5f,  0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}},
		{{-0.5f,  0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}},
		{{-0.5f, -0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}},
		{{-0.5f, -0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}},
		{{-0.5f, -0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}},
		{{-0.5f,  0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}},

		{{ 0.5f,  0.5f,  0.5f}, { 1.0f,  0.0f,  0.0f}},
		{{ 0.5f,  0.5f, -0.5f}, { 1.0f,  0.0f,  0.0f}},
		{{ 0.5f, -0.5f, -0.5f}, { 1.0f,  0.0f,  0.0f}},
		{{ 0.5f, -0.5f, -0.5f}, { 1.0f,  0.0f,  0.0f}},
		{{ 0.5f, -0.5f,  0.5f}, { 1.0f,  0.0f,  0.0f}},
		{{ 0.5f,  0.5f,  0.5f}, { 1.0f,  0.0f,  0.0f}},

		{{-0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f,  0.0f}},
		{{ 0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f,  0.0f}},
		{{ 0.5f, -0.5f,  0.5f}, { 0.0f, -1.0f,  0.0f}},
		{{ 0.5f, -0.5f,  0.5f}, { 0.0f, -1.0f,  0.0f}},
		{{-0.5f, -0.5f,  0.5f}, { 0.0f, -1.0f,  0.0f}},
		{{-0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f,  0.0f}},

		{{-0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f,  0.0f}},
		{{ 0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f,  0.0f}},
		{{ 0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f,  0.0f}},
		{{ 0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f,  0.0f}},
		{{-0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f,  0.0f}},
		{{-0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f,  0.0f}}
	};

	Mesh::Face faces[12];

	for(int i = 0; i < 12; i++)
		faces[i] = {3*i+0, 3*i+1, 3*i+2};

	return Mesh(std::begin(vertices), std::end(vertices), std::begin(faces), std::end(faces));
}

Mesh buildSphereMesh(int segments)
{
	assert(segments > 4);

	std::vector<Mesh::Vertex> vertices;
	std::vector<Mesh::Face> faces;

	// Poles
	glm::vec3 northPole{0.0f, 1.0f, 0.0f};
	glm::vec3 southPole{0.0f, -1.0f, 0.0f};
	vertices.push_back({northPole, southPole});
	vertices.push_back({southPole, southPole});

	const unsigned nPoleIdx = 0;
	const unsigned sPoleIdx = 1;

	// polar caps
	float y = std::cos(2*M_PI/segments);
	float r = std::sin(2*M_PI/segments);

	unsigned prevNidx;
	unsigned prevSidx;

	for(int i = 0; i < segments; i++)
	{
		float x0 = r*std::sin(i * 2*M_PI/segments);
		float z0 = r*std::cos(i * 2*M_PI/segments);

		glm::vec3 north(x0, y, z0);
		glm::vec3 south(x0, -y, z0);

		assert(glm::length(north) - 1.0 < 0.001); // verify
		assert(glm::length(south) - 1.0 < 0.001); // verify

		vertices.push_back({north, north});
		vertices.push_back({south, south});

		unsigned nIdx = vertices.size() - 2;
		unsigned sIdx = vertices.size() - 1;
		if (i > 0)
		{
			// build CCW faces
			Mesh::Face northFace(prevNidx, nIdx, nPoleIdx);
			Mesh::Face southFace(sPoleIdx, sIdx, prevSidx);

			faces.push_back(northFace);
			faces.push_back(southFace);
		}

		prevNidx = nIdx;
		prevSidx = sIdx;
	}
	// closing faces
	{
		Mesh::Face northFace(prevNidx, 2, nPoleIdx);
		Mesh::Face southFace(sPoleIdx, 3, prevSidx);

		faces.push_back(northFace);
		faces.push_back(southFace);
	}

	// rings
	for(r = 1; r <

	return Mesh(
		vertices.begin(), vertices.end(),
		faces.begin(), faces.end());

}

}

