#include "mesh_utilities.hh"

#include "mesh.hh"

namespace OT
{

Mesh buildCubeMesh()
{
	    // cube
	    OT::Mesh::Vertex vertices[] = { // xyz  rgb
		    {{0.5f,  0.5f, 0.5f},	{1.0f, 0.0f, 0.0f}},
		    {{0.5f, -0.5f, 0.5f},	{0.5f, 0.5f, 0.5f}},
		    {{-0.5f, -0.5f, 0.5f},	{0.0f, 1.0f, 0.0f}},
		    {{-0.5f,  0.5f, 0.5f},	{0.0f, 0.0f, 1.0f}},
		    {{0.5f,  0.5f, -0.5f},	{1.0f, 0.0f, 0.0f}},
		    {{0.5f, -0.5f, -0.5f},	{0.5f, 0.5f, 0.5f}},
		    {{-0.5f, -0.5f, -0.5f},	{0.0f, 1.0f, 0.0f}},
		    {{-0.5f,  0.5f, -0.5f},	{0.0f, 0.0f, 1.0f}},
		};

		OT::Mesh::Face faces[] = {
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

	return OT::Mesh(std::begin(vertices), std::end(vertices), std::begin(faces), std::end(faces));
}

}

