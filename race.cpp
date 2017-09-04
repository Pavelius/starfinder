#include "main.h"

static struct race_i
{
	const char*			name[2];
	char				ability[Charisma + 1];
} race_data[] = {
	{{"Androids"}, {0, 2, 0, 2, 0, -2}},
	{{"Humans"}, {0, 0, 0, 0, 0, 0}}, // TODO: Выбрать любую характеристику
	{{"Kasathas"}, {2, 0, 0, -2, 2, 0}},
	{{"Lashuntas"}, {0, 0, 0, 0, 0, 2}}, // TODO: Dimorphic
	{{"Shirrens"}, {0, 0, 2, 0, 2, -2}},
	{{"Vesk"}, {2, 0, 2, -2, 0, 0}},
	{{"Ysoki"}, {-2, 2, 0, 2, 0, 0}},
};
assert_enum(race, Ysoki);