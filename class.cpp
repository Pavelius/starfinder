#include "main.h"

static struct class_i
{
	const char*			name[2];
} class_data[] = {
	{{"Envoy", "Эмисар"}},
	{{"Mechanic", "Механик"}},
	{{"Mystic", "Мистик"}},
	{{"Operative", "Детектив"}},
	{{"Solarian", "Солариан"}},
	{{"Soldier", "Солдат"}},
	{{"Technomancer", "Техномансер"}},
};
assert_enum(class, Technomancer);

template<> const char* getstr<class_s>(class_s value)
{
	return class_data[value].name[1];
}