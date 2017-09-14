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
		// Проверить а есть ли этот навык, в стандартных классовых навыках
		// и добавить + 1 если есть
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
		// Проверить а есть ли этот навык, в стандартных классовых навыках
		// и добавить + 1 если есть
		if(::isclass(value, type))
			n++;
	}
	return n + skills_bonus[value];
}

bool character::isreach(unsigned short i) const
{
	return getdistance(position, i) <= (getreach() / 5);
}

void attack_of_opportunity(character* enemy, unsigned short from, unsigned short to)
{
	bool printed = !enemy->islogged();
	for(auto e : characters)
	{
		if(!e->action_reaction)
			continue;
		if(!e->isactive())
			continue;
		if(!e->isenemy(enemy))
			continue;
		if(e->isreach(from) && !e->isreach(to))
		{
			if(!printed)
			{
				if(enemy->islogged())
					logs::add("%1 спровацировал%2 атаку.", enemy->getname(), enemy->getA());
				printed = true;
			}
			e->attack(enemy);
			e->action_reaction = false;
		}
	}
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
			// Если продолжаем двигаться, сделаем размен действия на движение.
			// Таким образом сделаем двойное действие.
			if(action_standart)
			{
				action_standart = false;
				distance += getspeed();
			}
		}
		distance -= mc;
		auto op = position;
		position = np;
		if(iscombatmode())
			attack_of_opportunity(this, op, np);
		return true;
	}
	return false;
}

void character::maketurn()
{
	if(is(Dying))
	{
		if(!points.resolve)
		{
			if(islogged())
				logs::add("%1 умер%2.", getname(), getLA());
			set(Dead);
		}
		else
		{
			auto v = imin(1, getmaximumresolve() / 3);
			if(points.resolve >= v)
			{
				if(islogged())
					logs::add("%1 стабилизировал%а состояние.", getname(), getA());
				points.resolve -= v;
				set(Unconscious, Hour);
			}
			else
			{
				if(islogged())
					logs::add("%1 умирает и скоро умрет.", getname());
				points.resolve--;
			}
		}
		return;
	}
	// Обновим действия
	action_standart = true;
	action_swift = true;
	action_reaction = true;
	distance = getspeed();
	// Выполним действия
	if(islogged())
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
	if(!target || target==this)
		return false;
	if(!isactive() || !target->isactive())
		return false;
	return true;
}

bool character::isvisible(const character* target) const
{
	return true;
}

bool character::is(feat_s value) const
{
	return (feats[value/32]&(1<<(value%32)))!=0;
}

void character::set(feat_s value)
{
	feats[value / 32] |= (1 << (value % 32));
}

bool character::is(state_s value) const
{
	switch(value)
	{
	case Unconscious:
		if(is(Dying))
			return true;
		break;
	}
	return states[value] == 0xFFFFFFFF || states[value] > passed_rounds;
}

void character::set(state_s value, unsigned rounds)
{
	if(rounds == Infinite)
	{
		states[value] = rounds;
		return;
	}
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
		logs::add("%1 получил%2 %3i повреждений", getname(), getA(), value);
	auto sp = imin((unsigned char)value, points.stamina);
	points.stamina -= sp;
	value -= sp;
	if(value)
	{
		if(value > points.hit)
		{
			points.hit = 0;
			set(Dying, Infinite);
			logs::add("и упал%1", getA());
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
	return (gender == Male) ? "" : "а";
}

const char* character::getLA() const
{
	return (gender == Male) ? "" : "ла";
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
			logs::add("%1 промазал%2.", getname(), getA());
		return;
	}
	if(islogged())
	{
		if(critical)
			logs::add("%1 критически попал%2.", getname(), getA());
		else
			logs::add("%1 попал%2.", getname(), getA());
	}
	auto result = di.dice.roll();
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
	if(wear[MeleeWeapon])
		di = wear[MeleeWeapon].getdamage();
	else
	{
		di.clear();
		di.dice = dice::create(1, 3, getbonus(Strenght));
	}
	di.bonus += getbonus(Strenght);
	attack(enemy, di);
}

void character::skipturn()
{
	distance = 0;
	action_standart = false;
}