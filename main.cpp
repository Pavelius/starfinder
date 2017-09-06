#include "draw.h"
#include "main.h"

void __cdecl dlgerr(char const *, char const *, ...)
{
}

int main()
{
	character e;
	logs::open("Starfinder");
	map[0].clear();
	map[0].set(Block);
	map[0].set(gi(14, 10), Floor, Up, 6);
	map[0].set(gi(14, 10), Door, Up);
	map[0].set(gi(10, 10), Floor, Right, 10);
	map[0].set(gi(10, 11), Floor, Right, 9);
	map[0].set(gi(10, 12), Floor, Right, 8);
	map[0].normalize();
	characters.add(&e);
	e.index = gi(12, 12);
	e.create(true, Humans, Soldier, Male);
	return 0;
}

int _stdcall WinMain(void* ci, void* pi, char* cmd, int sw)
{
	srand(time(0));
	return main();
}