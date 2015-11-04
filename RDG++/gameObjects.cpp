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

void Player::init(Map* map_, const unsigned int tileSize_, tgui::ChatBox::Ptr chatBox_)
{
	map = map_;
	tileSize = tileSize_;
	chatBox = chatBox_;

	Point point = map->initPlayerPosition();
	prevPlayerPosition = point;
	playerPosition = prevPlayerPosition;
	RenderableObject::setPosition(sf::Vector2f(static_cast<float>(playerPosition.x * tileSize), static_cast<float>(playerPosition.y * tileSize)));

	setOne->setItemSize(tileSize);
	setTwo->setItemSize(tileSize);
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
				switch (lastDir)
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

		/* switch (intendedMovDir)
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

		setRotation(picAngle); */

		// enable to only turn player when moving direction changes or when standing still
		if (initialWait)
		{
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
		}
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

		std::cout << "facing dir: " << facingDir << std::endl;

		Point facingPoint = playerPosition.getDirPoint(facingDir);
		std::shared_ptr<RenderableObject> object = map->getOverlayObject(facingPoint);
		
		if (object != nullptr)
		{
			std::cout << "in front of player: " << object->getName() << std::endl;

			switch (object->getObjectType())
			{
				case ObjectType::KEY:
					OutputFormatter::chat(chatBox, "Found key for treasure chamber", sf::Color::White);
					map->setOverlayObject(facingPoint, putInInventory(object));
					break;
				case ObjectType::ITEM:
					map->setOverlayObject(facingPoint, putInInventory(object));
					break;
				case ObjectType::CREATURE:
					OutputFormatter::chat(chatBox, "Started fight against " + object->getName(), sf::Color::White);
					//startFight(this, std::dynamic_pointer_cast<Creature>(object));
					break;
				default:
					break;
			}
		}
	}
}

std::shared_ptr<RenderableObject> Player::putInInventory(std::shared_ptr<RenderableObject> object)
{
	if (inventory.size() >= maxInventorySize)
	{
		OutputFormatter::chat(chatBox, "Inventory is full!", sf::Color::White);
		return object;
	}
	else
	{
		if (object->getObjectType() == ObjectType::KEY)
		{
			map->openTreasureChamber();
		}
		object->setSize(tileSize * 2, tileSize * 2);
		setPositionInInventory(inventory.size(), object);
		inventory.push_back(object);
		OutputFormatter::chat(chatBox, "Picked up " + object->getName(), sf::Color::White);
		return nullptr;
	}
}

void Player::setPositionInInventory(unsigned int idx, std::shared_ptr<RenderableObject> object)
{
	float leftPadding = 17.f;
	float topPadding = 12.f;
	float spacing = 5.f;
	unsigned int horItemCount = 3u;

	unsigned int colIdx = idx % horItemCount;
	unsigned int rowIdx = idx / horItemCount;

	object->setPosition(sf::Vector2f(leftPadding + colIdx * (spacing + 2 * tileSize), topPadding + rowIdx * (spacing + 2 * tileSize)));
}

void Player::drawInventory(sf::RenderWindow& window, const float deltaTime) const
{
	for (unsigned int i = 0; i < inventory.size(); i++)
	{
		inventory[i]->draw(window, deltaTime);
	}
}

void Player::drawEquipment(sf::RenderWindow& window, const float deltaTime) const
{
	std::shared_ptr<EquipmentSet> set = getEquipmentSet();

	if (set->getHelmet() != nullptr)
	{
		set->getHelmet()->draw(window, deltaTime);
	}
	if (set->getHarness() != nullptr)
	{
		set->getHarness()->draw(window, deltaTime);
	}
	if (set->getCuisse() != nullptr)
	{
		set->getCuisse()->draw(window, deltaTime);
	}
	if (set->getGauntlets() != nullptr)
	{
		set->getGauntlets()->draw(window, deltaTime);
	}
	if (set->getBoots() != nullptr)
	{
		set->getBoots()->draw(window, deltaTime);
	}
	if (set->getPrimaryWeapon() != nullptr)
	{
		set->getPrimaryWeapon()->draw(window, deltaTime);
	}
	if (set->getSecondaryWeapon() != nullptr)
	{
		set->getSecondaryWeapon()->draw(window, deltaTime);
	}
	if (set->getPotion1() != nullptr)
	{
		set->getPotion1()->draw(window, deltaTime);
	}
	if (set->getPotion2() != nullptr)
	{
		set->getPotion2()->draw(window, deltaTime);
	}
	if (set->getPotion3() != nullptr)
	{
		set->getPotion3()->draw(window, deltaTime);
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

std::shared_ptr<Weapon> EquipmentSet::setPrimaryWeapon(std::shared_ptr<Weapon> weapon_)
{ 
	std::shared_ptr<Weapon> oldWeapon = primaryWeapon;

	primaryWeapon = weapon_;
	primaryWeapon->setSize(itemSize, itemSize);
	primaryWeapon->setPosition(sf::Vector2f(armorOffsets.x + 12.f, armorOffsets.y + 110.f));

	return oldWeapon;
};

std::shared_ptr<Weapon> EquipmentSet::setSecondaryWeapon(std::shared_ptr<Weapon> weapon_)
{ 
	std::shared_ptr<Weapon> oldWeapon = secondaryWeapon;

	secondaryWeapon = weapon_;
	secondaryWeapon->setSize(itemSize, itemSize);
	secondaryWeapon->setPosition(sf::Vector2f(armorOffsets.x + 196.f, armorOffsets.y + 110.f));

	return oldWeapon;
};

std::shared_ptr<Armament> EquipmentSet::setHelmet(std::shared_ptr<Armament> helmet_) 
{ 
	std::shared_ptr<Armament> oldArmament = helmet;

	helmet = helmet_; 
	helmet->setSize(itemSize, itemSize);
	helmet->setPosition(sf::Vector2f(armorOffsets.x + 196.f, armorOffsets.y + 12.f));

	return oldArmament;
};

std::shared_ptr<Armament> EquipmentSet::setHarness(std::shared_ptr<Armament> harness_)
{ 
	std::shared_ptr<Armament> oldArmament = harness;

	harness = harness_; 
	harness->setSize(itemSize, itemSize);
	harness->setPosition(sf::Vector2f(armorOffsets.x + 196.f, armorOffsets.y + 59.f));

	return oldArmament;
};

std::shared_ptr<Armament> EquipmentSet::setCuisse(std::shared_ptr<Armament> cuisse_)
{ 
	std::shared_ptr<Armament> oldArmament = cuisse;

	cuisse = cuisse_; 
	cuisse->setSize(itemSize, itemSize);
	cuisse->setPosition(sf::Vector2f(armorOffsets.x + 12.f, armorOffsets.y + 178.f));


	return oldArmament;
};

std::shared_ptr<Armament> EquipmentSet::setGauntlets(std::shared_ptr<Armament> gauntlets_)
{ 
	std::shared_ptr<Armament> oldArmament = gauntlets;

	gauntlets = gauntlets_; 
	gauntlets->setSize(itemSize, itemSize);
	gauntlets->setPosition(sf::Vector2f(armorOffsets.x + 12.f, armorOffsets.y + 42.f));

	return oldArmament;
};

std::shared_ptr<Armament> EquipmentSet::setBoots(std::shared_ptr<Armament> boots_)
{ 
	std::shared_ptr<Armament> oldArmament = boots;

	boots = boots_; 
	boots->setSize(itemSize, itemSize);
	boots->setPosition(sf::Vector2f(armorOffsets.x + 196.f, armorOffsets.y + 207.f));

	return oldArmament;
};

std::shared_ptr<Potion> EquipmentSet::setPotion1(std::shared_ptr<Potion> potion_)
{ 
	std::shared_ptr<Potion> oldPotion = potion1;

	potion1 = potion_; 
	potion1->setSize(itemSize, itemSize);
	potion1->setPosition(sf::Vector2f(potionOffsets.x + 178.f, potionOffsets.y + 2.f));

	return oldPotion;
};

std::shared_ptr<Potion> EquipmentSet::setPotion2(std::shared_ptr<Potion> potion_)
{ 
	std::shared_ptr<Potion> oldPotion = potion2;

	potion2 = potion_; 
	potion2->setSize(itemSize, itemSize);
	potion2->setPosition(sf::Vector2f(potionOffsets.x + 105.f, potionOffsets.y + 2.f));

	return oldPotion;
};

std::shared_ptr<Potion> EquipmentSet::setPotion3(std::shared_ptr<Potion> potion_)
{ 
	std::shared_ptr<Potion> oldPotion = potion3;

	potion3 = potion_; 
	potion3->setSize(itemSize, itemSize);
	potion3->setPosition(sf::Vector2f(potionOffsets.x + 33.f, potionOffsets.y + 2.f));

	return oldPotion;
};

std::list<std::shared_ptr<RenderableObject>> EquipmentSet::setItem(std::shared_ptr<Item> obj, EquipHotspots::Enum hotspot)
{
	std::list<std::shared_ptr<RenderableObject>> retList;

	if (obj == nullptr)
	{
		return retList;
	}

	switch (obj->getItemType())
	{
		case ItemType::ARMAMENT:
		{
			if (hotspot == EquipHotspots::LEFT || hotspot == EquipHotspots::RIGHT)
			{
				std::shared_ptr<Armament> retArm = std::dynamic_pointer_cast<Armament>(obj);

				switch (retArm->getArmamentType())
				{
					case ArmamentType::BOOTS:
					{
						retArm = setBoots(retArm);
						break;
					}
					case ArmamentType::CUISSE:
					{
						retArm = setCuisse(retArm);
						break;
					}
					case ArmamentType::GAUNTLETS:
					{
						retArm = setGauntlets(retArm);
						break;
					}
					case ArmamentType::HARNESS:
					{
						retArm = setHarness(retArm);
						break;
					}
					case ArmamentType::HELMET:
					{
						retArm = setHelmet(retArm);
						break;
					}
					default:
					{
						break;
					}
				}

				if (retArm != nullptr)
				{
					retList.push_back(retArm);
				}
			}
			break;
		}
		case ItemType::WEAPON:
		{
			std::shared_ptr<Weapon> weapon = std::dynamic_pointer_cast<Weapon>(obj);
			std::shared_ptr<Weapon> retArm1 = nullptr;
			std::shared_ptr<Weapon> retArm2 = nullptr;

			if (hotspot == EquipHotspots::LEFT)
			{
				if (weapon->getSlots() == 1)
				{
					retArm1 = setPrimaryWeapon(weapon);

					if (secondaryWeapon != nullptr && secondaryWeapon->getSlots() == 2)
					{
						retArm2 = secondaryWeapon;
					}
				}
				else if (weapon->getSlots() == 2)
				{
					retArm1 = setPrimaryWeapon(weapon);
					retArm2 = secondaryWeapon;
				}
			}
			else if (hotspot == EquipHotspots::RIGHT)
			{
				if (weapon->getSlots() == 1)
				{
					retArm1 = setSecondaryWeapon(weapon);

					if (primaryWeapon != nullptr && primaryWeapon->getSlots() == 2)
					{
						retArm2 = primaryWeapon;
					}
				}
				else if (weapon->getSlots() == 2)
				{
					retArm1 = primaryWeapon;
					retArm2 = setSecondaryWeapon(weapon);
				}
			}

			if (retArm1 != nullptr)
			{
				retList.push_back(retArm1);
			}
			if (retArm2 != nullptr)
			{
				retList.push_back(retArm2);
			}

			break;
		}
		case ItemType::POTION:
		{

			std::shared_ptr<Potion> retPot = std::dynamic_pointer_cast<Potion>(obj);

			if (hotspot == EquipHotspots::POTION1)
			{
				retPot = setPotion1(retPot);
			}
			else if (hotspot == EquipHotspots::POTION2)
			{
				retPot = setPotion2(retPot);
			}
			else if (hotspot == EquipHotspots::POTION3)
			{
				retPot = setPotion3(retPot);
			}

			if (retPot != nullptr)
			{
				retList.push_back(retPot);
			}

			break;
		}
		default:
		{
			break;
		}
	}

	return retList;
}

void Player::setEquipmentOffsets(sf::Vector2f armorOffsets, sf::Vector2f potionOffsets)
{
	setOne->setOffsets(armorOffsets, potionOffsets);
	setTwo->setOffsets(armorOffsets, potionOffsets);
}