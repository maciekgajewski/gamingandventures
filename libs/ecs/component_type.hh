// (C) 2017 Maciej Gajewski
#pragma once

#include <string>
#include <unordered_map>
#include <typeindex>
#include <typeinfo>

namespace Ecs {

// Component type defines per-type component sotrage.
// Components are stored in an associative container, uniquely identified by Entiti id

using EntityId = std::uint32_t;
using ComponentTypeId = std::uint32_t;

// Base, type-erased type
class AbstractComponentType
{
public:
	AbstractComponentType(const std::string& tn, std::type_index ti, ComponentTypeId id)
		: typeName_(tn), typeIndex_(ti), typeId_(id)
	{
	}

	const std::string& getName() const { return typeName_; }
	std::type_index getCppType() const { return typeIndex_; }

private:
	std::string typeName_;
	std::type_index typeIndex_;
	ComponentTypeId typeId_;
};


// Typed implementation. Contains the actual storage
template<typename CT>
struct ComponentType : public AbstractComponentType
{
public:

	ComponentType(const std::string& tn, std::type_index ti, ComponentTypeId id)
		: AbstractComponentType(tn, ti, id)
	{
	}

	CT& addToEntity(EntityId id, const CT& value)
	{
		auto res = mData.emplace(id, value);
		if (res.second)
			return res.first->second;
		else
			throw std::logic_error("Entity already has component of this type");
	}

	// Iterates over all components
	// Expects functor with the following or compatible signature:
	//
	// (EntityId, CT&);
	template<typename F>
	void forEach(F f)
	{
		for(auto& p : mData)
		{
			f(p.first, p.second);
		}
	}

	CT* find(EntityId id)
	{
		auto it = mData.find(id);
		if (it == mData.end())
		{
			return nullptr;
		}
		else
		{
			return &it->second;
		}
	}

private:

	// Component storage
	// TODO use different container (Dense hashmap?)
	std::unordered_map<EntityId, CT> mData;
};


}
