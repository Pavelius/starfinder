#include "main.h"

struct theme_i
{
	const char*			name[2];
	skill_s				discount;
	skillset			skills;
	ability_s			ability;
};

static theme_i theme_data[] = {
	{{"Ace Pilot"}, Culture, {1, {Piloting}}, Dexterity},
	{{"Bounty Hunter"}, Culture, {1, {Survival}}, Constitution},
	{{"Icon"}, Profession, {1, {Culture}}, Charisma},
	{{"Mercenary"}, Culture, {1, {Athletics}}, Strenght},
	{{"Outlaw"}, Culture, {1, {SleightOfHand}}, Dexterity},
	{{"Priest"}, Culture, {1, {Mysticism}}, Wisdow},
	{{"Scholar"}, Culture, {2, {PhysicalScience, LifeScience}}, Intellegence},
	{{"Spacefarer"}, PhysicalScience, {1, {PhysicalScience}}, Constitution},
	{{"Xenoseeker"}, LifeScience, {1, {LifeScience}}, Charisma},
};
assert_enum(theme, Xenoseeker);

const skillset& getskills(theme_s value)
{
	return theme_data[value].skills;
}