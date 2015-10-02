#ifndef GAME_OBJECTS_INCLUDE
#define GAME_OBJECTS_INCLUDE

#include "enums.hpp"
#include <map>
#include <string>

class NamedObject
{
public:

	NamedObject(const std::string name);

	std::string getName() const;

protected:

	const std::string name;
};

class VisibleObject
{
public:

	VisibleObject(const std::string image); // to be replaced with texture pointer

	std::string getImage() const;
	bool isVisible() const;
	void setVisible(bool visible_);

protected:

	const std::string image;
	bool visible;
};

class Item
{
public:

	class Item(const Classes itemClass);

	Classes getItemClass() const;

protected:

	virtual Item::~Item() = 0;

protected:

	const Classes itemClass;
};

class FightableCreature
{
public:

	FightableCreature(float hp, float strength, float speed, float accuracy);

	float getHp() const;
	float getStrength() const;
	float getSpeed() const;
	float getAccuracy() const;

protected:

	float hp, strength, speed, accuracy;
};

class Armament : public Item, public NamedObject, public VisibleObject
{
public:

	Armament(const std::string name, const std::string image, const Classes itemClass, const std::string type, const float armor, const float speed, const float bonus);

	std::string getType() const;
	float getArmor() const;
	float getSpeed() const;
	float getBonus() const;

protected:

	const std::string type;
	const float armor, speed, bonus;
};

class Monster : public NamedObject, public VisibleObject, public FightableCreature
{
public:

	Monster(const std::string name, const std::string image, const DifficultyLevel::Level level, const Attribute killBonusType, float killBonus, float hp, float strength, float speed, float accuracy);

	DifficultyLevel::Level getLevel() const;
	Attribute getKillBonusType() const;
	float getKillBonus() const;

protected:

	const DifficultyLevel::Level level;
	const Attribute killBonusType;
	const float killBonus;
};

class Potion : public Item, public NamedObject, public VisibleObject
{
public:

	Potion(const std::string name, const std::string image, const Classes itemClass, const std::string description, const Target target,
		const Attribute effect, const Mode mode, const float strength, const unsigned int duration);

	std::string getDescription() const;
	Target getTarget() const;
	Attribute getEffect() const;
	Mode getMode() const;
	float getStrength() const;
	unsigned int getDuration() const;

protected:

	const std::string description;
	const Target target;
	const Attribute effect;
	const Mode mode;
	const float strength;
	const unsigned int duration;
};

class Weapon : public Item, public NamedObject, public VisibleObject
{
public:

	Weapon(const std::string name, const std::string image, const Classes itemClass, const std::string type, const float attack, const float speed,
		const float accuracy, const float defence, const unsigned int slots, const unsigned int max);

	std::string getType() const;
	float getAttack() const;
	float getSpeed() const;
	float getAccuracy() const;
	float getDefence() const;
	unsigned int getSlots() const;
	unsigned int getMax() const;

protected:

	const std::string type;
	const float attack, speed, accuracy, defence;
	const unsigned int slots, max;
};

class Attack : public NamedObject
{
public:

	Attack(const std::string name, const Attribute effect, const float hpDamageMultiplier, const float hitProbability, 
		const float attributeDamageMultiplier, const float attackStatsLowMultiplier, const float attackStatsHighMultiplier);

	Attribute getEffect() const;
	float getHpDamageMultiplier() const;
	float getHitProbability() const;
	float getAttributeDamageMultiplier() const;
	float getStatsLowMultiplier() const;
	float getStatsHighMultiplier() const;

protected:

	const Attribute effect;
	const float hpDamageMultiplier, hitProbability, attributeDamageMultiplier;
		
private:
	
	const float attackStatsLowMultiplier, attackStatsHighMultiplier;
};

class Room : public NamedObject, public VisibleObject
{
public:

	Room(const std::string name, const std::string description, const std::string image,
		const std::map<DoorPositions, const bool> doorPositions,
		const std::map<MonsterProbabilities, const float> monsterProbabilities,
		const std::map<Classes, const float> findProbabilities,
		const unsigned int monsterCount, const unsigned int itemCount,
		const float itemMultiplier);

	std::string getDescription() const;
	std::map<DoorPositions, const bool> getDoorPositions() const;
	std::map<MonsterProbabilities, const float> getMonsterProbabilities() const;
	std::map<Classes, const float> getFindProbabilities() const;
	unsigned int getMonsterCount() const;
	unsigned int getItemCount() const;
	float getItemMultiplier() const;

protected:

	const std::string description;
	const std::map<DoorPositions, const bool> doorPositions;
	const std::map<MonsterProbabilities, const float> monsterProbabilities;
	const std::map<Classes, const float> findProbabilities;
	const unsigned int monsterCount, itemCount;
	const float itemMultiplier;
};

#endif // GAME_OBJECTS_INCLUDE
