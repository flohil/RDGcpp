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

void Armament::debugPrint() const
{
	std::cout << "Armament: " << std::endl;
	std::cout << "name = " << name << std::endl;
	std::cout << "image = " << image << std::endl;
	std::cout << "itemClass = " << itemClass << std::endl;
	std::cout << "type = " << type << std::endl;
	std::cout << "armor = " << armor << std::endl;
	std::cout << "speed = " << speed << std::endl;
	std::cout << "bonus = " << bonus << std::endl;
	std::cout << std::endl;
}

Monster::Monster(const std::string name_, const std::string image_, const DifficultyLevel::Level level_, const Attribute killBonusType_, float killBonus_, float hp_, float strength_, float speed_, float accuracy_) :
	level(level_), killBonusType(killBonusType_), killBonus(killBonus_), NamedObject(name_), VisibleObject(image_), FightableCreature(hp_, strength_, speed_, accuracy_)
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

float Monster::getKillBonus() const
{
	return killBonus;
}

void Monster::debugPrint() const
{
	std::cout << "Monster: " << std::endl;
	std::cout << "name = " << name << std::endl;
	std::cout << "image = " << image << std::endl;
	std::cout << "level = " << level << std::endl;
	std::cout << "killBonusType = " << killBonusType << std::endl;
	std::cout << "killBonus = " << killBonus << std::endl;
	std::cout << "hp = " << hp << std::endl;
	std::cout << "strength = " << strength << std::endl;
	std::cout << "speed = " << speed << std::endl;
	std::cout << "accuracy = " << accuracy << std::endl;
	std::cout << std::endl;
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

void Potion::debugPrint() const
{
	std::cout << "Potion: " << std::endl;
	std::cout << "name = " << name << std::endl;
	std::cout << "image = " << image << std::endl;
	std::cout << "itemClass = " << itemClass << std::endl;
	std::cout << "description = " << description << std::endl;
	std::cout << "target = " << target << std::endl;
	std::cout << "effect = " << effect << std::endl;
	std::cout << "mode = " << mode << std::endl;
	std::cout << "strength = " << strength << std::endl;
	std::cout << "duration = " << duration << std::endl;
	std::cout << std::endl;
}

Weapon::Weapon(const std::string name_, const std::string image_, const Classes itemClass_, const std::string type_, const float attack_,
	const float speed_, const float accuracy_, const float defence_, const unsigned int slots_, const unsigned int max_) :
	type(type_), attack(attack_), speed(speed_), accuracy(accuracy_), defence(defence_), slots(slots_), max(max_), NamedObject(name_), VisibleObject(image_), Item(itemClass_)
{

}

void Weapon::debugPrint() const
{
	std::cout << "Weapon: " << std::endl;
	std::cout << "name = " << name << std::endl;
	std::cout << "image = " << image << std::endl;
	std::cout << "itemClass = " << itemClass << std::endl;
	std::cout << "type = " << type << std::endl;
	std::cout << "attack = " << attack << std::endl;
	std::cout << "speed = " << speed << std::endl;
	std::cout << "accuracy = " << accuracy << std::endl;
	std::cout << "defence = " << defence << std::endl;
	std::cout << "slots = " << slots << std::endl;
	std::cout << "max = " << max << std::endl;
	std::cout << std::endl;
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

Attack::Attack(const std::string name_, const Attribute effect_, const float hpDamageMultiplier_, const float hitProbability_, 
	const float attributeDamageMultiplier_, const float attackStatsLowMultiplier_, const float attackStatsHighMultiplier_) :
effect(effect_), hpDamageMultiplier(hpDamageMultiplier_), hitProbability(hitProbability_), attributeDamageMultiplier(attributeDamageMultiplier_), attackStatsHighMultiplier(attackStatsHighMultiplier_), attackStatsLowMultiplier(attackStatsLowMultiplier_), NamedObject(name_)
{

}

void Attack::debugPrint() const
{
	std::cout << "Attack: " << std::endl;
	std::cout << "name = " << name << std::endl;
	std::cout << "effect = " << effect << std::endl;
	std::cout << "hpDamageMultiplier = " << hpDamageMultiplier << std::endl;
	std::cout << "hitProbability = " << hitProbability << std::endl;
	std::cout << "attributeDamageMultiplier = " << attributeDamageMultiplier << std::endl;
	std::cout << "attackStatsLowMultiplier = " << attackStatsLowMultiplier << std::endl;
	std::cout << "attackStatsHighMultiplier = " << attackStatsHighMultiplier << std::endl;
	std::cout << std::endl;
}

Attribute Attack::getEffect() const
{
	return effect;
}

float Attack::getHpDamageMultiplier() const
{
	return hpDamageMultiplier;
}

float Attack::getHitProbability() const
{
	return hitProbability;
}

float Attack::getAttributeDamageMultiplier() const
{
	return attributeDamageMultiplier;
}

float Attack::getStatsLowMultiplier() const
{
	return attackStatsLowMultiplier;
}

float Attack::getStatsHighMultiplier() const
{
	return attackStatsHighMultiplier;
}

Room::Room(const std::string name_, const std::string description_) :
	description(description_), NamedObject(name_)
{

}

std::string Room::getDescription() const
{
	return description;
}
