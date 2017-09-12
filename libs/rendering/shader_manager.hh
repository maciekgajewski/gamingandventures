// (C) 2017 Maciej Gajewski
#pragma once

#include "shader.hh"

#include <boost/functional/hash.hpp>

#include <string>
#include <unordered_map>

namespace Rendering {

class ShaderManager
{
public:
	ShaderManager();

	Shader* load(const std::string& vert, const std::string& frag);

private:

	using Key = std::pair<std::string, std::string>;

	std::unordered_map<Key, Shader, boost::hash<Key>> mShaders;
};

} // namespace Rendering

