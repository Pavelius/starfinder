#include "main.h"

enum item_special_s {
	Analog, Archaic, BlockItem, OperativeItem, Thrown, TwoHanded
};
struct weapondamage
{
	dice					dice;
	damage_s				type;
};
static struct iteminf
{
	const char*				name[3];
	slot_s					slot;
	char					level;
	int						price;
	char					bulk;
	weapondamage			damage;
	adat<item_special_s, 8>	special;
	unsigned char			powered[2];
} objects[] = {{{""}},
{{"Club"}, MeleeWeapon, 0, 0, 0, {{1, 6}, Bludgeon}, {2, {Analog, Archaic}}},
{{"Knife"}, MeleeWeapon, 1, 0, 0, {{1, 4}, Slashing}, {2, {Analog, OperativeItem}}},
{{"Sword"}, MeleeWeapon, 2, 0, 0, {{1, 6}, Slashing}, {1, {Analog}}},
{{"Sword, buzzblade"}, MeleeWeapon, 0, 0, 0, {{2, 6}, Slashing}, {0}, {20, 1}},
{{"Spear"}, MeleeWeapon, 1, 375, 1, {{1, 6}, Piercing}, {4, {Analog, BlockItem, Thrown, TwoHanded}}},
};

item::item(const char* id)
{
	type = find(id);
	capacity = 0;
}

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

dice item::getdamage() const
{
	return type->damage.dice;
}

damage_s item::getdamagetype() const
{
	return type->damage.type;
}