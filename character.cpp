#include "main.h"

bool character::isclass(skill_s value) const
{
	if(value == theme_skill)
		return true;
	return false;
}

int character::getbonus(skill_s value) const
{
	auto n = skills[value];
	if(!n)
		return 0;
	return isclass(value) ? n + 3 : n;
}