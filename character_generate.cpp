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
	//	logs::add("\nРаспределите числа ");
	//	for(int i = 0; i < 6; i++)
	//	{
	//		if(i != 0)
	//			logs::add(", ");
	//		logs::add("%1i", source[i]);
	//	}
	//	logs::next();
	//	for(int n = 0; n < 6; n++)
	//	{
	//		logs::add("Укажите куда необходимо поместить %1i?", source[n]);
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
	logs::add("Кто вы?");
	logs::add(Male, getstr(Male));
	logs::add(Female, getstr(Female));
	return (gender_s)logs::input();
}

class_s choose_class()
{
	logs::add("Каким классом хотите играть?");
	for(auto i = Envoy; i <= Technomancer; i = (class_s)(i + 1))
		logs::add(i, getstr(i));
	return (class_s)logs::input();
}

theme_s choose_theme()
{
	logs::add("Кто вы?");
	for(auto i = AcePilot; i <= Xenoseeker; i = (theme_s)(i + 1))
		logs::add(i, getstr(i));
	return (theme_s)logs::input();
}

static void apply_ability_modifiers(character& e)
{
}

void character::levelup(bool interactive)
{
	if(level == 0)
	{
		points.hit = getmaximumhits();
		points.stamina = getmaximumstamina();
	}
}

int character::getmaximumhits() const
{
	return gethitpoints(type) + gethitpoints(race);
}

int character::getmaximumstamina() const
{
	auto result = gethitpoints(type) + getbonus(Constitution);
	if(is(Toughness))
		result += getlevel();
	return result;
}

int	character::getmaximumresolve() const
{
	auto result = imin(1, getbonus(getability(type)) + imin(1, getlevel()/2));
	if(is(ExtraResolve))
		result += 2;
	return result;
}

int character::getlevel() const
{
	return level;
}

void character::create(race_s race, class_s type, gender_s gender)
{
	clear();
	this->gender = gender;
	this->race = race;
	this->type = type;
	choose_ability(abilities);
	// Увеличим на еденицу атрибут из темы
	abilities[getability(theme)]++;
	// Сохраним атрибуты
	memcpy(native_abilities, abilities, sizeof(native_abilities));
	levelup(false);
}