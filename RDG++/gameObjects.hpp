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

	NamedObject(const std::string name_) : name(name_) {};

	std::string getName() const { return name; };

protected:

	const std::string name;
};

class RenderableObject : public sf::Drawable, public sf::Transformable, public NamedObject
{
public:

	RenderableObject(const std::string name_) : NamedObject(name_) {}; // to be replaced with texture pointer

	bool isVisible() const { return visible; };
	void setVisible(bool visible_) { visible = visible_; };

protected:

	bool visible;

private:

	bool obtainTexture();

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		// apply the entity's transform -- combine it with the one that was passed by the caller
		states.transform *= getTransform(); // getTransform() is defined by sf::Transformable

		// apply the texture
		states.texture = &m_texture;

		// you may also override states.shader or states.blendMode if you want

		// draw the vertex array
		target.draw(m_vertices, states);
	}

	sf::VertexArray m_vertices;
	sf::Texture m_texture;
	std::shared_ptr<sf::Texture> texturePtr;
};

class Item : public RenderableObject
{
public:

	class Item(const std::string name_, const Classes itemClass_) : RenderableObject(name_), itemClass(itemClass_) {};

	Classes getItemClass() const { return itemClass; };

protected:

	const Classes itemClass;

	virtual Item::~Item() = 0;
};

class FightableCreature
{
public:

	FightableCreature(float hp_, float strength_, float speed_, float accuracy_)
	{
		hp = hp_;
		strength = strength_;
		speed = speed_;
		accuracy = accuracy_;
	};

	float getHp() const { return hp; };
	float getStrength() const { return strength; };
	float getSpeed() const { return speed; };
	float getAccuracy() const { return accuracy; };

protected:

	float hp, strength, speed, accuracy;
};

class Armament : public Item, public DebugPrintObject
{
public:

	Armament(const std::string name_, const Classes itemClass_, const std::string type_, const float armor_, const float speed_, const float bonus_) :
		type(type_), armor(armor_), speed(speed_), bonus(bonus_), Item(name_, itemClass_) {};

	std::string getType() const { return type; };
	float getArmor() const { return armor; };
	float getSpeed() const { return speed; };
	float getBonus() const { return bonus; };
	virtual void debugPrint() const;

protected:

	const std::string type;
	const float armor, speed, bonus;
};

class Monster : public RenderableObject, public FightableCreature, public DebugPrintObject
{
public:

	Monster(const std::string name_, const DifficultyLevel::Level level_, const Attribute killBonusType_, float killBonus_, float hp_, float strength_, float speed_, float accuracy_) :
		level(level_), killBonusType(killBonusType_), killBonus(killBonus_), RenderableObject(name_), FightableCreature(hp_, strength_, speed_, accuracy_) {};

	DifficultyLevel::Level getLevel() const { return level; };
	Attribute getKillBonusType() const { return killBonusType; };
	float getKillBonus() const { return killBonus; };
	virtual void debugPrint() const;

protected:

	const DifficultyLevel::Level level;
	const Attribute killBonusType;
	const float killBonus;
};

class Potion : public Item, public DebugPrintObject
{
public:

	Potion(const std::string name_, const Classes itemClass_, const std::string description_, const Target target_, const Attribute effect_, const Mode mode_, const float strength_, const unsigned int duration_) :
		description(description_), target(target_), effect(effect_), mode(mode_), strength(strength_), duration(duration_), Item(name_, itemClass_) {};
	
	std::string getDescription() const { return description; };
	Target getTarget() const { return target; };
	Attribute getEffect() const { return effect; };
	Mode getMode() const { return mode; };
	float getStrength() const { return strength; };
	unsigned int getDuration() const { return duration; };
	virtual void debugPrint() const;

protected:

	const std::string description;
	const Target target;
	const Attribute effect;
	const Mode mode;
	const float strength;
	const unsigned int duration;
};

class Weapon : public Item, public DebugPrintObject
{
public:

	Weapon::Weapon(const std::string name_, const Classes itemClass_, const WeaponType type_, const float attack_, const float speed_, const float accuracy_, const float defence_, const unsigned int slots_, const unsigned int max_) :
		type(type_), attack(attack_), speed(speed_), accuracy(accuracy_), defence(defence_), slots(slots_), max(max_), Item(name_, itemClass_) {};

	WeaponType getType() const { return type; };
	float getAttack() const { return attack; };
	float getSpeed() const { return speed; };
	float getAccuracy() const { return accuracy; };
	float getDefence() const { return defence; };
	unsigned int getSlots() const { return slots; };
	unsigned int getMax() const { return max; };
	virtual void debugPrint() const;

protected:

	const WeaponType type;
	const float attack, speed, accuracy, defence;
	const unsigned int slots, max;
};

class Attack : public NamedObject, public DebugPrintObject
{
public:

	Attack(const std::string name_, const Attribute effect_, const float hpDamageMultiplier_, const float hitProbability_,const float attributeDamageMultiplier_, const float attackStatsLowMultiplier_, const float attackStatsHighMultiplier_) :
		effect(effect_), hpDamageMultiplier(hpDamageMultiplier_), hitProbability(hitProbability_), attributeDamageMultiplier(attributeDamageMultiplier_), attackStatsHighMultiplier(attackStatsHighMultiplier_), attackStatsLowMultiplier(attackStatsLowMultiplier_), NamedObject(name_) {};
	
	Attribute getEffect() const { return effect; };
	float getHpDamageMultiplier() const { return hpDamageMultiplier; };
	float getHitProbability() const { return hitProbability; };
	float getAttributeDamageMultiplier() const { return attributeDamageMultiplier; };
	float getAttackStatsLowMultiplier() const { return attackStatsLowMultiplier; };
	float getAttackStatsHighMultiplier() const { return attackStatsHighMultiplier; };
	virtual void debugPrint() const;

protected:

	const Attribute effect;
	const float hpDamageMultiplier, hitProbability, attributeDamageMultiplier;
		
private:
	
	const float attackStatsLowMultiplier, attackStatsHighMultiplier;
};

class Room : public NamedObject, public DebugPrintObject
{
public:

	Room(const std::string name_, const std::string description_) :
		description(description_), NamedObject(name_) {};

	std::string getDescription() const { return description; };
	virtual void debugPrint() const;

protected:

	const std::string description;
};

#endif // GAME_OBJECTS_INCLUDE
