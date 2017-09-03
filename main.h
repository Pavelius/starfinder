#pragma once

#define assert_enum(name, last) static_assert(sizeof(name##_data) / sizeof(name##_data[0]) == last + 1, "Invalid count of " #name " elements");

enum ability_s : unsigned char {
	Strenght, Dexterity, Constitution, Intellegence, Wisdow, Charisma
};
enum skill_s : unsigned char {
	Acrobatics, Athletics, Bluff, Culture, Diplomacy,
	Disguise, Engineering, Intimidate, LifeScience, Medicine,
	Mysticism, Perception, PhysicalScience, Piloting, Profession,
	SenceMotive, SleightOfHand, Stealth, Survival,
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
	short			hit;
	short			stamina;
	short			temporary;
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
	//
	int				get(skill_s value) const;
	int				getbonus(skill_s value) const;
	bool			isclass(skill_s value) const;
};