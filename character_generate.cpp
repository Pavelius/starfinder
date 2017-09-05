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

static void choose_ability(bool interactive, unsigned char result[])
{
	unsigned char source[6];
	roll_4d6(source);
	memset(result, 0, 6);
	if(interactive)
	{
		logs::add("\n������������ ����� ");
		for(int i = 0; i < 6; i++)
		{
			if(i != 0)
				logs::add(", ");
			logs::add("%1i", source[i]);
		}
		logs::next();
		for(int n = 0; n < 6; n++)
		{
			logs::add("������� ���� ���������� ��������� %1i?", source[n]);
			for(auto i = Strenght; i <= Charisma; i = (ability_s)(i + 1))
			{
				if(result[i])
					continue;
				logs::add(i, getstr(i));
			}
			auto a = (ability_s)logs::input();
			result[a] = source[n];
		}
	}
	else
		zshuffle(result, 6);
}

static void choose_gender(bool interactive, character& e)
{
	if(interactive)
	{
		logs::add("��� ��?");
		logs::add(Male, getstr(Male));
		logs::add(Female, getstr(Female));
		e.gender = (gender_s)logs::input();
	}
}

static void choose_class(bool interactive, character& e)
{
	if(interactive)
	{
		logs::add("����� ������� ������ ������?");
		for(auto i = Envoy; i <= Technomancer; i = (class_s)(i + 1))
			logs::add(i, getstr(i));
		e.type = (class_s)logs::input();
	}
}

void character::create(bool interactive, race_s race, class_s type, gender_s gender)
{
	choose_gender(interactive, *this);
	choose_class(interactive, *this);
	choose_ability(interactive, native_abilities);
}