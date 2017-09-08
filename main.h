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
enum action_s : unsigned char {
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
	Block,
	Roof, Floor, Road,
	Brush,
};
enum feat_s : unsigned char {
	AgileCasting,  Antagonize,  ClimbingMaster,  ConnectionInkling, Diehard,
	Diversion, EnhancedResistance, ExtraResolve, FastTalk, GreatFortitude,
	GreaterSpell, Penetration, HarmUndead,  ImprovedGreatFortitude, ImprovedIronWill,
	ImprovedLightningReflexes, IronWill, JetDash, LightningReflexes, MajorPsychicPower,
	MasterCrafter, MedicalExpert, MinorPsychicPower, PenetratingSpell, PsychicPower,
	SkillFocus, SkillSynergy, SkyJockey, SpellFocus, SpellPenetration,
	Spellbane, SwimmingMaster, TechnomanticDabbler, Toughness, VeiledThreat
};
enum wall_s : unsigned char {
	NoWall, Wall, Door, Window,
};
typedef adat<skill_s, 8>	skillset;
typedef adat<ability_s, 6>	abilityset;
template<class T>
struct flags
{
	unsigned		data;
	void			clear() { data = 0; }
	bool			is(T id) const { return (data&mask(id))!=0; }
	void			set(T id) const { data |= maks(id); }
private:
	inline unsigned	mask(T id) const { return 1 << id; }
};
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
	direction_s		orientation;
	unsigned short	position;
	unsigned char	distance;
	char			initiative;
	bool			action_standart;
	bool			action_swift;
	//
	void			clear();
	class_s			choose_class(bool interactive);
	gender_s		choose_gender(bool interactive);
	theme_s			choose_theme(bool interactive);
	void			create(race_s race, class_s type, gender_s gender);
	int				get(ability_s value) const;
	int				get(skill_s value) const;
	int				get(save_s value) const;
	inline int		getbonus(ability_s value) const { return get(value) / 2 - 5; }
	int				getbonus(skill_s value) const;
	character*		getenemy() const;
	int				getinitiative() const;
	int				getmisc(skill_s value) const;
	unsigned char	getspeed() const { return 30; }
	bool			is(state_s value) const;
	bool			isclass(skill_s value) const;
	bool			isenemy(const character* target) const;
	bool			isvisible() const;
	bool			isvisible(const character* observer) const;
	void			maketurn(bool interactive);
	bool			move(direction_s d);
private:
	unsigned		states[Unconscious+1];
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
	character*		getcharacter(short unsigned index) const;
	bool			ispassable(short unsigned i) const;
	bool			ispassable(short unsigned i, direction_s d) const;
	void			normalize();
	void			rollinitiative();
	void			set(landscape_s value);
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
	void			move(character& e);
	void			next();
	void			open(const char* title);
}
class_s				choose_class();
gender_s			choose_gender();
theme_s				choose_theme();
ability_s			getability(skill_s value);
ability_s			getability(theme_s value);
inline short unsigned gi(int x, int y) { return y*mapx + x; }
const skillset&		getskills(theme_s value);
inline int			gx(short unsigned i) { return i % mapx; }
inline int			gy(short unsigned i) { return i / mapx; }
bool				isarmorcheck(skill_s value);
bool				isclass(skill_s value, class_s class_value);
bool				isuntrained(skill_s value);
extern location		map;
extern adat<character*, 256> characters;
template<class T> inline T random(T e1, T e2) { return (T)(e1 + rand()%(e2-e1+1)); }
short unsigned		to(short unsigned i, direction_s d);