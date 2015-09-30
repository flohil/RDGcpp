#include "gameObjects.hpp"
#include "enums.hpp"
#include <iostream>
#include <map>

NamedObject::NamedObject(const std::string name_) : name(name_)
{

}

std::string NamedObject::getName() const
{
	return name;
}

VisibleObject::VisibleObject(const std::string image_) : image(image_)
{

}

std::string VisibleObject::getImage() const
{
	return image;
}

void VisibleObject::setVisible(bool visible_)
{
	visible = visible_;
}

bool VisibleObject::isVisible() const
{
	return visible;
}

Item::Item(const Classes itemClass_) : itemClass(itemClass_)
{

}

Classes Item::getItemClass() const
{
	return itemClass;
}

Item::~Item()
{

}

FightableCreature::FightableCreature(float hp, float strength, float speed, float accuracy)
{

}

float FightableCreature::getHp() const
{
	return hp;
}

float FightableCreature::getStrength() const
{
	return strength;
}

float FightableCreature::getSpeed() const
{
	return speed;
}

float FightableCreature::getAccuracy() const
{
	return accuracy;
}

Armament::Armament(const std::string name_, const std::string image_, const Classes itemClass_, const std::string type_, const float armor_, const float speed_, const float bonus_) :
type(type_), armor(armor_), speed(speed_), bonus(bonus_), NamedObject(name_), VisibleObject(image_), Item(itemClass_)
{

}

Monster::Monster(const std::string name_, const std::string image_, const DifficultyLevel::Level level_, const Attribute killBonusType_,
	const float killBonusLow_, const float killBonusHigh_, float hp_, float strength_, float speed_, float accuracy_) :
	level(level_), killBonusType(killBonusType_), killBonusLow(killBonusLow_), killBonusHigh(killBonusHigh_),
	NamedObject(name_), VisibleObject(image_), FightableCreature(hp_, strength_, speed_, accuracy_)
{

}

DifficultyLevel::Level Monster::getLevel() const
{
	return level;
}

Attribute Monster::getKillBonusType() const
{
	return killBonusType;
}

float Monster::getKillBonusLow() const
{
	return killBonusLow;
}

float Monster::getKillBonusHigh() const
{
	return killBonusHigh;
}

Potion::Potion(const std::string name_, const std::string image_, const Classes itemClass_, const std::string description_, const Target target_,
	const Attribute effect_, const Mode mode_, const float strength_, const unsigned int duration_) :
	description(description_), target(target_), effect(effect_), mode(mode_), strength(strength_), duration(duration_), NamedObject(name_), VisibleObject(image_), Item(itemClass_)
{

};

std::string Potion::getDescription() const
{
	return description;
}

Target Potion::getTarget() const
{
	return target;
}

Attribute Potion::getEffect() const
{
	return effect;
}

Mode Potion::getMode() const
{
	return mode;
}

float Potion::getStrength() const
{
	return strength;
}

unsigned int Potion::getDuration() const
{
	return duration;
}

Weapon::Weapon(const std::string name_, const std::string image_, const Classes itemClass_, const std::string type_, const float attack_,
	const float speed_, const float accuracy_, const float defence_, const unsigned int slots_, const unsigned int max_) :
	type(type_), attack(attack_), speed(speed_), accuracy(accuracy_), defence(defence_), slots(slots_), max(max_), NamedObject(name_), VisibleObject(image_), Item(itemClass_)
{

}

std::string Weapon::getType() const
{
	return type;
}

float Weapon::getAttack() const
{
	return attack;
}

float Weapon::getSpeed() const
{
	return speed;
}

float Weapon::getAccuracy() const
{
	return accuracy;
}

float Weapon::getDefence() const
{
	return defence;
}

unsigned int Weapon::getSlots() const
{
	return slots;
}

unsigned int Weapon::getMax() const
{
	return max;
}

Attack::Attack(const std::string name_, const Attribute effect_, const float hpDamage_, const float hitProbability_, const float x_) :
effect(effect_), hpDamage(hpDamage_), hitProbability(hitProbability_), x(x_), NamedObject(name_)
{

}

Attribute Attack::getEffect() const
{
	return effect;
}

float Attack::getHpDamage() const
{
	return hpDamage;
}

float Attack::getHitProbability() const
{
	return hitProbability;
}

float Attack::getX() const
{
	return x;
}

Room::Room(const std::string name_, const std::string description_, const std::string image_,
	const std::map<DoorPositions, const bool> doorPositions_,
	const std::map<MonsterProbabilities, const float> monsterProbabilities_,
	const std::map<Classes, const float> findProbabilities_,
	const unsigned int monsterCount_, const unsigned int itemCount_,
	const float itemMultiplier_) :
	doorPositions(doorPositions_), monsterProbabilities(monsterProbabilities_), findProbabilities(findProbabilities_),
	monsterCount(monsterCount_), itemCount(itemCount_), itemMultiplier(itemMultiplier_), NamedObject(name_), VisibleObject(image_)
{

}

std::string Room::getDescription() const
{
	return description;
}

std::map<DoorPositions, const bool> Room::getDoorPositions() const
{
	return doorPositions;
}

std::map<MonsterProbabilities, const float> Room::getMonsterProbabilities() const
{
	return monsterProbabilities;
}

std::map<Classes, const float> Room::getFindProbabilities() const
{
	return findProbabilities;
}

unsigned int Room::getMonsterCount() const
{
	return monsterCount;
}

unsigned int Room::getItemCount() const
{
	return itemCount;
}

float Room::getItemMultiplier() const
{
	return itemMultiplier;
}
