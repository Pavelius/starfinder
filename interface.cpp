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

static void clear_text()
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

static void view_character(int x, int y, char letter)
{
	draw::circle(x + tile_size/2, y + tile_size / 2, 14);
}

static void view_floor(rect rc, point camera, location& e)
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
		view_character(x, y, 'C');
	}
}

static void view_zone()
{
	rect rc = {0, 0, draw::getwidth(), draw::getheight()};
	draw::rectf(rc, colors::gray);
	view_floor(rc, {0, 0}, map);
	view_message();
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
		if(p1[0] != '.' && p1[0] != '?' && p1[0] != '!' && p1[0] != ':' && p1[0] != 0 && p1[0] != ' ' && p1[0] != '\t')
			zcat(p, " ");
	}
	format = zskipspcr(format);
	szprintv(zend(state_message), format, vl);
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
		view_zone();
		auto id = draw::input();
		if(id >= Alpha + '1' && id <= Alpha + '9')
		{
			auto i = id - (Alpha + '1');
			auto result = answers.data[i];
			clear_text();
			return result;
		}
	}
}

void logs::next()
{
	add("...[Далее]");
	while(true)
	{
		view_zone();
		auto id = draw::input();
		if(id == KeySpace || id == KeyEscape)
		{
			clear_text();
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

static void move_combat(character& e, int id)
{
	auto dr = getdirection(id);
}

struct action_i
{
	const char*		name;
	unsigned		key;
	void			(*proc)(character& e, int id);
	operator bool() const { return name != 0; }
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
	while(e.distance || e.action_standart || e.action_swift)
	{
		getcamera(e.position);
		view_zone();
		auto id = draw::input();
		auto dr = getdirection(id);
		if(dr != Center)
			e.move(dr);
		else
		{
		}
	}
}