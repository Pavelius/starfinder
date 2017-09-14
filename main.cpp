#include "draw.h"
#include "main.h"

void __cdecl dlgerr(char const *, char const *, ...)
{
}

int main()
{
	character e, e2;
	logs::open("Starfinder");
	map.clear();
	map.set(Block);
	map.set(gi(14, 10), Floor, Up, 6);
	map.set(gi(14, 10), Door, Up);
	map.set(gi(10, 10), Floor, Right, 10);
	map.set(gi(10, 11), Floor, Right, 9);
	map.set(gi(10, 12), Floor, Right, 8);
	map.set(gi(8, 13), Floor, Right, 10);
	map.set(gi(11, 14), Floor, Right, 10);
	map.set(gi(10, 15), Floor, Right, 12);
	map.set(gi(6, 16), Floor, Right, 20);
	map.set(gi(6, 17), Floor, Right, 20);
	map.set(gi(1, 18), Floor, Right, 16);
	map.normalize();
	characters.add(&e);
	e.create(Humans, Soldier, Male);
	e.position = gi(12, 12);
	e.name = "Павел";
	e.wear[MeleeWeapon] = "Sword";
	characters.add(&e2);
	e2.create(Humans, Soldier, Female);
	e2.position = gi(16, 11);
	e2.name = "Мира";
	e2.wear[MeleeWeapon] = "Longsword";
	map.combat();
	return 0;
}

int _stdcall WinMain(void* ci, void* pi, char* cmd, int sw)
{
	srand(time(0));
	return main();
}