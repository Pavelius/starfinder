#include "main.h"

static struct slot_i
{
	const char*			name[2];
} slot_data[] = {
	{{"Melee Weapon", "Оружие ближнего боя"}},
	{{"Range Weapon", "Дистанционное оружие"}},
	{{"Grenade Weapon", "Гранаты"}},
	{{"Body suit", "Нательная броня"}},
};
assert_enum(slot, BodySuit);

template<> const char* getstr<slot_s>(slot_s value)
{
	return slot_data[value].name[1];
}