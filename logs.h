#pragma once

namespace logs
{
	void				add(const char* format, ...);
	void				add(int id, const char* format, ...);
	void				addv(const char* format, const char* param);
	extern const char*	information;
	int					input(bool interactive = true, const char* format = 0, ...);
	int					inputv(bool interactive, const char* format, const char* param, const char* element);
	bool				loadart(const char* url);
	void				next(bool interactive = true);
	void				open(const char* title, bool resize = false);
	bool				yesno(bool interactive = true, const char* format = 0, ...);
	int					whatdo(bool clear_text = true);
	int					whodo(bool interactive, const char* title);
}
