#include "main.h"

enum item_special_s {
	Analog, Archaic, BlockItem, OperativeItem, Thrown, TwoHanded
};
static struct iteminf
{
	const char*				name[3];
	slot_s					slot;
	char					level;
	int						price;
	char					bulk;
	damageinfo				damage;
	adat<item_special_s, 8>	special;
	unsigned char			powered[2];
	critical_i				critical;
} objects[] = {{{""}},
{{"Club"}, MeleeWeapon, 0, 0, 0, {{1, 6}, Bludgeon}, {2, {Analog, Archaic}}},
{{"Knife"}, MeleeWeapon, 1, 0, 0, {{1, 4}, Slashing}, {2, {Analog, OperativeItem}}},
{{"Longsword"}, MeleeWeapon, 1, 375, 1, {{1, 8}, Slashing}, {1, {Analog}}},
{{"Sword"}, MeleeWeapon, 2, 0, 0, {{1, 6}, Slashing}, {1, {Analog}}},
{{"Sword, buzzblade"}, MeleeWeapon, 0, 0, 0, {{2, 6}, Slashing}, {0}, {20, 1}},
{{"Spear"}, MeleeWeapon, 1, 375, 1, {{1, 6}, Piercing}, {4, {Analog, BlockItem, Thrown, TwoHanded}}},
{{"Starfire Sword"}, MeleeWeapon, 7, 6120, 1, {{2, 4}, Fire}, {0}, {20, 2}, {Burn, {1,8}}},
{{"Starfire Sword, inferno"}, MeleeWeapon, 17, 246000, 1, {{7, 8}, Fire}, {0}, {20, 1}, {Burn, {4, 12}}},
};

iteminf* item::find(const char* id)
{
	for(auto& e : objects)
	{
		if(strcmp(e.name[0], id) == 0)
			return &e;
	}
	return 0;
}

const char* item::getname() const
{
	return type->name[0];
}

const damageinfo& item::getdamage() const
{
	return type->damage;
}

const critical_i& item::getcritical() const
{
	return type->critical;
}

bool item::isanalog() const
{
	return type->special.is(Analog);
}

bool item::isarchaic() const
{
	return type->special.is(Archaic);
}

bool item::isblock() const
{
	return type->special.is(BlockItem);
}

bool item::istwohanded() const
{
	return type->special.is(TwoHanded);
}