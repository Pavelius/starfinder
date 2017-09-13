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
	{{"Enhanced Resistance"}},
	{{"Extra Resolve"}},
	{{"Fast Talk"}},
	{{"GreatFortitude"}},
	{{"Greater Spell Penetration"}},
	{{"Harm Undead"}},
	{{"Improved Great Fortitude"}, {0}, {0}, {1, {GreatFortitude}}, {0}, 5},
	{{"Improved Iron Will"}, {0}, {0}, {1, {IronWill}}, {0}, 5},
	{{"Improved Lightning Reflexes"}, {0}, {0}, {1, {LightningReflexes}}, {0}, 5},
	{{"Iron Will"}},
	{{"Jet Dash"}},
	{{"Lightning Reflexes"}},
	{{"Major Psychic Power"}},
	{{"Master Crafter"}},
	{{"Medical Expert"}},
	{{"Minor Psychic Power"}},
	{{"Penetrating Spell"}},
	{{"Psychic Power"}},
	{{"Skill Focus"}},
	{{"Skill Synergy"}},
	{{"Sky Jockey"}},
	{{"Spell Focus"}},
	{{"Spell Penetration"}},
	{{"Spellbane"}},
	{{"Swimming Master"}},
	{{"Technomantic Dabbler"}},
	{{"Toughness"}},
	{{"Veiled Threat"}}
};
assert_enum(feat, VeiledThreat);