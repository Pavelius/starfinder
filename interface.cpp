#include "command.h"
#include "draw.h"
#include "main.h"

static int				current_command;
const int				tile_size = 32;
const unsigned char		dialog_alpha = 128;
static adat<int, 9>		answers;
static char				state_message[4096];
static point			viewport;

namespace colors
{
	color				door = color::create(216, 100, 75);
}

void logs::clear()
{
	answers.clear();
	state_message[0] = 0;
}

static void view_dialog(rect rc)
{
	rc.offset(-metrics::padding);
	draw::rectf(rc, colors::black, dialog_alpha);
	draw::rectb(rc, colors::border);
}

static int view_dialog(rect rc, const char* title)
{
	draw::state push;
	auto x = rc.x1;
	auto y = rc.y1;
	auto w = rc.width();
	rc.offset(-metrics::padding);
	view_dialog(rc);
	draw::font = metrics::h2;
	draw::fore = colors::yellow;
	draw::text(x + (w - draw::textw(title)) / 2, y, title);
	return draw::texth() + metrics::padding;
}

static void view_message()
{
	if(!state_message[0])
		return;
	rect rc = {0, 0, 400, 800};
	draw::textf(rc, state_message);
	auto width = rc.width();
	auto height = rc.height();
	rc.x1 = (draw::getwidth() - width) / 2;
	rc.y1 = metrics::padding * 2;
	rc.x2 = rc.x1 + width;
	rc.y2 = rc.y1 + height;
	view_dialog(rc);
	draw::textf(rc.x1, rc.y1, width, state_message);
}

static void view_wall(int x, int y, bool vertical)
{
	if(vertical)
		draw::rectf({x + tile_size - 1, y, x + tile_size + 2, y + tile_size + 2}, colors::red);
	else
		draw::rectf({x, y + tile_size - 1, x + tile_size + 2, y + tile_size + 2}, colors::red);
}

static void view_door(int x, int y, bool vertical, bool open)
{
	const int w = 4;
	if(vertical)
	{
		draw::rectf({x + tile_size - 1, y, x + tile_size + 2, y + w + 2}, colors::red);
		draw::rectf({x + tile_size - 1, y + tile_size - w - 2, x + tile_size + 2, y + tile_size + 2}, colors::red);
		if(!open)
			draw::rectf({x + tile_size - 2, y + w + 2, x + tile_size + 3, y + tile_size - 2 - w}, colors::door);
	}
	else
	{
		draw::rectf({x, y + tile_size - 1, x + w + 2, y + tile_size + 2}, colors::red);
		draw::rectf({x + tile_size - w - 2, y + tile_size - 1, x + tile_size + 2, y + tile_size + 2}, colors::red);
		if(!open)
			draw::rectf({x + w + 2, y + tile_size - 2, x + tile_size - w - 2, y + tile_size + 3}, colors::door);
	}
}

static void view_character(int x, int y, char letter, bool current)
{
	draw::state push;
	if(current)
		draw::fore = colors::white;
	else
		draw::fore = colors::border;
	draw::circle(x + tile_size/2, y + tile_size / 2, 14);
}

static void view_floor(rect rc, point camera, location& e, character* current)
{
	draw::state push;
	auto x0 = rc.x1 - camera.x;
	auto y0 = rc.y1 - camera.y;
	auto mx2 = (camera.x + viewport.x) / tile_size + 1;
	auto my2 = (camera.y + viewport.y) / tile_size + 1;
	// Floor
	for(auto my = camera.y / tile_size; my < my2; my++)
	{
		for(auto mx = camera.x / tile_size; mx < mx2; mx++)
		{
			auto x = x0 + mx*tile_size;
			auto y = y0 + my*tile_size;
			auto i = gi(mx, my);
			switch(e.get(i))
			{
			case Block:
				draw::rectf({x, y, x + tile_size, y + tile_size}, colors::black);
				break;
			}
		}
	}
	// Walls
	for(auto my = camera.y / tile_size; my < my2; my++)
	{
		for(auto mx = camera.x / tile_size; mx < mx2; mx++)
		{
			auto x = x0 + mx*tile_size;
			auto y = y0 + my*tile_size;
			auto i = gi(mx, my);
			switch(e.get(i, Right))
			{
			case Wall:
				view_wall(x, y, true);
				break;
			case Door:
				view_door(x, y, true, false);
				break;
			}
			switch(e.get(i, Down))
			{
			case Wall:
				view_wall(x, y, false);
				break;
			case Door:
				view_door(x, y, false, false);
				break;
			}
		}
	}
	for(auto pc : characters)
	{
		auto i = pc->position;
		auto x = x0 + gx(i)*tile_size;
		auto y = y0 + gy(i)*tile_size;
		view_character(x, y, 'C', current==pc);
	}
}

static void view(int x, int y, const char* t1, int value)
{
	const int width = 32;
	char temp[260];
	szprint(temp, "%1:", t1);
	draw::text(x, y, temp);
	sznum(temp, value);
	draw::text(x + width, y, temp);
}

static void view(int x, int y, const char* t1, int v1, int v2)
{
	const int width = 64;
	char temp[260];
	szprint(temp, "%1:", t1);
	draw::text(x, y, temp);
	szprint(temp, "%1i/%2i", v1, v2);
	draw::text(x + width, y, temp);
}

static int view_action(int x, int y, const char* text, ...)
{
	char temp[2048]; szprintv(temp, text, xva_start(text));
	rect rc = {x, y, x + 440, y};
	draw::textf(rc, temp);
	auto dy = rc.height() + metrics::padding*3;
	rc.y1 -= dy; rc.y2 -= dy;
	view_dialog(rc);
	draw::textf(rc.x1, rc.y1, rc.width(), temp);
	return rc.y1;
}

static void view_actions(character* pc)
{
	if(!pc)
		return;
	const int width = 440;
	int height = 3 * draw::texth();
	int x = (draw::getwidth() - width) / 2;
	int y = draw::getheight() - height - metrics::padding * 2;
	if(pc->distance>0)
		y = view_action(x, y, "Движение: %1i футов", pc->distance);
	if(pc->action_standart && pc->action_swift)
		y = view_action(x, y, "Стандартное и быстрое действия");
	else if(pc->action_standart)
		y = view_action(x, y, "Стандартное действие");
	else if(pc->action_swift)
		y = view_action(x, y, "Быстрое действие");
}

static void view_info(character* pc)
{
	if(!pc)
		return;
	const int width = 440;
	int height = 3 * draw::texth();
	int x = (draw::getwidth() - width) / 2;
	int y1 = draw::getheight() - height - metrics::padding*2;
	int y2 = y1 + draw::texth();
	int y = y1;
	view_dialog({x, y1, x + width, y1 + height});
	draw::text(x, y1, pc->getname());
	draw::text(x, y2, getstr(pc->type));
	x += 128;
	view(x, y1, "Сил", pc->get(Strenght));
	view(x, y2, "Лов", pc->get(Dexterity));
	x += 64;
	view(x, y1, "Тел", pc->get(Constitution));
	view(x, y2, "Инт", pc->get(Intellegence));
	x += 64;
	view(x, y1, "Муд", pc->get(Wisdow));
	view(x, y2, "Хар", pc->get(Charisma));
	x += 64;
	view(x, y1, "Энергия", pc->points.stamina, pc->getmaximumstamina());
	view(x, y2, "Жизнь", pc->points.hit, pc->getmaximumhits());
}

static void view_zone(character* p)
{
	rect rc = {0, 0, draw::getwidth(), draw::getheight()};
	draw::rectf(rc, colors::gray);
	view_floor(rc, {0, 0}, map, p);
	view_message();
	view_info(p);
	view_actions(p);
}

void logs::add(const char* format, ...)
{
	addv(format, xva_start(format), true);
}

void logs::addns(const char* format, ...)
{
	addv(format, xva_start(format), false);
}

void logs::addv(const char* format, const char* vl, bool test_spaces)
{
	auto p = zend(state_message);
	if(p != state_message && test_spaces)
	{
		auto p1 = p - 1;
		if(p1[0] != '\n' && p1[0] != '\r' && p1[0] != ' ' && p1[0] != '\t')
			zcat(p, " ");
	}
	szprintv(zend(p), zskipspcr(format), vl);
}

void logs::add(int id, const char* format ...)
{
	auto p = zend(state_message);
	if(p != state_message && p[-1] != '\n')
		zcat(p, "\n");
	answers.add(id);
	p = szprint(zend(p), "[%1i)] ", answers.count);
	p = szprintv(zend(p), format, xva_start(format));
	szupper(p, 1);
}

int logs::input()
{
	while(true)
	{
		//point camera = getcamera(game::getx(e.position), game::gety(e.position));
		view_zone(0);
		auto id = draw::input();
		if(id >= Alpha + '1' && id <= Alpha + '9')
		{
			auto i = id - (Alpha + '1');
			auto result = answers.data[i];
			clear();
			return result;
		}
	}
}

void logs::next()
{
	add("...[Далее]");
	while(true)
	{
		view_zone(0);
		auto id = draw::input();
		if(id == KeySpace || id == KeyEscape)
		{
			clear();
			return;
		}
	}
}

void logs::open(const char* format)
{
	viewport.x = 800;
	viewport.y = 600;
	static draw::window main_window(-1, -1, viewport.x, viewport.y, WFMinmax | WFResize, 32);
	colors::form = color::create(32, 32, 64);
	colors::text = colors::white;
	colors::window = colors::white;
	colors::special = colors::yellow;
	draw::fore = colors::text;
	draw::sysmouse(false);
}

void draw::execute(int id, int param)
{
	hot::key = 0;
	current_command = id;
	hot::param = param;
}

int draw::input(bool redraw)
{
	auto temp_hotkey = hot::key;
	auto temp_command = current_command;
	// Очистим данные
	current_command = 0;
	hot::key = 0;
	// Если была команда, надо ее выполнить
	if(temp_command)
	{
		hot::key = temp_command;
		return hot::key;
	}
	if(hot::key)
		return hot::key;
	int id = InputUpdate;
	if(redraw)
		draw::sysredraw();
	else
		id = draw::rawinput();
	if(!id)
		exit(0);
	hot::cursor = CursorArrow;
	return id;
}

void draw::window::opening()
{
}

void draw::window::closing()
{
}

void draw::window::resizing(const rect& rc)
{
	if(draw::canvas)
	{
		draw::canvas->resize(rc.x2, rc.y2, draw::canvas->bpp, true);
		draw::clipping.set(0, 0, rc.x2, rc.y2);
	}
}

point getcamera(short unsigned index)
{
	return {0};
}

static direction_s getdirection(int id)
{
	switch(id)
	{
	case KeyLeft: return Left;
	case KeyRight: return Right;
	case KeyDown: return Down;
	case KeyUp: return Up;
	default: return Center;
	}
}

static void skip_move(character& e, int id)
{
	auto dr = getdirection(id);
}

static struct action_i
{
	const char*		name;
	unsigned		key;
	void			(character::*proc)();
	operator bool() const { return name != 0; }
} main_actions[] = {
	{"Пропустить ход", KeySpace, &character::skipturn},
};

static action_i* findaction(action_i* actions, unsigned key)
{
	for(auto p = actions; *p; p++)
	{
		if(p->key == key)
			return p;
	}
	return 0;
}

void logs::move(character& e)
{
	while(e.distance || e.action_standart)
	{
		getcamera(e.position);
		view_zone(&e);
		auto id = draw::input();
		auto dr = getdirection(id);
		if(dr != Center)
			e.move(dr);
		else
		{
			auto pa = findaction(main_actions, id);
			if(pa)
				(e.*pa->proc)();
		}
	}
}