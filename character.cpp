#include "main.h"

adat<character*, 256> characters;

void character::clear()
{
	memset(this, 0, sizeof(character));
}

bool character::isclass(skill_s value) const
{
	if(value == theme_skill)
		return true;
	return ::isclass(value, type);
}

int character::get(ability_s value) const
{
	return abilities[value];
}

int character::getmisc(skill_s value) const
{
	auto n = skills[value];
	if (!n)
		return 0;
	if (isclass(value))
		n = n + 3;
	if (theme_skill == value)
	{
		// Проверить а есть ли этот навык, в стандартных классовых навыках
		// и добавить + 1 если есть
		if (::isclass(value, type))
			n++;
	}
	return n + skills_bonus[value];
}

int character::getbonus(skill_s value) const
{
	auto n = skills[value];
	if(!n)
		return 0;
	if(isclass(value))
		n = n + 3;
	if(theme_skill == value)
	{
		// Проверить а есть ли этот навык, в стандартных классовых навыках
		// и добавить + 1 если есть
		if(::isclass(value, type))
			n++;
	}
	return n + skills_bonus[value];
}

bool character::move(direction_s d)
{
	auto np = to(position, d);
	auto mc = 5;
	auto pc = map.getcharacter(np);
	if(pc)
	{
		return false;
	}
	if(map.ispassable(position, d) && distance >= mc)
	{
		distance -= mc;
		position = np;
		return true;
	}
	return false;
}

void character::maketurn(bool interactive)
{
	action_standart = true;
	action_swift = true;
	distance = getspeed();
	if(interactive)
		logs::move(*this);
	else
	{
		auto enemy = getenemy();
		if(enemy)
		{
		}
		else
		{
		}
	}
}

bool character::isenemy(const character* target) const
{
	return true;
}

bool character::isvisible(const character* target) const
{
	return true;
}

character* character::getenemy() const
{
	for(auto p : characters)
	{
		if(!p->isvisible(this))
			continue;
		if(p->isenemy(this))
			return p;
	}
	return 0;
}

int character::getinitiative() const
{
	return getbonus(Dexterity);
}