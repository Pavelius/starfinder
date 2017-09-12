#include "main.h"

static unsigned			start_year;
static unsigned			passed_rounds;
adat<character*, 256>	characters;

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

bool character::move(direction_s d)
{
	auto np = to(position, d);
	auto mc = 5;
	auto pc = map.getcharacter(np);
	if(pc)
	{
		if(action_standart)
		{
			if(isenemy(pc))
				attack(pc);
		}
		return false;
	}
	if(map.ispassable(position, d))
	{
		if(distance < mc)
		{
			// ���� ���������� ���������, ������� ������ �������� �� ��������.
			// ����� ������� ������� ������� ��������.
			if(action_standart)
			{
				action_standart = false;
				distance += getspeed();
			}
		}
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
	if(!target)
		return false;
	if(!isactive() || !target->isactive())
		return false;
	return true;
}

bool character::isvisible(const character* target) const
{
	return true;
}

bool character::is(state_s value) const
{
	return states[value] > passed_rounds;
}

void character::set(state_s value, unsigned rounds)
{
	auto v = passed_rounds + rounds;
	if(v > states[value])
		states[value] = v;
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

int	character::getac(bool kinetic, bool flatfooted) const
{
	auto result = 10;
	auto bonus = getbonus(Dexterity);
	if(bonus < 0 || !flatfooted)
		result += bonus;
	return result;
}

void character::damage(int value)
{
	if(islogged())
		logs::add("%1 �������%2 %3i �����������", getname(), getA(), value);
	auto sp = imin((unsigned char)value, points.stamina);
	points.stamina -= sp;
	value -= sp;
	if(value)
	{
		if(value > points.hit)
		{
			points.hit = 0;
			set(Dying, 1);
		}
		points.hit -= value;
	}
	if(islogged())
		logs::addns(".");
}

const char* character::getname() const
{
	return name;
}

const char* character::getA() const
{
	return (gender == Male) ? "" : "�";
}

void character::attack(character* enemy, damageinfo& di)
{
	distance = 0;
	action_standart = false;
	auto tohit = d20() + di.bonus;
	auto ac = enemy->getac();
	auto critical = (tohit == 20) && (tohit < ac);
	if((!critical && tohit < ac) || tohit == 1)
	{
		if(islogged())
			logs::add("%1 ��������%2.", getname(), getA());
		return;
	}
	if(islogged())
	{
		if(critical)
			logs::add("%1 ���������� �����%2.", getname(), getA());
		else
			logs::add("%1 �����%2.", getname(), getA());
	}
	auto result = di.damage.roll();
	if(result < 0)
		result = 1;
	enemy->damage(result);
}

bool character::islogged() const
{
	return true;
}

bool character::isactive() const
{
	return !is(Dying);
}

void character::attack(character* enemy)
{
	damageinfo di;
	di.bonus = getbonus(Strenght);
	di.damage = dice::create(1, 3, getbonus(Strenght));
	attack(enemy, di);
}

void character::skipturn()
{
	distance = 0;
	action_standart = false;
}