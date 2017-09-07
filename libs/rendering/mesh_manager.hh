// (C) 2017 Maciej Gajewski
#pragma once

#include "mesh.hh"

#include <unordered_map>
#include <string>

namespace Rendering {


class MeshManager
{
public:

	// TODO
	// Mesh* LoadFromFile(path)

	Mesh* CreateSphere(int segments = 10);

private:

	std::unordered_map<std::string, Mesh> mMeshes;

};

} // namespace Rendering

