#include "main.h"

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

int character::getbonus(skill_s value) const
{
	auto n = skills[value];
	if(!n)
		return 0;
	if(isclass(value))
		n = n + 3;
	if(theme_skill == value)
	{
		// ��������� � ���� �� ���� �����, � ����������� ��������� �������
		// � �������� + 1 ���� ����
		if(::isclass(value, type))
			n++;
	}
	return n + skills_bonus[value];
}