#include "adat.h"
#include "crt.h"
#include "dice.h"

#pragma once

#define assert_enum(name, last) static_assert(sizeof(name##_data) / sizeof(name##_data[0]) == last + 1,\
	"Invalid count of " #name " elements")

const int mapx = 128;
const int mapy = 128;

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
enum direction_s : unsigned char {
	Left, Right, Up, Down, Center
};
enum landscape_s : unsigned char {
	NoLandscape,
	Earth,
	Roof, Floor, Road,
	Brush,
};
enum wall_s : unsigned char {
	NoWall, Wall, Door, Window,
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
struct location
{
	landscape_s		floor[mapx*mapy];
	wall_s			wallh[mapx*mapy];
	wall_s			wallv[mapx*mapy];
	location();
	void			clear();
	landscape_s		get(short unsigned i) const { return floor[i]; }
	wall_s			get(short unsigned i, direction_s d) const;
	bool			ispassable(short unsigned i) const;
	bool			ispassable(short unsigned i, direction_s d) const;
	void			set(short unsigned i, landscape_s value);
	short unsigned	set(short unsigned i, landscape_s value, direction_s move_direction, int count);
	void			set(short unsigned i, wall_s value, direction_s d);
	short unsigned	set(short unsigned i, wall_s value, direction_s d, direction_s move_direction, int count);
};
namespace logs
{
	void			add(const char* format, ...);
	void			add(int id, const char* format ...);
	void			addns(const char* format, ...);
	void			addv(const char* format, const char* vl, bool test_spaces);
	int				input();
	void			next();
	void			open(const char* title);
}
ability_s			getability(skill_s value);
inline short unsigned geti(int x, int y) { return y*mapx + x; }
const skillset&		getskills(theme_s value);
inline int			getx(short unsigned i) { return i % mapx; }
inline int			gety(short unsigned i) { return i / mapx; }
bool				isarmorcheck(skill_s value);
bool				isclass(skill_s value, class_s class_value);
bool				isuntrained(skill_s value);
extern location		map[4];
short unsigned		to(short unsigned i, direction_s d);