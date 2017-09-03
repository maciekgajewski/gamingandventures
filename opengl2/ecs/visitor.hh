// (C) 2017 Maciej Gajewski
#pragma once

#include "ecs.hh"

#include <array>

namespace Ecs {


namespace Meta {
// Meta-programming trick - combines c++ type and runtime index
template<typename CT, int INDEX>
struct TypeAndId
{
	using ComponentType = CT;
	static constexpr const int index = INDEX;
};

template<typename F, typename CT, typename... CTs>
void ForwardIf(F fun, EntityId id, bool cond, CT& primary, CTs*... ptrs)
{
	if (cond)
		fun(id, primary, *ptrs...);
}

// Classic tuple iteration
template<int ...> struct Seq {};

template<int N, int ...S> struct GenSeq : GenSeq<N-1, N-1, S...> {};

template<int ...S> struct GenSeq<0, S...>{ using type = Seq<S...>; };


}

// a type that combines C++ compontent type with type ID
template<typename CT>
struct CTypeId
{
	CTypeId(ComponentTypeId id) : id(id) {}

	using type = CT;
	ComponentTypeId id;
};

template<typename PrimaryCT, typename... CTs>
class Visitor;


// Builds visitor for unique types
template<typename PrimaryCT,  typename... CTs>
Visitor<PrimaryCT, CTs...> BuildVisitor(Ecs& ecs, CTypeId<PrimaryCT> primaryId, CTypeId<CTs>... ids)
{
	// build an array of pointers to storages
	typename Visitor<PrimaryCT, CTs...>::SecondaryArrayType secondaryTypes = {
		&ecs.GetComponentType<CTs>(ids.id)...
		};

	return Visitor<PrimaryCT, CTs...>(ecs.GetComponentType<PrimaryCT>(primaryId.id), secondaryTypes);
}


template<typename PrimaryCT, typename... CTs>
class Visitor
{
public:

	using SecondaryArrayType = std::array<AbstractComponentType*, sizeof...(CTs)>;

	Visitor(ComponentType<PrimaryCT>& pc, const SecondaryArrayType& secs)
		: primaryType_(pc)
		, secondaryTypes_(secs)
	{
	}


	// Visits all entities that have all the required components.
	// Expects fyunctor of singature compatible to:
	//
	// f(EntityId, PrimaryCT&, CTs&...)
	template<typename F>
	void ForEach(F fun)
	{
		primaryType_.ForEach([&](EntityId id, PrimaryCT& component)
		{
//			bool allPresent = true;
//			Meta::ForwardIf(fun, id, allPresent, component, (secondaryTypes_<CTs>().Find(id, allPresent))...);
			VisitSecondaries<F, typename Meta::GenSeq<sizeof...(CTs)>::type>(fun, id, component);
		});
	}


private:

	template<typename F, int... SEQ>
	void VisitSecondaries(F fun, EntityId id, PrimaryCT& component)
	{
		bool allPresent = true;
		Meta::ForwardIf(fun, id, allPresent, component,
			static_cast<ComponentType<CTs>*>(secondaryTypes_[SEQ].Find(id, allPresent))...);
	}

	ComponentType<PrimaryCT>& primaryType_;
	SecondaryArrayType secondaryTypes_;
};



}
