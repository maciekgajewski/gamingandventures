#include "ecs.hh"

#include <cassert>

namespace Ecs {

EntityId Ecs::CreateEntity(const std::string& name)
{
	auto id = mNextFreeEntityId;
	mNextFreeEntityId++;

	if (mNextFreeEntityId == 0)
		throw std::runtime_error("Exchausted entity ids");

	auto r = mEntityNames.insert({id, name});
	assert(r.second);
	(void)r;

	return id;
}

}
