// (C) 2017 Maciej Gajewski
#include <ecs/ecs.hh>
#include <ecs/visitor.hh>

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

struct CharAspect
{
	char c;
};


class EcsTests : public ::testing::Test
{
public:

protected:

	void RegisterTypes()
	{
		ecs_.registerAutoComponentType<NumAspect>("NumAspect");
		ecs_.registerAutoComponentType<StringAspect>("StringAspect");
		ecs_.registerAutoComponentType<CharAspect>("CharAspect");
	}

	void AddElements()
	{
		stringId_ = ecs_.createEntity("string entity");
		ecs_.addAutoComponentToEntity<StringAspect>(stringId_, {"str1"});

		stringNumId_ = ecs_.createEntity("string/int entity");
		ecs_.addAutoComponentToEntity<StringAspect>(stringNumId_, {"str2"});
		ecs_.addAutoComponentToEntity<NumAspect>(stringNumId_, {2});

		numId_ = ecs_.createEntity("int entity");
		ecs_.addAutoComponentToEntity<NumAspect>(numId_, {3});

		numCharId_ = ecs_.createEntity("int-char entity");
		ecs_.addAutoComponentToEntity<NumAspect>(numCharId_, {4});
		ecs_.addAutoComponentToEntity<CharAspect>(numCharId_, {'a'});
		ecs_.addAutoComponentToEntity<StringAspect>(numCharId_, {"str4"});
	}

	Ecs ecs_;

	ComponentTypeId stringId_ = 666;
	ComponentTypeId numId_ = 666;
	ComponentTypeId stringNumId_ = 666;
	ComponentTypeId numCharId_ = 666;
};

TEST_F(EcsTests, RegisteringTheSameUniqueTypeTwiceFails)
{
	ecs_.registerAutoComponentType<NumAspect>("NumAspect");
	EXPECT_THROW(ecs_.registerAutoComponentType<NumAspect>("NumAspect2"), std::logic_error);
}

TEST_F(EcsTests, VisitationPrimaryOnly)
{
	RegisterTypes();
	AddElements();

	auto visitor = buildAutoTypeVisitor<StringAspect>(ecs_);

	int stringCount = 0;
	visitor.forEach([&](EntityId, StringAspect&)
	{
		stringCount++;
	});

	EXPECT_EQ(3, stringCount);
}

TEST_F(EcsTests, VisitationWithSecondaries)
{
	RegisterTypes();
	AddElements();

	auto visitor = buildAutoTypeVisitor<StringAspect, NumAspect, CharAspect>(ecs_);

	int numsTotal = 0;
	std::string stringsTotal;
	char charTotal = 0;
	visitor.forEach([&](EntityId, const StringAspect& s, const NumAspect& n, const CharAspect& c)
	{
		numsTotal += n.i;
		stringsTotal += s.s;
		charTotal += c.c;
	});

	EXPECT_EQ(4, numsTotal);
	EXPECT_EQ("str4", stringsTotal);
	EXPECT_EQ('a', charTotal);
}

}}

