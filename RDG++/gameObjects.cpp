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

void Player::init(Map* map_, const unsigned int tileSize_)
{
	map = map_;
	tileSize = tileSize_;

	Point point = map->initPlayerPosition();
	prevPlayerPosition = point;
	playerPosition = prevPlayerPosition;
	RenderableObject::setPosition(sf::Vector2f(static_cast<float>(playerPosition.x * tileSize), static_cast<float>(playerPosition.y * tileSize)));
}

// player may only move on tiles, but for smooth movements, player is moved in between tiles
void Player::update(const float deltaTime)
{
	accumulatedTime += deltaTime;

	// preMove stage: turn player, but do not yet move
	if (accumulatedTime > moveInterval)
	{
		preMove();

		if (initialWait && intendedMovDir != ViewingDirections::UNKNOWN)
		{
			initialDelayCtr++;

			if (initialDelayCtr == initialDelaySpan)
			{
				initialDelayCtr = 0;
				initialWait = false;
			}
		}
		
		if (!initialWait)
		{

			if (moveState == MoveState::RESTING)
			{

				Point target = playerPosition.getDirPoint(lastDir);
				bool passable = map->isFieldPassable(target);

				// enable to only turn player when moving direction changes
				/*switch (lastDir)
				{
				case ViewingDirections::N:
					picAngle = 0.f;
					break;
				case ViewingDirections::E:
					picAngle = 90.f;
					break;
				case ViewingDirections::S:
					picAngle = 180.f;
					break;
				case ViewingDirections::W:
					picAngle = 270.f;
					break;
				default:
					break;
				}

				setRotation(picAngle);*/

				if (passable)
				{
					toMove = moveDistance;
					velocity = vFactor;
					moveState = MoveState::MOVING;
					movDir = lastDir;

				}
				else
				{
					toMove = 0;
					velocity = 0;
					lastDir = ViewingDirections::UNKNOWN;
				}
			}
		}		

		lastDir = intendedMovDir;

		accumulatedTime = 0;
	}

	move(deltaTime);
}

void Player::preMove()
{
	ViewingDirections::Enum tempMovDir = ViewingDirections::UNKNOWN;
	unsigned int nbrKeysPressed = 0;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		nbrKeysPressed++;
		tempMovDir = ViewingDirections::N;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		nbrKeysPressed++;
		tempMovDir = ViewingDirections::E;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		nbrKeysPressed++;
		tempMovDir = ViewingDirections::S;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		nbrKeysPressed++;
		tempMovDir = ViewingDirections::W;
	}

	if (nbrKeysPressed == 0)
	{
		waitTillStandingCtr++;
	}
	else if (nbrKeysPressed == 1)
	{
		waitTillStandingCtr = 0;
		intendedMovDir = tempMovDir;

		switch (intendedMovDir)
		{
		case ViewingDirections::N:
			picAngle = 0.f;
			facingDir = ViewingDirections::N;
			break;
		case ViewingDirections::E:
			picAngle = 90.f;
			facingDir = ViewingDirections::E;
			break;
		case ViewingDirections::S:
			picAngle = 180.f;
			facingDir = ViewingDirections::S;
			break;
		case ViewingDirections::W:
			picAngle = 270.f;
			facingDir = ViewingDirections::W;
			break;
		default:
			break;
		}

		setRotation(picAngle);

		// enable to only turn player when moving direction changes or when standing still
		/*if (initialWait)
		{
			switch (intendedMovDir)
			{
			case ViewingDirections::N:
				picAngle = 0.f;
				break;
			case ViewingDirections::E:
				picAngle = 90.f;
				break;
			case ViewingDirections::S:
				picAngle = 180.f;
				break;
			case ViewingDirections::W:
				picAngle = 270.f;
				break;
			default:
				break;
			}

			setRotation(picAngle);
		}*/
	} 
	else
	{
		waitTillStandingCtr++;
		intendedMovDir = movDir;
	}

	if (waitTillStandingCtr >= waitTillStandingSpan)
	{
		waitTillStandingCtr = waitTillStandingSpan;
		intendedMovDir = ViewingDirections::UNKNOWN;
		initialWait = true;
	}
}

void Player::move(const float deltaTime)
{
	float distance = velocity * deltaTime;

	if (distance > toMove)
	{
		distance = toMove;
		toMove = 0;
		velocity = 0;
		moveState = MoveState::ARRIVED;
		//std::cout << "finished movement at: x = " << offset.x << ", " << offset.y << " with distance " << distance << std::endl;
	}
	else
	{
		toMove -= distance;
		/*if (toMove > 0)
		std::cout << "movement left: " << toMove << std::endl;*/
	}

	switch (movDir)
	{
		case ViewingDirections::N:
			offset.y -= distance;
			break;
		case ViewingDirections::E:
			offset.x += distance;
			break;
		case ViewingDirections::S:
			offset.y += distance;
			break;
		case ViewingDirections::W:
			offset.x -= distance;
			break;
	}

	// std::cout << "moveState: " << moveState << std::endl;
		
	//if (movDir != ViewingDirections::UNKNOWN && finishedMove)
	if (moveState == MoveState::ARRIVED)
	{
		//std::cout << "playerPosition: x = " << playerPosition.x << ", y = " <<  playerPosition.y << std::endl;
		setPosition(playerPosition.getDirPoint(movDir));
		//std::cout << "after playerPosition: x = " << playerPosition.x << ", y = " << playerPosition.y << std::endl;
		offset.x = 0;
		offset.y = 0;
		moveState = MoveState::RESTING;
	}

	if (moveState == MoveState::MOVING)
	{
		// std::cout << "offset: x = " << offset.x << ", y = " << offset.y << std::endl;
		float x = playerPosition.x * tileSize + offset.x;
		float y = playerPosition.y * tileSize + offset.y;
		RenderableObject::setPosition(sf::Vector2f(x, y));
	}
}

void Player::handleInput(sf::Event event)
{

	if (event.key.code == sf::Keyboard::E)
	{
		Point facingPoint = playerPosition.getDirPoint(facingDir);
		std::shared_ptr<RenderableObject> object = map->getOverlayObject(facingPoint);
		
		if (object != nullptr)
		{
			std::cout << "in front of player: " << object->getName() << std::endl;

			switch (object->getObjectType())
			{
				case ObjectType::KEY:
					std::cout << "found treasure chamber key " << object->getName() << std::endl;
					if (putInInventar(object))
					{
						map->setOverlayObject(facingPoint, nullptr);
					};
					map->openTreasureChamber();
					break;
				case ObjectType::ITEM:
					std::cout << "putting into inventary " << object->getName() << std::endl;
					if (putInInventar(object))
					{
						map->setOverlayObject(facingPoint, nullptr);
					};
					break;
				case ObjectType::CREATURE:
					std::cout << "starting fight against " << object->getName() << std::endl;
					//startFight(this, std::dynamic_pointer_cast<Creature>(object));
					break;
				default:
					break;
			}
		}
	}
}

bool Player::putInInventar(std::shared_ptr<RenderableObject> object)
{
	if (inventory.size() > maxInventorySize - 1)
	{
		return false;
	}
	else
	{
		inventory.push_back(object);
		std::cout << "inventory now contains " << inventory.size() << " objects " << std::endl;
		return true;
	}
}

void Player::setPosition(Point position_)
{
	prevPlayerPosition = playerPosition;
	playerPosition = position_;
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

float EquipmentSet::getStats(ItemType::Enum, ArmorStatsMode::Enum, ArmorStatsAttributes::Enum)
{
	//implement this!!!

	return 0.f;
}