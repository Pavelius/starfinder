#include "draw.h"
#include "main.h"

void __cdecl dlgerr(char const *, char const *, ...)
{
}

int main()
{
	character e2;
	character e;
	logs::open("Starfinder");
	map.clear();
	map.set(Block);
	map.set(gi(14, 10), Floor, Up, 6);
	map.set(gi(14, 10), Door, Up);
	map.set(gi(10, 10), Floor, Right, 10);
	map.set(gi(10, 11), Floor, Right, 9);
	map.set(gi(10, 12), Floor, Right, 8);
	map.normalize();
	characters.add(&e);
	e.create(Humans, Soldier, random(Male,Female));
	e.position = gi(12, 12);
	characters.add(&e2);
	e2.create(Humans, Soldier, random(Male, Female));
	e2.position = gi(16, 11);
	map.rollinitiative();
	for(auto e : characters)
		e->maketurn(true);
	return 0;
}

int _stdcall WinMain(void* ci, void* pi, char* cmd, int sw)
{
	srand(time(0));
	return main();
}