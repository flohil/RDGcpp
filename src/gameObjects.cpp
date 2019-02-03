#include "gameObjects.hpp"
#include "enums.hpp"
#include "resourceManager.hpp"
#include "map.hpp"
#include "OutputFormatter.hpp"
#include <iostream>
#include <map>

bool RenderableObject::obtainSprite(ObjectType::Enum objectType) {

	if (objectType == ObjectType::TILE)
	{
		sprite = ResourceManager::getInstance().getRandomTile(name);
	}
	else
	{
		sprite.setTexture(ResourceManager::getInstance().getTexture(name));
		sprite.setTextureRect(sf::IntRect(0, 0, ResourceManager::getInstance().getTexture(name).getSize().x, ResourceManager::getInstance().getTexture(name).getSize().y));
	}
	return false;
}

void RenderableObject::setScale(const sf::Vector2f scale_)
{
	if (scale_ == scale)
	{
		return;
	}

	if (objectType == ObjectType::TILE)
	{
		sf::Texture& tex = ResourceManager::getInstance().getTexture("tileset");
		scale.x = scale_.x * 0.1f;
		scale.y = scale_.y * 0.1f;
		size.x = tex.getSize().x * scale.x;
		size.y = tex.getSize().y * scale.y;
	}
	else
	{
		sf::Texture& tex = ResourceManager::getInstance().getTexture(name);
		scale.x = scale_.x;
		scale.y = scale_.y;
		size.x = tex.getSize().x * scale.x;
		size.y = tex.getSize().y * scale.y;
	}

	sprite.setScale(scale_);
}

void RenderableObject::setSize(const unsigned int width, const unsigned int height)
{
	sf::Vector2f scale;

	if (size.x == width && size.y == height)
	{
		return; //did not change
	}

	if (objectType == ObjectType::TILE)
	{
		sf::Texture& tex = ResourceManager::getInstance().getTexture("tileset");
		scale = sf::Vector2f(static_cast<float>(width) / tex.getSize().x * 10, static_cast<float>(height) / tex.getSize().y * 10); //tileset is 320x320 but tile is only 32x32
	}
	else
	{
		sf::Texture& tex = ResourceManager::getInstance().getTexture(name);
		scale = sf::Vector2f(static_cast<float>(width) / tex.getSize().x, static_cast<float>(height) / tex.getSize().y);
	}

	size.x = static_cast<float>(width);
	size.y = static_cast<float>(height);

	sprite.setScale(scale);
}

void RenderableObject::setPosition(const sf::Vector2f pos_)
{
	if (pos_ == rawPos)
	{ 
		return; // position did not change
	}

	rawPos = pos_;

	finalPos.x = rawPos.x + size.x * 0.5f;
	finalPos.y = rawPos.y + size.y * 0.5f;
	
	sprite.setPosition(finalPos);
}


void RenderableObject::setCenteredPosition(const sf::Vector2i pos_)
{
	sf::Vector2f floatPos;

	floatPos.x = static_cast<float>(pos_.x);
	floatPos.y = static_cast<float>(pos_.y);

	finalPos.x = floatPos.x;
	finalPos.y = floatPos.y;

	rawPos.x = finalPos.x - size.x * 0.5f;
	rawPos.y = finalPos.y - size.y * 0.5f;

	sprite.setPosition(floatPos);
}

void RenderableObject::draw(sf::RenderWindow& window, float deltaTime)
{	
	if (visible)
	{
		window.draw(sprite);
	}
}

Item::~Item() {

}

void Armament::debugPrint() const
{
	std::cout << "Armament: " << std::endl;
	std::cout << "name = " << name << std::endl;
	std::cout << "itemClass = " << itemClass << std::endl;
	std::cout << "material = " << material << std::endl;
	std::cout << "armor = " << armor << std::endl;
	std::cout << "speed = " << speed << std::endl;
	std::cout << "bonus = " << bonus << std::endl;
	std::cout << "armamentType = " << EnumMapper::mapArmamentTypeNames(armamentType) << std::endl;
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
	std::cout << "max = " << maxWeapons << std::endl;
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

void Room::initialize(unsigned int width, unsigned int height)
{
	// null-initialize overlay and background
	for (unsigned int y = 0; y < height; y++) //rows
	{
		std::vector<std::shared_ptr<RenderableObject>> overlayRow;
		std::vector<std::shared_ptr<RenderableObject>> backgroundRow;
		for (unsigned int x = 0; x < width; x++) //cols
		{
			overlayRow.push_back(nullptr);
			backgroundRow.push_back(nullptr);
		}
		overlay.push_back(overlayRow);
		background.push_back(backgroundRow);
	}
}

void Creature::resetOriginals()
{
	this->hp = this->orHp;
	this->accuracy = this->orAccuracy;
	this->speed = this->orSpeed;
	this->strength = this->orStrength;
}

void Creature::removeActivePotions(std::shared_ptr<Potion> potion_)
{
	activePotions.erase(std::remove(activePotions.begin(), activePotions.end(), potion_), activePotions.end());
}

void Creature::addActivePotion(std::shared_ptr<Potion> potion_)
{
	activePotions.push_back(potion_);
}

