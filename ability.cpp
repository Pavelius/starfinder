#include "main.h"

static struct ability_i
{
	const char*		name[2];
} ability_data[] = {
	{{"Strenght", "����"}},
	{{"Dexterity", "��������"}},
	{{"Constitution", "������������"}},
	{{"Intellegence", "���������"}},
	{{"Wisdow", "��������"}},
	{{"Charisma", "�������"}},
};
assert_enum(ability, Charisma);

template<> const char* getstr<ability_s>(ability_s value)
{
	return ability_data[value].name[1];
}