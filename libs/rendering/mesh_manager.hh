// (C) 2017 Maciej Gajewski
#pragma once

#include "mesh.hh"

#include <unordered_map>
#include <string>

namespace Rendering {


class MeshManager
{
public:

	// TODO loading from file

	Mesh* createSphere(int segments = 10);
	Mesh* createCube();

private:

	std::unordered_map<std::string, Mesh> mMeshes;

};

} // namespace Rendering

