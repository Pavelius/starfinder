#include "main.h"

static struct class_i
{
	const char*		name[2];
	char			hp, sp;
	ability_s		ability;
	char			skills;
} class_data[] = {
	{{"Envoy", "������"}, 6, 6, Charisma, 8},
	{{"Mechanic", "�������"}, 6, 6, Intellegence, 4},
	{{"Mystic", "������"}, 6, 6, Wisdow, 6},
	{{"Operative", "��������"}, 6, 6, Dexterity, 8},
	{{"Solarian", "��������"}, 7, 7, Charisma, 4},
	{{"Soldier", "������"}, 7, 7, Strenght, 4},
	{{"Technomancer", "�����������"}, 5, 5, Intellegence, 4},
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