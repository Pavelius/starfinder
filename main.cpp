#include "draw.h"
#include "main.h"

void __cdecl dlgerr(char const *, char const *, ...)
{
}

int main()
{
	logs::open("Starfinder");
	character e;
	e.create(true, Humans, Soldier, Male);
	return 0;
}

int _stdcall WinMain(void* ci, void* pi, char* cmd, int sw)
{
	return main();
}