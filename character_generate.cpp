#include "main.h"

static int compare_lowest(const void* p1, const void* p2)
{
	return *((char*)p2) - *((char*)p1);
}

static int compare_greatest(const void* p1, const void* p2)
{
	return *((char*)p1) - *((char*)p2);
}

static void roll_4d6(unsigned char result[])
{
	for(int i = 0; i < 6; i++)
	{
		char temp[4];
		temp[0] = rand() % 6 + 1;
		temp[1] = rand() % 6 + 1;
		temp[2] = rand() % 6 + 1;
		temp[3] = rand() % 6 + 1;
		qsort(temp, sizeof(temp), sizeof(temp[0]), compare_lowest);
		result[i] = temp[0] + temp[1] + temp[2];
	}
	qsort(result, 6, 1, compare_lowest);
}

static void generate_ability(bool interactive, unsigned char result[])
{
	unsigned char source[6];
	roll_4d6(source);
	memset(result, 0, 6);
	if(interactive)
	{
		logs::add("������������ ����� ");
		for(int i = 0; i < 6; i++)
		{
			if(i != 0)
				logs::add(", ");
			logs::add("%1i", source[i]);
		}
		logs::add(".");
		for(int n = 0; n < 6; n++)
		{
			for(auto i = Strenght; i <= Charisma; i = (ability_s)(i + 1))
			{
				if(result[i])
					continue;
				logs::add(i, getstr(i));
			}
			auto a = (ability_s)logs::input(true, "������� ���� ���������� ��������� %1i?", source[n]);
			result[a] = source[n];
			logs::add("\n%1 %2i", getstr(a), result[a]);
		}
	}
	else
		zshuffle(result, 6);
}

void character::create(bool interactive, race_s race, class_s type, gender_s gender)
{
	generate_ability(interactive, native_abilities);
}