#include "main.h"

location		map;
static bool		combat_mode;

short unsigned to(short unsigned i, direction_s d)
{
	if(i == 0xFFFF)
		return i;
	switch(d)
	{
	case Left:
		if(gx(i) == 0)
			return 0xFFFF;
		return i - 1;
	case Right:
		if(gx(i) == mapx - 1)
			return 0xFFFF;
		return i + 1;
	case Up:
		if(gy(i) == 0)
			return 0xFFFF;
		return i - mapx;
	case Down:
		if(gy(i) == mapy - 1)
			return 0xFFFF;
		return i + mapx;
	default:
		return i;
	}
}

bool iscombatmode()
{
	return combat_mode;
}

location::location()
{
	clear();
}

void location::clear()
{
	memset(this, 0, sizeof(location));
}

void location::set(short unsigned i, landscape_s value)
{
	if(i != 0xFFFF)
		floor[i] = value;
}

void location::set(landscape_s value)
{
	for(short unsigned i = 0; i < mapx*mapy; i++)
		set(i, value);
}

short unsigned location::set(short unsigned i, landscape_s value, direction_s d, int count)
{
	while(count > 0)
	{
		set(i, value);
		i = to(i, d);
		count--;
	}
	return i;
}

wall_s location::get(short unsigned i, direction_s d) const
{
	switch(d)
	{
	case Left:
		i = to(i, Left);
	case Right:
		if(i == 0xFFFF)
			return NoWall;
		return wallh[i];
	case Up:
		i = to(i, Up);
	case Down:
		if(i == 0xFFFF)
			return NoWall;
		return wallv[i];
	default:
		return NoWall;
	}
}

void location::set(short unsigned i, wall_s value, direction_s d)
{
	switch(d)
	{
	case Left:
		i = to(i, Left);
	case Right:
		if(i == 0xFFFF)
			break;
		wallh[i] = value;
		break;
	case Up:
		i = to(i, Up);
	case Down:
		if(i == 0xFFFF)
			break;
		wallv[i] = value;
		break;
	}
}

short unsigned location::set(short unsigned i, wall_s value, direction_s d, direction_s md, int count)
{
	while(count > 0)
	{
		set(i, value, d);
		i = to(i, md);
		count--;
	}
	return i;
}

bool location::ispassable(short unsigned i) const
{
	switch(get(i))
	{
	case NoLandscape:
	case Brush: return true;
	default: return false;
	}
}

bool location::ispassable(short unsigned i, direction_s d) const
{
	switch(get(i, d))
	{
	case Wall: return false;
	default: return true;
	}
}

static void test_wall(location& e, short unsigned i, direction_s d)
{
	if(e.get(to(i, d)) == Block)
		return;
	if(e.get(i, d) == NoWall)
		e.set(i, Wall, d);
}

void location::normalize()
{
	for(short unsigned i = 0; i < mapx*mapy; i++)
	{
		if(get(i) != Block)
			continue;
		test_wall(*this, i, Up);
		test_wall(*this, i, Down);
		test_wall(*this, i, Left);
		test_wall(*this, i, Right);
	}
}

character* location::getcharacter(short unsigned index) const
{
	for(auto p : characters)
	{
		if(p->position == index)
			return p;
	}
	return 0;
}

int compare_characters(const void* e1, const void* e2)
{
	auto p1 = *((character**)e1);
	auto p2 = *((character**)e2);
	return p1->initiative - p2->initiative;
}

void location::rollinitiative()
{
	for(auto p : characters)
		p->initiative = d20() + p->getinitiative();
	qsort(characters.data, characters.count, sizeof(characters.data[0]), compare_characters);
}

bool location::iscombat() const
{
	for(unsigned i = 0; i < characters.count; i++)
	{
		auto p1 = characters.data[i];
		for(unsigned j = i + 1; j < characters.count; j++)
		{
			if(p1->isenemy(characters.data[j]))
				return true;
		}
	}
	return false;
}

void location::combat()
{
	if(combat_mode)
		return;
	combat_mode = true;
	while(iscombat())
	{
		for(auto e : characters)
		{
			if(e->isactive())
				e->maketurn(true);
		}
		logs::next();
		logs::clear();
	}
	combat_mode = false;
}