#include "main.h"

static struct feat_i
{
	struct desc_ability
	{
		ability_s			id;
		unsigned char		value;
	};
	struct desc_skill
	{
		skill_s				id;
		unsigned char		value;
	};
	struct desc_class
	{
		class_s				id;
		unsigned char		value;
	};
	const char*				name[2];
	adat<desc_ability, 4>	abilitites[2];
	adat<desc_skill, 4>		skills[2];
	adat<feat_s, 4>			feats;
	adat<desc_class, 4>		classes;
	unsigned char			character_level;
	unsigned char			caster_level;
	unsigned char			base_attack;
	unsigned char			key_ability;
} feat_data[] = {
	{{"Agile Casting"}, {1, {Dexterity, 15}}, {0}, {0}, {0}, 0, 4, 15},
	{{"Antagonize"}, {0}, {2, {{Diplomacy, 5}, {Intimidate, 5}}}, {0}},
	{{"Climb master"}, {0}, {1, {{Athletics, 5}}}},
	{{"Connection Inkling"}, {1, {Wisdow, 15}}, {0}, {0}, {1, {Mystic, 0}}, 5},
	{{"Diehard"}},
	{{"Diversion"}},
};