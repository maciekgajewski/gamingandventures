#pragma once

#include <boost/range/algorithm.hpp>

#include <cinttypes>
#include <string>
#include <unordered_map>
#include <memory>
#include <typeindex>
#include <typeinfo>
#include <tuple>

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
	CT& AddComponentToEntity(EntityId id, const CT& value = CT{});

	// Iterates over entities with specific components
	template<typename CT, typename... CTs, typename F>
	void ForEach(F fun);

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

		CT& AddToEntity(EntityId id, const CT& value)
		{
			auto res = mData.emplace(id, value);
			if (res.second)
				return res.first->second;
			else
				throw std::logic_error("Entity already has component of this type");
		}

		template<typename F>
		void ForEach(F f)
		{
			for(auto& p : mData)
			{
				f(p.first, p.second);
			}
		}

		CT* Find(EntityId id, bool& bother)
		{
			if (!bother)
				return nullptr;

			auto it = mData.find(id);
			if (it == mData.end())
			{
				bother = false;
				return nullptr;
			}
			else
			{
				return &it->second;
			}
		}

		// Component storage
		// TODO use different container (Dense hashmap?)
		std::unordered_map<EntityId, CT> mData;
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
CT& Ecs::AddComponentToEntity(EntityId id, const CT& value)
{
	if (mEntityNames.find(id) == mEntityNames.end())
		throw std::logic_error("No such entity");


	ComponentType<CT>& type = GetComponentByType<CT>();
	return type.AddToEntity(id, value);
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

}
