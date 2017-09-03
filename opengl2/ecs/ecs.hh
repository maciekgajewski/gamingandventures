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
	EntityId CreateEntity(const std::string& name);

	// Deletes entity and all associated components
	void DeleteEntity(EntityId id);

	// Registers a unique C++ component type. The C++ will be bound to the returned ID.
	// Only one compennt with this C++ type can exist.
	//
	// Use AddComponentType when re-using C++ types is needed
	template<typename CT>
	ComponentTypeId RegisterUniqueComponentType(const std::string& typeName);

	// Returs Id of unique type, thorws if no such type
	template<typename CT>
	ComponentTypeId GetUniqueComponentTypeId();

	// Adds uniquely-typed component to entity
	template<typename CT>
	CT& AddComponentToEntity(EntityId eid, ComponentTypeId cid, const CT& value = CT{});

	// Adds uniquely-typed component to entity
	template<typename CT>
	CT& AddUniqueComponentToEntity(EntityId id, const CT& value = CT{});

	// Gets component storage by id
	template<typename CT>
	ComponentType<CT>& GetComponentType(ComponentTypeId id);

	// Gets unique compontent type by type alone
	template<typename CT>
	ComponentType<CT>& GetUniqueComponentType();

private:

	std::unordered_map<EntityId, std::string> entityNames_;
	std::uint64_t mNextFreeEntityId = 0;

	std::unordered_map<ComponentTypeId, std::unique_ptr<AbstractComponentType>> componentTypes_;

	std::unordered_map<std::type_index, ComponentTypeId> uniqueTypes_;
};

template<typename CT>
ComponentTypeId Ecs::RegisterUniqueComponentType(const std::string& typeName)
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
CT& Ecs::AddComponentToEntity(EntityId eid, ComponentTypeId cid, const CT& value)
{
	if (entityNames_.find(eid) == entityNames_.end())
		throw std::logic_error("No such entity");

	ComponentType<CT>& type = GetComponentType<CT>(cid);
	return type.AddToEntity(eid, value);
}

template<typename CT>
CT& Ecs::AddUniqueComponentToEntity(EntityId eid, const CT& value)
{
	if (entityNames_.find(eid) == entityNames_.end())
		throw std::logic_error("No such entity");

	ComponentType<CT>& type = GetUniqueComponentType<CT>();
	return type.AddToEntity(eid, value);
}


template<typename CT>
ComponentType<CT>& Ecs::GetComponentType(ComponentTypeId id)
{
	auto it = componentTypes_.find(id);
	if (it == componentTypes_.end())
		throw std::logic_error("No component type for such id");

	AbstractComponentType& abstractType = *(it->second);

	std::type_index expectedTi(typeid(CT));

	if (abstractType.GetCppType() != expectedTi)
		throw std::logic_error("Invalid component type");

	return static_cast<ComponentType<CT>&>(abstractType);
}


template<typename CT>
ComponentTypeId Ecs::GetUniqueComponentTypeId()
{
	std::type_index expectedTi(typeid(CT));
	auto typeIt = uniqueTypes_.find(expectedTi);
	if (typeIt == uniqueTypes_.end())
		throw std::logic_error(std::string("Not a registered unique type: '") + typeid(CT).name() + "'");

	return typeIt->second;
}

template<typename CT>
ComponentType<CT>& Ecs::GetUniqueComponentType()
{
	ComponentTypeId id = GetUniqueComponentTypeId<CT>();
	return GetComponentType<CT>(id);
}


/*
template<typename T> struct GetCtFromTypePtr;
template<typename CT> struct GetCtFromTypePtr<Ecs::ComponentType<CT>*>
{
	using type = CT;
};

template<typename F, typename CT, typename... CTs>
void ForwardIf(F fun, EntityId id, bool cond, CT& primary, CTs*... ptrs)
{
	if (cond)
		fun(id, primary, *ptrs...);
}

template<typename CT, typename... CTs, typename F>
void Ecs::ForEach(F fun)
{
	ComponentType<CT>& primaryType = GetComponentByType<CT>();
	primaryType.ForEach([&](EntityId id, CT& component)
	{
		bool allPresent = true;
		ForwardIf(fun, id, allPresent, component, (GetComponentByType<CTs>().Find(id, allPresent))...);
	});
}
*/

}
