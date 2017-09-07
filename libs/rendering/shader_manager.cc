// (C) 2017 Maciej Gajewski
#include "shader_manager.hh"

namespace Rendering {


ShaderManager::ShaderManager()
{
}

Shader* ShaderManager::Load(const std::string& vert, const std::string& frag)
{
	Key key(vert, frag);
	auto it = mShaders.find(key);

	if (it == mShaders.end())
	{
		Shader s(vert, vert, frag, frag);

		auto res = mShaders.emplace(key, std::move(s));
		assert(res.second);
		it = res.first;
	}

	return &(it->second);
}

} // namespace Rendering
