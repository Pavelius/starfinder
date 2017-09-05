#include "main.h"

location map[4];

short unsigned to(short unsigned i, direction_s d)
{
	if(i == 0xFFFF)
		return i;
	switch(d)
	{
	case Left:
		if(getx(i) == 0)
			return 0xFFFF;
		return i - 1;
	case Right:
		if(getx(i) == mapx - 1)
			return 0xFFFF;
		return i + 1;
	case Up:
		if(gety(i) == 0)
			return 0xFFFF;
		return i - mapx;
	case Down:
		if(gety(i) == mapy - 1)
			return 0xFFFF;
		return i + mapx;
	default:
		return i;
	}
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
	case Brush:
		return true;
	default:
		return false;
	}
}

bool location::ispassable(short unsigned i, direction_s d) const
{
	switch(get(i, d))
	{
	case Wall:
		return false;
	default:
		return true;
	}
}