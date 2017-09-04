#include "adat.h"
#include "command.h"
#include "crt.h"
#include "draw.h"
#include "logs.h"

struct answer
{
	int						id;
	int						param;
	const char*				text;
};

static adat<answer, 35>		answers;
static char					content[256 * 8];
static char					text_buffer[256 * 32];
static char*				text_ptr = text_buffer;
extern rect					sys_static_area;
extern bool					sys_optimize_mouse_move;

enum answer_tokens {
	FirstAnswer = InputUser,
	LastAnswer = FirstAnswer + sizeof(answers.data) / sizeof(answers.data[0])
};

static void answer_clear()
{
	text_ptr = text_buffer;
	answers.clear();
}

static char* ending(char* p, const char* string)
{
	char* p1 = (char*)zright(p, 1);
	if(p1[0] != '.' && p1[0] != '?' && p1[0] != '!' && p1[0] != ':' && p1[0] != 0)
		zcat(p1, string);
	return p;
}

void logs::add(int id, const char* format ...)
{
	auto e = answers.add();
	if(!e)
		return;
	memset(e, 0, sizeof(answers.data[0]));
	e->id = id;
	szprintv(text_ptr, format, xva_start(format));
	szupper(text_ptr, 1);
	e->text = ending(text_ptr, ".");
	text_ptr = zend(text_ptr) + 1;
}

void logs::addv(const char* format, const char* param)
{
	if(!format)
	{
		memset(content, 0, sizeof(content));
		return;
	}
	char* p = zend(content);
	if(p == content)
		format = zskipspcr(format);
	// First string may not be emphty or white spaced
	if(format[0] == 0 && p == content)
		return;
	if(p != content)
	{
		if(p[-1] != ' ' && p[-1] != '\n' && *format != '\n' && *format != '.' && *format != ',')
		{
			*p++ = ' ';
			*p = 0;
		}
		if(p[-1] == '\n' && format[0] == '\n')
			format++;
	}
	szprintv(p, format, param);
}

void logs::add(const char* format, ...)
{
	addv(format, xva_start(format));
}

void logs::open(const char* title, bool resize)
{
	command_app_initialize->execute();
	sys_optimize_mouse_move = true;
	static draw::window window(-1, -1, 760, 640, resize ? WFResize | WFMinmax : 0);
	set_light_theme();
	draw::font = metrics::font;
	draw::fore = colors::text;
	draw::setcaption(title);
	answer_clear();
}

static char* letter(char* result, int n)
{
	if(n < 9)
		return szprint(result, "%1i)", n + 1);
	result[0] = 'A' + (n - 9);
	result[1] = ')';
	result[2] = 0;
	return result;
}

int wdt_answer(int x, int y, int width, draw::textwidget& e)
{
	int id = e.value;
	char result[32];
	int y0 = y;
	int x2 = x + width;
	//x += metrics::padding;
	int i = id - FirstAnswer;
	letter(result, i);
	draw::text(x, y, result);
	int w1 = draw::textw(result) + metrics::padding;
	int x1 = x + w1;
	rect rc = {x1, y, x2, y};
	int dy = draw::textf(rc, answers.data[i].text);
	areas a = draw::area(rc);
	if(a == AreaHilited || a == AreaHilitedPressed)
	{
		if(a == AreaHilitedPressed)
		{
			hot::pressed = false;
			draw::execute(i + FirstAnswer);
		}
		draw::rectf({rc.x1 - 2, rc.y1 - 2, rc.x2 + 2, rc.y2 + 2}, colors::edit, 16);
		draw::rectb({rc.x1 - 2, rc.y1 - 2, rc.x2 + 2, rc.y2 + 2}, colors::border.mix(colors::window, 128));
	}
	draw::textf(x1, y, x2 - x1, answers.data[i].text);
	y += dy + metrics::padding;
	return y - y0;
}

int wdt_answers(int x, int y, int width, draw::textwidget& e)
{
	auto y0 = y;
	auto last = FirstAnswer + answers.getcount();
	for(int i = FirstAnswer; i<last; i++)
	{
		draw::textwidget w = {0};
		w.value = i;
		y += wdt_answer(x, y, width, w);
	}
	return y - y0;
}

static int render_input()
{
	const int left_panel = 300;
	while(true)
	{
		int x = 0;
		int y = 0;
		int y2 = draw::getheight();
		int x2 = draw::getwidth();
		draw::rectf({x, y, x2, y2}, colors::window);
		x += metrics::padding;
		y += metrics::padding;
		x2 -= metrics::padding;
		y2 -= metrics::padding;
		if(true)
		{
			int y1 = metrics::padding;
			int x1 = x2 - left_panel;
			y1 += draw::textf(x1, y1, left_panel, content);
			x2 = x1 - metrics::padding;
		}
		int id = draw::input();
		if(!id)
			exit(0);
		else if(id >= FirstAnswer && id <= LastAnswer)
			return answers.data[id - FirstAnswer].id;
		else if(hot::key >= (Alpha + '1')
			&& hot::key <= (Alpha + '1' + (int)answers.count - 1))
			draw::execute(FirstAnswer + hot::key - Alpha - '1');
		else if(hot::key >= ((int)Alpha + 'A') && hot::key <= ((int)Alpha + 'Z'))
		{
			if(hot::key <= ((Alpha + 'A') + ((int)answers.count - 10)))
				draw::execute(FirstAnswer + 9 + hot::key - (Alpha + 'A'));
		}
		else if(id < FirstInput)
			return id; // События от прочих элементов упавления
	}
}

static void correct(char* p)
{
	bool need_uppercase = true;
	for(; *p; p++)
	{
		if(*p == '.' || *p == '?' || *p == '!')
		{
			p = (char*)zskipsp(p + 1);
			if(*p != '-')
				need_uppercase = true;
		}
		if(*p == ' ' || *p == '-' || *p == '\t')
			continue;
		if(need_uppercase)
		{
			szupper(p, 1);
			need_uppercase = false;
		}
	}
}

int logs::inputv(bool interactive, const char* format, const char* param, const char* element)
{
	char* p = zend(content);
	while(p > content && p[-1] == '\n')
		*--p = 0;
	if(format && format[0])
	{
		if(p != content)
			zcat(p, "\n");
		zcat(p, "[");
		logs::addv(format, param);
		zcat(p, "]");
	}
	correct(content);
	if(element)
		zcat(content, element);
	int r = 0;
	if(interactive)
		r = render_input();
	else if(answers.count)
		r = answers.data[rand() % (answers.count)].id;
	p[0] = 0;
	answer_clear();
	return r;
}

int logs::input(bool inveractive, const char* format, ...)
{
	return inputv(inveractive, format, xva_start(format), "\n$(answers)");
}

bool logs::loadart(const char* url)
{
	return true;
}

void logs::next(bool interactive)
{
	logs::add(1, "Далее");
	logs::input(interactive);
}

bool logs::yesno(bool interactive, const char* format, ...)
{
	add(1, "Да");
	add(2, "Нет");
	return inputv(interactive, format, xva_start(format), "\n$(answers)") == 1;
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

static draw::textwidget::plugin::element support_controls[] = {
	{"answers", wdt_answers},
	{0}
};
static draw::textwidget::plugin widget_plugin_instance(support_controls);