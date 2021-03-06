#include "crt.h"
#include "draw.h"
#include "stringid.h"

using namespace draw;

stringid*	textf_icons_id;

textwidget::plugin* textwidget::plugin::first;

textwidget::plugin::plugin(textwidget::plugin::element* controls) : controls(controls)
{
	seqlink(this);
}

textwidget::plugin::element* textwidget::plugin::find(const char* id)
{
	for(auto pm = first; pm; pm = pm->next)
	{
		for(auto p = pm->controls; p->id; p++)
		{
			if(strcmp(p->id, id) == 0)
				return p;
		}
	}
	return 0;
}

static const char* glink(char* temp, const char** source)
{
	temp[0] = 0;
	const char* p = *source;
	if(*p != '(')
		return 0;
	p++;
	if(*p == '\"')
	{
		p = psstr(p + 1, temp, *p);
	}
	else
	{
		char* s = temp;
		while(*p && *p != ')')
			*s++ = *p++;
		*s++ = 0;
	}
	if(*p == ')')
		p++;
	*source = p;
	return temp;
}

static const char* word(const char* text)
{
	while(((unsigned char)*text) > 0x20 && *text != '*' && *text != '[' && *text != ']')
		text++;
	return text;
}

static bool match(const char** string, const char* name)
{
	int n = zlen(name);
	if(memcmp(*string, name, n) != 0)
		return false;
	(*string) += n;
	return true;
}

static const char* textspc(const char* p, int x0, int& x, int tab_width)
{
	int tb;
	while(true)
	{
		switch(p[0])
		{
		case ' ':
			p++;
			x += draw::textw(' ');
			continue;
		case '\t':
			p++;
			if(!tab_width)
				tb = draw::textw(' ') * 4;
			else
				tb = tab_width;
			x = x0 + ((x - x0 + tb) / tb)*tb;
			continue;
		}
		break;
	}
	return p;
}

static int textfln(int x0, int y0, int width, const char** string, color c1, int* max_width, int tab_width)
{
	char temp[4096];
	int y = y0;
	int x = x0;
	int x2 = x0 + width;
	const char* p = *string;
	unsigned flags = 0;
	draw::fore = c1;
	if(max_width)
		*max_width = 0;
	temp[0] = 0;
	while(true)
	{
		if(p[0] == '*' && p[1] == '*')
		{
			p += 2;
			if(flags&TextBold)
				flags &= ~TextBold;
			else
				flags |= TextBold;
			continue;
		}
		else if(p[0] == '*')
		{
			p++;
			if(flags&TextItalic)
				flags &= ~TextItalic;
			else
			{
				if((flags&TextItalic) == 0)
					x += draw::texth() / 3;
				flags |= TextItalic;
			}
			continue;
		}
		else if(p[0] == '[' && p[1] == '[')
			p++;
		else if(p[0] == ']' && p[1] == ']')
			p++;
		else if(p[0] == '[')
		{
			p++;
			switch(*p)
			{
			case '~':
				p++;
				draw::fore = colors::text.mix(colors::window, 64);
				break;
			case '+':
				p++;
				draw::fore = colors::green;
				break;
			case '-':
				p++;
				draw::fore = colors::red;
				break;
			case '!':
				p++;
				draw::fore = colors::yellow;
				break;
			case '#':
				p++;
				flags |= TextUscope;
				draw::fore = colors::special;
				break;
			default:
				draw::fore = colors::special;
				break;
			}
			glink(temp, &p);
		}
		else if(p[0] == ']')
		{
			p++;
			draw::fore = c1;
			temp[0] = 0;
			flags &= ~TextUscope;
		}
		// ���������� ������� � ���������
		p = textspc(p, x0, x, tab_width);
		int w;
		if(p[0] == ':' && p[1] >= 'a' && p[1] <= 'z')
		{
			p++;
			char temp[128];
			p = psidn(p, temp, temp + sizeof(temp) - 1);
			if(*p == ':')
				p++;
			w = 0;
			if(metrics::icons)
			{
				int index = 0;
				if(textf_icons_id)
				{
					for(auto p = textf_icons_id; p->id; p++)
					{
						if(strcmp(p->id, temp) == 0)
						{
							index = p->value;
							break;
						}
					}
				}
				auto fr = metrics::icons->get(index);
				w = fr.sx;
				if(x + w > x2)
				{
					if(max_width)
						*max_width = imax(*max_width, x - x0);
					x = x0;
					y += draw::texth();
				}
				draw::image(x + fr.sx / 2, y, metrics::icons, index, 0);
			}
		}
		else
		{
			const char* p2 = word(p);
			w = draw::textw(p, p2 - p);
			if(x + w > x2)
			{
				if(max_width)
					*max_width = imax(*max_width, x - x0);
				x = x0;
				y += draw::texth();
			}
			draw::text(x, y, p, p2 - p, flags);
			p = p2;
		}
		int x4 = x;
		x += w;
		p = textspc(p, x0, x, tab_width);
		if(temp[0] || (flags&TextUscope) != 0)
		{
			int x3 = imin(x2, x);
			int y2 = y + draw::texth();
			if(flags&TextUscope)
				draw::line(x4, y2, x3, y2, draw::fore);
			rect rc = {x4, y, x3, y2};
			if(draw::areb(rc))
			{
				if(flags&TextUscope)
				{
					hot::cursor = CursorHand;
					if(temp[0] && hot::key == MouseLeft && !hot::pressed)
					{
						zcpy(draw::link, temp, sizeof(draw::link) - 1);
						//draw::execute(HtmlLink);
					}
				}
				else
					zcpy(draw::link, temp, sizeof(draw::link) - 1);
			}
		}
		// ������� ������� ������ � ��������� ������
		if(p[0] == 0 || p[0] == 10 || p[0] == 13)
		{
			y += draw::texth();
			p = szskipcr(p);
			break;
		}
	}
	if(max_width)
		*max_width = imax(*max_width, x - x0);
	*string = p;
	return y - y0;
}

static int render_control(const char** result, int x, int y, int width)
{
	textwidget e = {0};
	char type[64];
	char name[64];
	char buffer[4096];
	auto p = *result;
	auto pb = buffer;
	auto pe = buffer + sizeof(buffer) - 1;
	p = psidn(p, type, type + sizeof(type) - 1);
	p = zskipsp(p);
	while(*p && *p != ')')
	{
		p = psidn(p, name, name + sizeof(name) - 1);
		p = zskipsp(p);
		int value_number = 1;
		const char* value_text = 0;
		if(*p == '=')
		{
			p = zskipsp(p + 1);
			if(isnum(*p) || *p == '-')
				value_number = sz2num(p, &p);
			else if(*p == '\"' || *p == '\'')
			{
				value_text = pb;
				p = psstr(p + 1, pb, p[0]);
				pb = zend(pb);
				if(pb < pe)
					pb = pb + 1;
			}
			else
			{
				// Error
				while(*p != ')' && *p)
					p++;
			}
			p = zskipsp(p);
		}
		if(strcmp(name, "id") == 0)
			e.id = value_text;
		else if(strcmp(name, "label") == 0)
			e.label = value_text;
		else if(strcmp(name, "tips") == 0)
			e.tips = value_text;
		else if(strcmp(name, "link") == 0)
			e.link = value_text;
		else if(strcmp(name, "value") == 0)
			e.value = value_number;
		else if(strcmp(name, "maximum") == 0)
			e.maximum = value_number;
	}
	if(*p == ')')
		p++;
	p = szskipcr(p);
	*result = p;
	auto pm = textwidget::plugin::find(type);
	if(pm)
		return pm->render(x, y, width, e);
	return 0;
}

int draw::textf(int x, int y, int width, const char* string, int* max_width,
	int min_height, int* cashe_height, const char** cashe_string, int tab_width)
{
	state push;
	color color_text = fore;
	const char* p = string;
	int y0 = y;
	if(cashe_height)
	{
		*cashe_string = p;
		*cashe_height = 0;
	}
	if(max_width)
		*max_width = 0;
	while(p[0])
	{
		int mw2 = 0;
		if(cashe_height && (y - y0) <= min_height)
		{
			*cashe_string = p;
			*cashe_height = y - y0;
		}
		if(match(&p, "###")) // Header 3
		{
			p = zskipsp(p);
			font = metrics::h3;
			y += textfln(x, y, width, &p, colors::h3, &mw2, tab_width);
		}
		else if(match(&p, "##")) // Header 2
		{
			p = zskipsp(p);
			font = metrics::h2;
			y += textfln(x, y, width, &p, colors::h2, &mw2, tab_width);
		}
		else if(match(&p, "#")) // Header 1
		{
			p = zskipsp(p);
			font = metrics::h1;
			y += textfln(x, y, width, &p, colors::h1, &mw2, tab_width);
		}
		else if(match(&p, "...")) // ��� ��������������
		{
			p = szskipcr(p);
			font = metrics::font;
			color c1 = colors::window.mix(colors::edit, 256 - 32);
			y += texth() / 2;
			while(p[0])
			{
				int c = textbc(p, width);
				if(!c)
					break;
				rectf({x, y, x + width, y + texth()}, c1);
				text(x, y, p, c);
				y += texth();
				p += c;
				if(match(&p, "..."))
				{
					p = szskipcr(p);
					y += texth() / 2;
					break;
				}
			}
		}
		else if(match(&p, "* ")) // ������
		{
			int dx = texth() / 2;
			int rd = texth() / 6;
			circlef(x + dx + 2, y + dx, rd, color_text);
			circle(x + dx + 2, y + dx, rd, color_text);
			y += textfln(x + texth(), y, width - texth(), &p, color_text, &mw2, tab_width);
		}
		else if(p[0] == '$' && p[1] == '(')
		{
			p = zskipsp(p + 2);
			y += render_control(&p, x, y, width);
		}
		else
		{
			y += textfln(x, y, width, &p, color_text, &mw2, tab_width);
			if(p[0])
			{
				if(p[0] == ':' && p[1] == ':' && p[2] == ':')
					p += 3;
				else
					y += metrics::padding;
			}
		}
		// ��������� ����������� ��������� �����
		font = metrics::font;
		fore = color_text;
		if(max_width)
		{
			if(*max_width < mw2)
				*max_width = mw2;
		}
	}
	return y - y0;
}

int draw::textf(rect& rc, const char* string, int tab_width)
{
	state push;
	clipping.clear();
	rc.y2 = rc.y1 + draw::textf(0, 0, rc.width(), string, &rc.x2, 0, 0, 0, tab_width);
	rc.x2 += rc.x1;
	return rc.height();
}