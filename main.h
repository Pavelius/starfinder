#include "adat.h"
#include "crt.h"
#include "dice.h"
#include "logs.h"

#pragma once

#define assert_enum(name, last) static_assert(sizeof(name##_data) / sizeof(name##_data[0]) == last + 1,\
	"Invalid count of " #name " elements")

enum ability_s : unsigned char {
	Strenght, Dexterity, Constitution, Intellegence, Wisdow, Charisma
};
enum skill_s : unsigned char {
	Acrobatics, Athletics, Bluff, Computers, Culture,
	Diplomacy, Disguise, Engineering, Intimidate, LifeScience,
	Medicine, Mysticism, Perception, PhysicalScience, Piloting,
	Profession, SenceMotive, SleightOfHand, Stealth, Survival,
};
enum race_s : unsigned char {
	Androids, Humans, Kasathas, Lashuntas,
	Shirrens, Vesk, Ysoki,
};
enum gender_s : unsigned char {
	Male, Female,
};
enum class_s : unsigned char {
	Envoy, Mechanic, Mystic, Operative, Solarian,
	Soldier, Technomancer,
};
enum theme_s : unsigned char {
	AcePilot, BountyHunter, Icon, Mercenary, Outlaw,
	Priest, Scholar, Spacefarer, Xenoseeker
};
struct point_i
{
	unsigned char	hit;
	unsigned char	stamina;
	unsigned char	temporary;
	unsigned char	resolve;
};
enum save_s : unsigned char {
	Fortitude, Reflex, Will
};
enum action_type_s : unsigned char {
	StandartAction, MoveAction, SwiftAction, FullAction, Reaction
};
enum state_s : unsigned char {
	Asleep, Bleeding, Blinded, Broken, Burning,
	Confused, Cowering, Dazed, Dazzled, Dead,
	Deafened, Dying, Encumbered, Entagled, Exhaused,
	Fascinated, Fatigued, FlatFooted, Frightened, Grappled,
	Helpless, Nauseated, Offkilter, OffTarget, Overburned,
	Panicked, Paralyzed, Pinned, Prone, Shaken,
	Sickened, Stable, Straggered, Stunned, Unconscious
};
typedef adat<skill_s, 8>	skillset;
typedef adat<ability_s, 6>	abilityset;
struct character
{
	unsigned char	abilities[Charisma + 1];
	unsigned char	skills[Survival + 1];
	race_s			race;
	gender_s		gender;
	class_s			type;
	theme_s			theme;
	skill_s			theme_skill;
	point_i			points;
	//
	void			clear();
	void			create(bool interactive, race_s race, class_s type, gender_s gender);
	int				get(ability_s value) const;
	int				get(skill_s value) const;
	int				get(save_s value) const;
	inline int		getbonus(ability_s value) const { return get(value) / 2 - 5; }
	int				getbonus(skill_s value) const;
	int				getmisc(skill_s value) const;
	bool			isclass(skill_s value) const;
private:
	unsigned char	skills_bonus[Survival + 1];
	unsigned char	native_abilities[Charisma + 1];
};
namespace logs
{
	skill_s			choose(bool interactive, const skillset& source);
	ability_s		choose(bool interactive, const abilityset& source);
}
ability_s			getability(skill_s value);
const skillset&		getskills(theme_s value);
bool				isarmorcheck(skill_s value);
bool				isclass(skill_s value, class_s class_value);
bool				isuntrained(skill_s value);