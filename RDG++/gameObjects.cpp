#include "gameObjects.hpp"
#include "enums.hpp"
#include "resourceManager.hpp"
#include <iostream>
#include <map>

bool RenderableObject::obtainTexture() {

	texturePtr = ResourceManager::getInstance().getTexture(name);
	return false;
}

Item::~Item()
{

}

void Armament::debugPrint() const
{
	std::cout << "Armament: " << std::endl;
	std::cout << "name = " << name << std::endl;
	std::cout << "itemClass = " << itemClass << std::endl;
	std::cout << "type = " << type << std::endl;
	std::cout << "armor = " << armor << std::endl;
	std::cout << "speed = " << speed << std::endl;
	std::cout << "bonus = " << bonus << std::endl;
	std::cout << std::endl;
}

void Monster::debugPrint() const
{
	std::cout << "Monster: " << std::endl;
	std::cout << "name = " << name << std::endl;
	std::cout << "level = " << level << std::endl;
	std::cout << "killBonusType = " << killBonusType << std::endl;
	std::cout << "killBonus = " << killBonus << std::endl;
	std::cout << "hp = " << hp << std::endl;
	std::cout << "strength = " << strength << std::endl;
	std::cout << "speed = " << speed << std::endl;
	std::cout << "accuracy = " << accuracy << std::endl;
	std::cout << std::endl;
}

void Potion::debugPrint() const
{
	std::cout << "Potion: " << std::endl;
	std::cout << "name = " << name << std::endl;
	std::cout << "itemClass = " << itemClass << std::endl;
	std::cout << "description = " << description << std::endl;
	std::cout << "target = " << target << std::endl;
	std::cout << "effect = " << effect << std::endl;
	std::cout << "mode = " << mode << std::endl;
	std::cout << "strength = " << strength << std::endl;
	std::cout << "duration = " << duration << std::endl;
	std::cout << std::endl;
}

void Weapon::debugPrint() const
{
	std::cout << "Weapon: " << std::endl;
	std::cout << "name = " << name << std::endl;
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

void Room::debugPrint() const
{
	std::cout << "Room: " << std::endl;
	std::cout << "name = " << name << std::endl;
	std::cout << "description = " << description << std::endl;
	std::cout << std::endl;
}