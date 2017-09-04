#include "main.h"

static struct class_i
{
	const char*			name[2];
} class_data[] = {
	{{"Envoy", "������"}},
	{{"Mechanic", "�������"}},
	{{"Mystic", "������"}},
	{{"Operative", "��������"}},
	{{"Solarian", "��������"}},
	{{"Soldier", "������"}},
	{{"Technomancer", "�����������"}},
};
assert_enum(class, Technomancer);

template<> const char* getstr<class_s>(class_s value)
{
	return class_data[value].name[1];
}