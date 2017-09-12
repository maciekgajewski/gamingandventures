// (C) 2017 Maciej Gajewski
#include "shader_manager.hh"

#include <utils/io.hh>

namespace Rendering {


ShaderManager::ShaderManager()
{
}

Shader* ShaderManager::load(const std::string& vert, const std::string& frag)
{
	Key key(vert, frag);
	auto it = mShaders.find(key);

	std::string vertSrc = Utils::readFile(vert);
	std::string fragSrc = Utils::readFile(frag);

	if (it == mShaders.end())
	{
		Shader s(vertSrc, vert, fragSrc, frag);

		auto res = mShaders.emplace(key, std::move(s));
		assert(res.second);
		it = res.first;
	}

	return &(it->second);
}

} // namespace Rendering
