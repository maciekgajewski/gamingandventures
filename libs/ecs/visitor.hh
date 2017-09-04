// (C) 2017 Maciej Gajewski
#pragma once

#include "ecs.hh"

#include <array>
#include <type_traits>
#include <iostream> // TODO

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


// Builds visitor for non-unique types
template<typename PrimaryCT,  typename... CTs>
Visitor<PrimaryCT, CTs...> BuildVisitor(Ecs& ecs, CTypeId<PrimaryCT> primaryId, CTypeId<CTs>... ids)
{
	// build an array of pointers to storages
	typename Visitor<PrimaryCT, CTs...>::SecondaryTupleType secondary{
		ecs.GetComponentType<CTs>(ids.id)...
		};

	return Visitor<PrimaryCT, CTs...>(ecs.GetComponentType<PrimaryCT>(primaryId.id), secondary);
}

// Builds visitor for unique types
template<typename... CTs>
Visitor<CTs...> BuildUniqueTypeVisitor(Ecs& ecs)
{
	return BuildVisitor(ecs, CTypeId<CTs>(ecs.GetUniqueComponentTypeId<CTs>())...);
}


template<typename PrimaryCT, typename... CTs>
class Visitor
{
public:

	//using SecondaryArrayType = std::array<AbstractComponentType*, sizeof...(CTs)>;
	using SecondaryTupleType = std::tuple<ComponentType<CTs>&...>;

	Visitor(ComponentType<PrimaryCT>& pc, const SecondaryTupleType& secs)
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
			VisitSecondaries<sizeof...(CTs)>(fun, id, component);
		});
	}


private:

	// recursive array visitator
	template<int LEFT_TO_VISIT, typename F>
	std::enable_if_t<LEFT_TO_VISIT == 0>
	VisitSecondaries(F fun, EntityId id, PrimaryCT& primary, CTs&... cts)
	{
		fun(id, primary, cts...);
	}

	template<int LEFT_TO_VISIT, typename F, typename... ResovledCTs>
	std::enable_if_t<(LEFT_TO_VISIT > 0)>
	VisitSecondaries(F fun, EntityId id, PrimaryCT& primary, ResovledCTs&... foundPreviously)
	{
		constexpr int NEXT_TO_FIND = sizeof...(CTs) - LEFT_TO_VISIT;

		auto& compomnentType = std::get<NEXT_TO_FIND>(secondaryTypes_);
		auto* foundNow = compomnentType.Find(id);

		if (!foundNow)
			return;

		VisitSecondaries<LEFT_TO_VISIT-1>(
			fun, id, primary, foundPreviously..., *foundNow);
	}

	ComponentType<PrimaryCT>& primaryType_;
	SecondaryTupleType secondaryTypes_;
};



}
