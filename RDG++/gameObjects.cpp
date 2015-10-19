#include "gameObjects.hpp"
#include "enums.hpp"
#include "resourceManager.hpp"
#include "map.hpp"
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

void RenderableObject::setSize(const unsigned int width, const unsigned int height)
{
	sf::Vector2f scale;

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
	sprite.setScale(scale);
}

void RenderableObject::draw(sf::RenderWindow& window, float deltaTime)
{	
	window.draw(sprite);
}

Item::~Item() {

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

void Player::init(Map* map_)
{
	map = map_;

	Point point = map->initPlayerPosition();
	prevPlayerPosition = point;
	playerPosition = point;
}

void Player::update(const float deltaTime)
{
	// velocity
	float vx = 0.f;
	float vy = 0.f;

	std::cout << movingDirection << std::endl;

	switch (movingDirection)
	{
		case ViewingDirections::N:
			vy = -10.f;
			break;
		case ViewingDirections::E:
			vx = 10.f;
			break;
		case ViewingDirections::S:
			vy = 10.f;
			break;
		case ViewingDirections::W:
			vx = -10.f;
			break;
		default:
			break;
	}

	Point curPos = getPosition();
	curPos.x += vx * deltaTime;
	curPos.y += vy * deltaTime;

	setPosition(curPos);

	movingDirection = ViewingDirections::UNKNOWN;

	setRotation(picAngle);
}

void Player::handleInput(sf::Event event)
{
	if (event.key.code == sf::Keyboard::Up)
	{
		picAngle = 0.f;
		movingDirection = ViewingDirections::N;
	}
	else if (event.key.code == sf::Keyboard::Right)
	{
		picAngle = 90.f;
		movingDirection = ViewingDirections::E;
	}
	else if (event.key.code == sf::Keyboard::Down)
	{
		picAngle = 180.f;
		movingDirection = ViewingDirections::S;
	}
	else if (event.key.code == sf::Keyboard::Left)
	{
		picAngle = 270.f;
		movingDirection = ViewingDirections::W;
	}
}

void Player::setPosition(Point position_)
{
	prevPlayerPosition = playerPosition;
	playerPosition = position_;
}