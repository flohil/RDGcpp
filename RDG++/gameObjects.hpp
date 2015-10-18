#ifndef GAME_OBJECTS_INCLUDE
#define GAME_OBJECTS_INCLUDE

#include "debugPrint.hpp"
#include <SFML\Graphics.hpp>
#include "enums.hpp"
#include <map>
#include <string>
#include <memory>

class NamedObject
{
public:

	NamedObject(const std::string& name_) : name(name_) {};

	const std::string getName() const { return name; };

protected:

	const std::string name;
};

// public sf::Drawable, public sf::Transformable, 
class RenderableObject : public NamedObject
{
public:

	RenderableObject(const std::string& name_) : NamedObject(name_) {}; // to be replaced with texture pointer

	bool isVisible() const { return visible; };
	void setVisible(bool visible_) { visible = visible_; };

protected:

	bool visible;

private:

	bool obtainTexture();

	//virtual void draw(sf::RenderTarget& target_, sf::RenderStates states_) const
	//{
	//	// apply the entity's transform -- combine it with the one that was passed by the caller
	//	states_.transform *= getTransform(); // getTransform() is defined by sf::Transformable

	//	// apply the texture
	//	states_.texture = &texture;

	//	// you may also override states.shader or states.blendMode if you want

	//	// draw the vertex array
	//	target_.draw(vertices, states_);
	//}

	//sf::VertexArray vertices;

	void draw(sf::RenderWindow& window, float deltaTime);

	sf::Texture texture;
	sf::Sprite sprite;
};

class Item : public RenderableObject
{
public:

	class Item(const std::string& name_, const Classes::Enum itemClass_) : RenderableObject(name_), itemClass(itemClass_) {};

	Classes::Enum getItemClass() const { return itemClass; };

protected:

	const Classes::Enum itemClass;

	virtual Item::~Item() = 0;
};

class Creature
{
public:

	Creature(float hp_, float strength_, float speed_, float accuracy_)
		: orHp(hp_), orStrength(strength_), orSpeed(speed_), orAccuracy(accuracy_), hp(hp_), strength(strength_), speed(speed_), accuracy(accuracy_) {};

	float hp, strength, speed, accuracy;

	void resetOriginals();

private:
	float orHp, orStrength, orSpeed, orAccuracy;

};

class Armament : public Item, public DebugPrintObject
{
public:

	Armament(const std::string& name_, const Classes::Enum itemClass_, const std::string& type_, const float armor_, const float speed_, const float bonus_) :
		Item(name_, itemClass_), type(type_), armor(armor_), speed(speed_), bonus(bonus_) {};

	std::string getType() const { return type; };
	float getArmor() const { return armor; };
	float getSpeed() const { return speed; };
	float getBonus() const { return bonus; };
	virtual void debugPrint() const;

protected:

	const std::string type;
	const float armor, speed, bonus;
};

class Monster : public RenderableObject, public Creature, public DebugPrintObject
{
public:

	Monster(const std::string& name_, const DifficultyLevel::Enum level_, const Attribute::Enum killBonusType_, float killBonus_, float hp_, float strength_, float speed_, float accuracy_) :
		RenderableObject(name_), Creature(hp_, strength_, speed_, accuracy_), level(level_), killBonusType(killBonusType_), killBonus(killBonus_) {};

	DifficultyLevel::Enum getLevel() const { return level; };
	Attribute::Enum getKillBonusType() const { return killBonusType; };
	float getKillBonus() const { return killBonus; };
	virtual void debugPrint() const;

protected:

	const DifficultyLevel::Enum level;
	const Attribute::Enum killBonusType;
	const float killBonus;
};

class Potion : public Item, public DebugPrintObject
{
public:

	Potion(const std::string& name_, const Classes::Enum itemClass_, const std::string& description_, const Target::Enum target_, const Attribute::Enum effect_, const Mode::Enum mode_, const float strength_, const unsigned int duration_) :
		Item(name_, itemClass_), description(description_), target(target_), effect(effect_), mode(mode_), strength(strength_), duration(duration_) {};
	
	std::string getDescription() const { return description; };
	Target::Enum getTarget() const { return target; };
	Attribute::Enum getEffect() const { return effect; };
	Mode::Enum getMode() const { return mode; };
	float getStrength() const { return strength; };
	unsigned int getDuration() const { return duration; };
	virtual void debugPrint() const;

protected:

	const std::string description;
	const Target::Enum target;
	const Attribute::Enum effect;
	const Mode::Enum mode;
	const float strength;
	const unsigned int duration;
};

class Weapon : public Item, public DebugPrintObject
{
public:

	Weapon::Weapon(const std::string& name_, const Classes::Enum itemClass_, const WeaponType::Enum type_, const float attack_, const float speed_, const float accuracy_, const float defence_, const unsigned int slots_, const unsigned int max_) :
		Item(name_, itemClass_), type(type_), attack(attack_), speed(speed_), accuracy(accuracy_), defence(defence_), slots(slots_), max(max_) {};

	WeaponType::Enum getType() const { return type; };
	float getAttack() const { return attack; };
	float getSpeed() const { return speed; };
	float getAccuracy() const { return accuracy; };
	float getDefence() const { return defence; };
	unsigned int getSlots() const { return slots; };
	unsigned int getMax() const { return max; };
	virtual void debugPrint() const;

protected:

	const WeaponType::Enum type;
	const float attack, speed, accuracy, defence;
	const unsigned int slots, max;
};

class Attack : public NamedObject, public DebugPrintObject
{
public:

	Attack(const std::string& name_, const Attribute::Enum effect_, const float hpDamageMultiplier_, const float hitProbability_, const float attributeDamageMultiplier_, const float attackStatsLowMultiplier_, const float attackStatsHighMultiplier_) :
		NamedObject(name_), effect(effect_), hpDamageMultiplier(hpDamageMultiplier_), hitProbability(hitProbability_), attributeDamageMultiplier(attributeDamageMultiplier_), attackStatsHighMultiplier(attackStatsHighMultiplier_), attackStatsLowMultiplier(attackStatsLowMultiplier_) {};
	
	Attribute::Enum getEffect() const { return effect; };
	float getHpDamageMultiplier() const { return hpDamageMultiplier; };
	float getHitProbability() const { return hitProbability; };
	float getAttributeDamageMultiplier() const { return attributeDamageMultiplier; };
	float getAttackStatsLowMultiplier() const { return attackStatsLowMultiplier; };
	float getAttackStatsHighMultiplier() const { return attackStatsHighMultiplier; };
	virtual void debugPrint() const;

protected:

	const Attribute::Enum effect;
	const float hpDamageMultiplier, hitProbability, attributeDamageMultiplier;
		
private:
	
	const float attackStatsLowMultiplier, attackStatsHighMultiplier;
};

class Room : public NamedObject, public DebugPrintObject
{
public:

	Room(const std::string& name_, const std::string& description_) :
		NamedObject(name_), description(description_) {};

	std::string getDescription() const { return description; };
	virtual void debugPrint() const;

protected:

	const std::string description;
};

#endif // GAME_OBJECTS_INCLUDE
