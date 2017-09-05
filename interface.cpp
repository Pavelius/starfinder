#include "command.h"
#include "draw.h"
#include "main.h"

static int				current_command;
const int				tile_size = 32;
const unsigned char		dialog_alpha = 128;
static adat<int, 9>		answers;
static char				state_message[4096];
static point			viewport;

static char* ending(char* p, const char* string)
{
	char* p1 = (char*)zright(p, 1);
	if(p1[0] != '.' && p1[0] != '?' && p1[0] != '!' && p1[0] != ':' && p1[0] != 0 && p1[0] != ' ' && p1[0] != '\t')
		zcat(p1, string);
	return p;
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
	if(test_spaces)
		ending(state_message, " ");
	szprintv(zend(state_message), format, vl);
}

void logs::add(int id, const char* format ...)
{
	ending(state_message, "\n");
	answers.add(id);
	auto p = szprint(zend(state_message), "[%1i)] ", answers.count);
	p = szprintv(zend(p), format, xva_start(format));
	szupper(p, 1);
}

int logs::input()
{
	answers.clear();
	return 0;
}

void logs::open(const char* format)
{
	viewport.x = 800;
	viewport.y = 600;
	static draw::window main_window(-1, -1, viewport.x, viewport.y, WFMinmax | WFResize, 32);
	colors::form = color::create(32, 32, 64);
	colors::text = colors::white;
	colors::special = colors::yellow;
	draw::fore = colors::text;
	draw::sysmouse(false);
}

void draw::window::opening()
{
}

void draw::window::closing()
{
}

void draw::window::resizing(const rect& rc)
{
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