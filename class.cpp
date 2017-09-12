#include "main.h"

static struct class_i
{
	const char*		name[2];
	char			hp, sp;
	ability_s		ability;
	char			skills;
} class_data[] = {
	{{"Envoy", "Эмисар"}, 6, 6, Charisma, 8},
	{{"Mechanic", "Механик"}, 6, 6, Intellegence, 4},
	{{"Mystic", "Мистик"}, 6, 6, Wisdow, 6},
	{{"Operative", "Детектив"}, 6, 6, Dexterity, 8},
	{{"Solarian", "Солариан"}, 7, 7, Charisma, 4},
	{{"Soldier", "Солдат"}, 7, 7, Strenght, 4},
	{{"Technomancer", "Техномансер"}, 5, 5, Intellegence, 4},
};
assert_enum(class, Technomancer);

template<> const char* getstr<class_s>(class_s value)
{
	return class_data[value].name[1];
}

ability_s getability(class_s value)
{
	return class_data[value].ability;
}

int	getstaminapoints(class_s value)
{
	return class_data[value].sp;
}

int	gethitpoints(class_s value)
{
	return class_data[value].hp;
}

int	getskillpoints(class_s value)
{
	return class_data[value].skills;
}