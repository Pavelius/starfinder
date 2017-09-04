#include "main.h"

static struct skill_i
{
	const char*		name[2];
	unsigned char	isclass[Xenoseeker + 1];
	ability_s		ability;
	bool			untrained;
	bool			acmor_check_penalty;
} skill_data[] = {
	{{"Acrobatics"}, {1, 0, 0, 1, 1, 1, 0}, Dexterity, true, true},
	{{"Athletics"}, {1, 1, 0, 1, 1, 1, 0}, Strenght, true, true},
	{{"Bluff"}, {1, 0, 1, 1, 0, 0, 0}, Charisma, true},
	{{"Computers"}, {1, 1, 0, 1, 0, 0, 1}, Intellegence, false},
	{{"Culture"}, {1, 0, 1, 1, 0, 0, 0}, Intellegence, false},
	{{"Diplomacy"}, {1, 0, 1, 0, 1, 0, 0}, Charisma, true},
	{{"Disguise"}, {1, 0, 1, 1, 0, 0, 0}, Charisma, true},
	{{"Engineering"}, {1, 1, 0, 1, 0, 1, 1}, Intellegence, false},
	{{"Intimidate"}, {1, 0, 1, 1, 1, 1, 0}, Charisma, true},
	{{"Life Science"}, {0, 0, 1, 0, 0, 0, 1}, Intellegence, false},
	{{"Medicine"}, {1, 1, 1, 1, 0, 1, 0}, Intellegence, false},
	{{"Mysticism"}, {0, 0, 1, 0, 1, 0, 1}, Wisdow, false},
	{{"Perception"}, {1, 1, 1, 1, 1, 0, 0}, Wisdow, true},
	{{"Physical Science"}, {0, 1, 0, 0, 1, 0, 1}, Intellegence, false},
	{{"Piloting"}, {1, 1, 0, 1, 0, 1, 1}, Dexterity, true},
	{{"Profession"}, {1, 1, 1, 1, 1, 1, 1}, Intellegence, false},
	{{"Sence Motives"}, {1, 0, 1, 1, 1, 0, 0}, Wisdow, true},
	{{"Sleight of Hand"}, {1, 0, 0, 1, 0, 0, 1}, Dexterity, false, true},
	{{"Stealth"}, {1, 0, 0, 1, 1, 0, 0}, Dexterity, true, true},
	{{"Survival"}, {0, 0, 1, 1, 0, 1, 0}, Wisdow, true}
};
assert_enum(skill, Survival);

bool isuntrained(skill_s value)
{
	return skill_data[value].untrained;
}

bool isclass(skill_s value, class_s class_value)
{
	return skill_data[value].untrained;
}

ability_s getability(skill_s value)
{
	return skill_data[value].ability;
}

bool isarmorcheck(skill_s value)
{
	return skill_data[value].acmor_check_penalty;
}

template<> const char* getstr<skill_s>(skill_s value)
{
	return skill_data[value].name[0];
}