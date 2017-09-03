// (C) 2017 Maciej Gajewski
#pragma once

#include "ecs.hh"

namespace Ecs {

// Meta-programming trick - cobines c++ type and runtime index
template<typename CT, int INDEX>
struct TypeAndId
{
	using ComponentType = CT;
	static constexpr const int index = INDEX;
};





template<typename PrimaryCT, typename... CTs>
class Visitor
{
public:

private:


};

}
