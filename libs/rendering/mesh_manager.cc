// (C) 2017 Maciej Gajewski
#include "mesh_manager.hh"

#include "mesh_utilities.hh"

namespace Rendering {

Mesh* MeshManager::CreateSphere(int segments)
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

} // namespace Rendering
