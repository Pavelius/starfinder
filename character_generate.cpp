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

static void choose_ability(unsigned char result[])
{
	unsigned char source[6];
	roll_4d6(source);
	memset(result, 0, 6);
	//if(interactive)
	//{
	//	logs::add("\n������������ ����� ");
	//	for(int i = 0; i < 6; i++)
	//	{
	//		if(i != 0)
	//			logs::add(", ");
	//		logs::add("%1i", source[i]);
	//	}
	//	logs::next();
	//	for(int n = 0; n < 6; n++)
	//	{
	//		logs::add("������� ���� ���������� ��������� %1i?", source[n]);
	//		for(auto i = Strenght; i <= Charisma; i = (ability_s)(i + 1))
	//		{
	//			if(result[i])
	//				continue;
	//			logs::add(i, getstr(i));
	//		}
	//		auto a = (ability_s)logs::input();
	//		result[a] = source[n];
	//	}
	//}
	//else
	//{
	memcpy(result, source, sizeof(source));
	zshuffle(result, 6);
	//}
}

gender_s choose_gender()
{
	logs::add("��� ��?");
	logs::add(Male, getstr(Male));
	logs::add(Female, getstr(Female));
	return (gender_s)logs::input();
}

class_s choose_class()
{
	logs::add("����� ������� ������ ������?");
	for(auto i = Envoy; i <= Technomancer; i = (class_s)(i + 1))
		logs::add(i, getstr(i));
	return (class_s)logs::input();
}

theme_s choose_theme()
{
	logs::add("��� ��?");
	for(auto i = AcePilot; i <= Xenoseeker; i = (theme_s)(i + 1))
		logs::add(i, getstr(i));
	return (theme_s)logs::input();
}

static void apply_ability_modifiers(character& e)
{
}

void character::create(race_s race, class_s type, gender_s gender)
{
	clear();
	this->gender = gender;
	this->race = race;
	this->type = type;
	choose_ability(abilities);
	// �������� �� ������� ������� �� ����
	abilities[getability(theme)]++;
	// �������� ��������
	memcpy(native_abilities, abilities, sizeof(native_abilities));
}