// (C) 2017 Maciej Gajewski
#include "ecs.hh"

#include <cassert>

namespace Ecs {

EntityId Ecs::createEntity(const std::string& name)
{
	auto id = mNextFreeEntityId;
	mNextFreeEntityId++;

	if (mNextFreeEntityId == 0)
		throw std::runtime_error("Exchausted entity ids");

	auto r = entityNames_.insert({id, name});
	assert(r.second);
	(void)r;

	return id;
}

}
