#pragma once

#include <boost/range/algorithm.hpp>

#include <cinttypes>
#include <string>
#include <unordered_map>
#include <memory>
#include <typeindex>
#include <typeinfo>

namespace Ecs {

using EntityId = std::uint64_t;
using ComponentTypeId = std::uint64_t;


class Ecs
{
public:

	// Creates new entity
	EntityId CreateEntity(const std::string& name);

	// Deletes entity and all associated components
	void DeleteEntity(EntityId id);

	// Registers a unique C++ component type
	template<typename CT>
	ComponentTypeId RegisterComponentType(const std::string& typeName);

	// Adds component to entity
	template<typename CT>
	CT& AddComponentToEntity(EntityId id);

private:

	struct AbstractComponentType
	{
		AbstractComponentType(const std::string& tn, std::type_index ti, ComponentTypeId id)
			: typeName_(tn), typeIndex_(ti), typeId_(id)
		{
		}

		std::string typeName_;
		std::type_index typeIndex_;
		ComponentTypeId typeId_;
	};

	template<typename CT>
	struct ComponentType : public AbstractComponentType
	{

		ComponentType(const std::string& tn, std::type_index ti, ComponentTypeId id)
			: AbstractComponentType(tn, ti, id)
		{
		}

		// Component storage
		// TODO use different container (Dense hashmap?)
		std::unordered_map<EntityId, CT> mData;

		CT& AddToEntity(EntityId id)
		{
			auto res = mData.emplace(id, CT{});
			if (res.second)
				return res.first->second;
			else
				throw std::logic_error("Entity already has component of this type");
		}

	};

	template<typename CT>
	ComponentType<CT>& GetComponentByType();

	std::unordered_map<EntityId, std::string> mEntityNames;
	std::uint64_t mNextFreeEntityId = 0;

	std::vector<std::unique_ptr<AbstractComponentType>> mComponentTypes;
};


template<typename CT>
ComponentTypeId Ecs::RegisterComponentType(const std::string& typeName)
{
	ComponentTypeId id = mComponentTypes.size();
	auto type = std::make_unique<ComponentType<CT>>(typeName, std::type_index(typeid(CT)), id);

	mComponentTypes.push_back(std::move(type));
	return id;
}

template<typename CT>
CT& Ecs::AddComponentToEntity(EntityId id)
{
	if (mEntityNames.find(id) == mEntityNames.end())
		throw std::logic_error("No such entity");


	ComponentType<CT>& type = GetComponentByType<CT>();
	return type.AddToEntity(id);
}

template<typename CT>
Ecs::ComponentType<CT>& Ecs::GetComponentByType()
{
	std::type_index ti(typeid(CT));

	auto it = boost::find_if(mComponentTypes, [&](const auto& tptr) { return tptr->typeIndex_ == ti; });
	if (it == mComponentTypes.end())
		throw std::logic_error("No component type for such C++ type");

	ComponentType<CT>* type = static_cast<ComponentType<CT>*>(it->get());
	assert(type);

	return *type;
}

}
