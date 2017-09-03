#include "main.h"

struct theme_knowlendge_i
{
	skill_s				discount;
	skill_s				skill;
	ability_s			ability;
};

struct theme_i
{
	const char*			name[2];
	theme_knowlendge_i	knowledge;
};

static theme_i theme_data[] = {
	{{"Ace Pilot"}, {Culture, Piloting, Dexterity}},
	{{"Bounty Hunter"}, {Culture, Survival, Constitution}},
	{{"Icon"}, {Profession, Culture, Charisma}},
	{{"Mercenary"}, {Culture, Athletics, Strenght}},
	{{"Outlaw"}, {Culture, SleightOfHand, Dexterity}},
	{{"Priest"}, {Culture, Mysticism, Wisdow}},
	{{"Scholar"}, {Culture, PhysicalScience, Intellegence}}, // TODO: Выбирать между Life/Physic
	{{"Spacefarer"}, {PhysicalScience, PhysicalScience, Constitution}},
	{{"Xenoseeker"}, {LifeScience, LifeScience, Charisma}},
};
assert_enum(theme, Xenoseeker);