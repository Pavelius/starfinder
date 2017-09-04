#include "main.h"

skill_s logs::choose(bool interactive, const skillset& source)
{
	for(auto e : source)
		add(e, getstr(e));
	return (skill_s)logs::input(interactive);
}