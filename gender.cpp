#include "main.h"

static struct gender_i
{
	const char*			name[2];
} gender_data[] = {
	{{"Male", "Мужчина"}},
	{{"Female", "Женщина"}},
};
assert_enum(gender, Female);

template<> const char* getstr<gender_s>(gender_s value)
{
	return gender_data[value].name[1];
}