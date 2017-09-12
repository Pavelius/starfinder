#include "main.h"

static struct race_i
{
	const char*			name[2];
	char				ability[Charisma + 1];
	char				hp;
} race_data[] = {
	{{"Androids"}, {0, 2, 0, 2, 0, -2}, 4},
	{{"Humans"}, {0, 0, 0, 0, 0, 0}, 4}, // TODO: Выбрать любую характеристику
	{{"Kasathas"}, {2, 0, 0, -2, 2, 0}, 4},
	{{"Lashuntas"}, {0, 0, 0, 0, 0, 2}, 4}, // TODO: Dimorphic
	{{"Shirrens"}, {0, 0, 2, 0, 2, -2}, 6},
	{{"Vesk"}, {2, 0, 2, -2, 0, 0}, 6},
	{{"Ysoki"}, {-2, 2, 0, 2, 0, 0}, 2},
};
assert_enum(race, Ysoki);

int	gethitpoints(race_s value)
{
	return race_data[value].hp;
}