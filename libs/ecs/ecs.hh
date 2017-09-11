// (C) 2017 Maciej Gajewski
#pragma once

#include "component_type.hh"

#include <boost/range/algorithm.hpp>

#include <cinttypes>
#include <string>
#include <unordered_map>
#include <memory>
#include <tuple>

namespace Ecs {

class Ecs
{
public:

	// Creates new entity
	EntityId createEntity(const std::string& name);

	// Deletes entity and all associated components
	void deleteEntity(EntityId id);

	// Registers an automatic (compile-time detected) C++ component type. The C++ type will be bound to the returned ID.
	// The registered C++ type must be unique
	//
	// Use addComponentType when re-using C++ types is needed
	template<typename CT>
	ComponentTypeId registerAutoComponentType(const std::string& typeName);

	// Returs Id of automatic type, thorws if no such type
	template<typename CT>
	ComponentTypeId getAutoComponentTypeId();

	// Adds uniquely-typed component to entity
	template<typename CT>
	CT& addComponentToEntity(EntityId eid, ComponentTypeId cid, const CT& value = CT{});

	// Adds uniquely-typed component to entity
	template<typename CT>
	CT& addAutoComponentToEntity(EntityId id, const CT& value = CT{});

	// Gets component storage by id
	template<typename CT>
	ComponentType<CT>& getComponentType(ComponentTypeId id);

	// Gets automatic compontent type by type alone
	template<typename CT>
	ComponentType<CT>& getAutoComponentType();

private:

	std::unordered_map<EntityId, std::string> entityNames_;
	std::uint64_t mNextFreeEntityId = 1;

	std::unordered_map<ComponentTypeId, std::unique_ptr<AbstractComponentType>> componentTypes_;

	std::unordered_map<std::type_index, ComponentTypeId> uniqueTypes_;
};

template<typename CT>
ComponentTypeId Ecs::registerAutoComponentType(const std::string& typeName)
{
	if (uniqueTypes_.find(std::type_index(typeid(CT))) != uniqueTypes_.end())
		throw std::logic_error(std::string("C++ type '") + typeid(CT).name() + "' already resgistered");

	ComponentTypeId id = componentTypes_.size();
	auto type = std::make_unique<ComponentType<CT>>(typeName, std::type_index(typeid(CT)), id);

	auto r = componentTypes_.emplace(id, std::move(type));
	(void)r;
	assert(r.second);

	uniqueTypes_.emplace(std::type_index(typeid(CT)), id);

	return id;
}

template<typename CT>
CT& Ecs::addComponentToEntity(EntityId eid, ComponentTypeId cid, const CT& value)
{
	if (entityNames_.find(eid) == entityNames_.end())
		throw std::logic_error("No such entity");

	ComponentType<CT>& type = getComponentType<CT>(cid);
	return type.addToEntity(eid, value);
}

template<typename CT>
CT& Ecs::addAutoComponentToEntity(EntityId eid, const CT& value)
{
	if (entityNames_.find(eid) == entityNames_.end())
		throw std::logic_error("No such entity");

	ComponentType<CT>& type = getAutoComponentType<CT>();
	return type.addToEntity(eid, value);
}


template<typename CT>
ComponentType<CT>& Ecs::getComponentType(ComponentTypeId id)
{
	auto it = componentTypes_.find(id);
	if (it == componentTypes_.end())
		throw std::logic_error("No component type for such id");

	AbstractComponentType& abstractType = *(it->second);

	std::type_index expectedTi(typeid(CT));

	if (abstractType.getCppType() != expectedTi)
		throw std::logic_error("Invalid component type");

	return static_cast<ComponentType<CT>&>(abstractType);
}


template<typename CT>
ComponentTypeId Ecs::getAutoComponentTypeId()
{
	std::type_index expectedTi(typeid(CT));
	auto typeIt = uniqueTypes_.find(expectedTi);
	if (typeIt == uniqueTypes_.end())
		throw std::logic_error(std::string("Not a registered automatic type: '") + typeid(CT).name() + "'");

	return typeIt->second;
}

template<typename CT>
ComponentType<CT>& Ecs::getAutoComponentType()
{
	ComponentTypeId id = getAutoComponentTypeId<CT>();
	return getComponentType<CT>(id);
}

}
