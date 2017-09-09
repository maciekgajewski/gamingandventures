// (C) 2017 Maciej Gajewski
#pragma once

#include <cstdint>

namespace Rendering {
	class Mesh;
}

namespace Components
{

struct MousePickable
{
	std::uint32_t value = 0;
	Rendering::Mesh* mesh = nullptr;
};

struct Selectable
{
	bool selected = false;
};

}

