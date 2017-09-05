#include "draw.h"
#include "main.h"

void __cdecl dlgerr(char const *, char const *, ...)
{
}

int main()
{
	logs::open("Starfinder");
	map[0].clear();
	short unsigned i = geti(5, 5);
	//map[0].set(i, Earth);
	//map[0].set(to(i, Right), Earth);
	map[0].set(i, Wall, Left);
	map[0].set(i, Wall, Right);
	//map[0].set(i, Brick, Up);
	map[0].set(i, Wall, Down); i = to(i, Right);
	map[0].set(i, Door, Up); i = to(i, Right);
	map[0].set(i, Wall, Up);
	character e; e.create(true, Humans, Soldier, Male);
	return 0;
}

int _stdcall WinMain(void* ci, void* pi, char* cmd, int sw)
{
	srand(time(0));
	return main();
}