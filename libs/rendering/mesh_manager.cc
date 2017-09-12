// (C) 2017 Maciej Gajewski
#include "mesh_manager.hh"

#include "mesh_utilities.hh"

namespace Rendering {

Mesh* MeshManager::createSphere(int segments)
{
	std::string name = "spehere" + std::to_string(segments);

	auto it = mMeshes.find(name);

	if (it == mMeshes.end())
	{
		Mesh m = buildSphereMesh(segments);
		auto res = mMeshes.emplace(name, std::move(m));
		assert(res.second);
		it = res.first;
	}

	return &(it->second);

}

Mesh*MeshManager::createCube()
{
	std::string name = "cube";

	auto it = mMeshes.find(name);

	if (it == mMeshes.end())
	{
		Mesh m = buildCubeMesh();
		auto res = mMeshes.emplace(name, std::move(m));
		assert(res.second);
		it = res.first;
	}

	return &(it->second);
}

} // namespace Rendering
