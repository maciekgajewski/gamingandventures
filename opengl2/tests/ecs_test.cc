#include <ecs/ecs.hh>

#include <gtest/gtest.h>

#include <iostream>

namespace Ecs { namespace Test {

struct NumAspect
{
	int i;
};

struct StringAspect
{
	std::string s;
};

TEST(EcsTests, RegisteringTheSameUniqueTypeTwiceFails)
{
	Ecs ecs;
	ecs.RegisterUniqueComponentType<NumAspect>("NumAspect");
	EXPECT_THROW(ecs.RegisterUniqueComponentType<NumAspect>("NumAspect2"), std::logic_error);
}

TEST(EcsTests, AddingElements)
{
	Ecs ecs;
	ecs.RegisterUniqueComponentType<NumAspect>("NumAspect");
	ecs.RegisterUniqueComponentType<StringAspect>("StringAspect");

	auto stringId = ecs.CreateEntity("string entity");
	ecs.AddUniqueComponentToEntity<StringAspect>(stringId, {"str1"});


	auto bothId = ecs.CreateEntity("string int entity");
	ecs.AddUniqueComponentToEntity<StringAspect>(bothId, {"str2"});
	ecs.AddUniqueComponentToEntity<NumAspect>(bothId, {2});

}


	// Iterate!
	/* TODO
	int stringCount = 0;
	ecs.ForEach<StringAspect>([&](EntityId, StringAspect&)
	{
		stringCount++;
	});

	EXPECT_EQ(2, stringCount);

	int intCount = 0;
	ecs.ForEach<NumAspect>([&](EntityId, NumAspect&)
	{
		intCount++;
	});

	EXPECT_EQ(1, intCount);

	int bothCount = 0;
	ecs.ForEach<NumAspect, StringAspect>([&](EntityId, NumAspect&, StringAspect&)
	{
		bothCount++;
	});

	EXPECT_EQ(1, bothCount);
	*/


}}

