#include "main.h"

struct theme_i
{
	const char*			name[2];
	skill_s				discount;
	skillset			skills;
	ability_s			ability;
};

static theme_i theme_data[] = {
	{{"Ace Pilot", "Ас"}, Culture, {1, {Piloting}}, Dexterity},
	{{"Bounty Hunter", "Охотник за головами"}, Culture, {1, {Survival}}, Constitution},
	{{"Icon", "Картинка"}, Profession, {1, {Culture}}, Charisma},
	{{"Mercenary", "Наемник"}, Culture, {1, {Athletics}}, Strenght},
	{{"Outlaw", "Преступник"}, Culture, {1, {SleightOfHand}}, Dexterity},
	{{"Priest", "Жрец"}, Culture, {1, {Mysticism}}, Wisdow},
	{{"Scholar", "Ученный"}, Culture, {2, {PhysicalScience, LifeScience}}, Intellegence},
	{{"Spacefarer", "Космический перепроходец"}, PhysicalScience, {1, {PhysicalScience}}, Constitution},
	{{"Xenoseeker", "Исследователь пришельцев"}, LifeScience, {1, {LifeScience}}, Charisma},
};
assert_enum(theme, Xenoseeker);

const skillset& getskills(theme_s value)
{
	return theme_data[value].skills;
}

template<> const char* getstr<theme_s>(theme_s value)
{
	return theme_data[value].name[1];
}

ability_s getability(theme_s value)
{
	return theme_data[value].ability;
}