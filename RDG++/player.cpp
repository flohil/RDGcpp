#include "gameObjects.hpp"
#include "map.hpp"
#include "OutputFormatter.hpp"

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
	primaryWeapon->setPosition(primaryWeaponPos);

	return oldWeapon;
};

std::shared_ptr<Weapon> EquipmentSet::setSecondaryWeapon(std::shared_ptr<Weapon> weapon_)
{
	std::shared_ptr<Weapon> oldWeapon = secondaryWeapon;

	secondaryWeapon = weapon_;
	secondaryWeapon->setSize(itemSize, itemSize);
	secondaryWeapon->setPosition(secondaryWeaponPos);

	return oldWeapon;
};

std::shared_ptr<Armament> EquipmentSet::setHelmet(std::shared_ptr<Armament> helmet_)
{
	std::shared_ptr<Armament> oldArmament = helmet;

	helmet = helmet_;
	helmet->setSize(itemSize, itemSize);
	helmet->setPosition(helmetPos);

	return oldArmament;
};

std::shared_ptr<Armament> EquipmentSet::setHarness(std::shared_ptr<Armament> harness_)
{
	std::shared_ptr<Armament> oldArmament = harness;

	harness = harness_;
	harness->setSize(itemSize, itemSize);
	harness->setPosition(harnessPos);

	return oldArmament;
};

std::shared_ptr<Armament> EquipmentSet::setCuisse(std::shared_ptr<Armament> cuisse_)
{
	std::shared_ptr<Armament> oldArmament = cuisse;

	cuisse = cuisse_;
	cuisse->setSize(itemSize, itemSize);
	cuisse->setPosition(cuissePos);


	return oldArmament;
};

std::shared_ptr<Armament> EquipmentSet::setGauntlets(std::shared_ptr<Armament> gauntlets_)
{
	std::shared_ptr<Armament> oldArmament = gauntlets;

	gauntlets = gauntlets_;
	gauntlets->setSize(itemSize, itemSize);
	gauntlets->setPosition(gauntletsPos);

	return oldArmament;
};

std::shared_ptr<Armament> EquipmentSet::setBoots(std::shared_ptr<Armament> boots_)
{
	std::shared_ptr<Armament> oldArmament = boots;

	boots = boots_;
	boots->setSize(itemSize, itemSize);
	boots->setPosition(bootsPos);

	return oldArmament;
};

std::shared_ptr<Potion> EquipmentSet::setPotion1(std::shared_ptr<Potion> potion_)
{
	std::shared_ptr<Potion> oldPotion = potion1;

	potion1 = potion_;
	potion1->setSize(itemSize, itemSize);
	potion1->setPosition(potion1Pos);

	return oldPotion;
};

std::shared_ptr<Potion> EquipmentSet::setPotion2(std::shared_ptr<Potion> potion_)
{
	std::shared_ptr<Potion> oldPotion = potion2;

	potion2 = potion_;
	potion2->setSize(itemSize, itemSize);
	potion2->setPosition(potion2Pos);

	return oldPotion;
};

std::shared_ptr<Potion> EquipmentSet::setPotion3(std::shared_ptr<Potion> potion_)
{
	std::shared_ptr<Potion> oldPotion = potion3;

	potion3 = potion_;
	potion3->setSize(itemSize, itemSize);
	potion3->setPosition(potion3Pos);

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

void EquipmentSet::setOffsets(sf::Vector2f armorOffsets_, sf::Vector2f potionOffsets_, sf::Vector2f armorDims_, sf::Vector2f potionDims_, int horSplitAbs_, int verRightSplitAbs_) {

	armorOffsets = armorOffsets_;
	potionOffsets = potionOffsets_;
	armorDims = armorDims_;
	potionDims = potionDims_;
	horSplitAbs = horSplitAbs_;
	verRightSplitAbs = verRightSplitAbs_;

	std::cout << "armorDims: " << armorDims.x << " x " << armorDims.y << std::endl;
	std::cout << "potionDims: " << potionDims.x << " x " << potionDims.y << std::endl;

	primaryWeaponPos = sf::Vector2f(armorOffsets.x + 12.f, armorOffsets.y + 110.f);
	secondaryWeaponPos = sf::Vector2f(armorOffsets.x + 196.f, armorOffsets.y + 110.f);
	helmetPos = sf::Vector2f(armorOffsets.x + 196.f, armorOffsets.y + 12.f);
	harnessPos = sf::Vector2f(armorOffsets.x + 196.f, armorOffsets.y + 59.f);
	cuissePos = sf::Vector2f(armorOffsets.x + 12.f, armorOffsets.y + 178.f);
	gauntletsPos = sf::Vector2f(armorOffsets.x + 12.f, armorOffsets.y + 42.f);
	bootsPos = sf::Vector2f(armorOffsets.x + 196.f, armorOffsets.y + 207.f);
	potion1Pos = sf::Vector2f(potionOffsets.x + 178.f, potionOffsets.y + 2.f);
	potion2Pos = sf::Vector2f(potionOffsets.x + 105.f, potionOffsets.y + 2.f);
	potion3Pos = sf::Vector2f(potionOffsets.x + 33.f, potionOffsets.y + 2.f);
}

std::shared_ptr<RenderableObject> EquipmentSet::getItemAtPixels(sf::Vector2i pos, bool remove)
{
	sf::Vector2i relPos;
	std::shared_ptr<RenderableObject> retObj = nullptr;

	relPos.x = pos.x - horSplitAbs;
	relPos.y = pos.y;

	if (relPos.x >= primaryWeaponPos.x && relPos.y >= primaryWeaponPos.y && relPos.x <= (primaryWeaponPos.x + itemSize) && relPos.y <= (primaryWeaponPos.y + itemSize))
	{
		retObj = primaryWeapon;
		if (remove)
		{
			primaryWeapon = nullptr;
		}
	}
	else if (relPos.x >= secondaryWeaponPos.x && relPos.y >= secondaryWeaponPos.y && relPos.x <= (secondaryWeaponPos.x + itemSize) && relPos.y <= (secondaryWeaponPos.y + itemSize))
	{
		retObj = secondaryWeapon;
		if (remove)
		{
			secondaryWeapon = nullptr;
		}
	}
	else if (relPos.x >= bootsPos.x && relPos.y >= bootsPos.y && relPos.x <= (bootsPos.x + itemSize) && relPos.y <= (bootsPos.y + itemSize))
	{
		retObj = boots;
		if (remove)
		{
			boots = nullptr;
		}
	}
	else if (relPos.x >= cuissePos.x && relPos.y >= cuissePos.y && relPos.x <= (cuissePos.x + itemSize) && relPos.y <= (cuissePos.y + itemSize))
	{
		retObj = cuisse;
		if (remove)
		{
			cuisse = nullptr;
		}
	}
	else if (relPos.x >= gauntletsPos.x && relPos.y >= gauntletsPos.y && relPos.x <= (gauntletsPos.x + itemSize) && relPos.y <= (gauntletsPos.y + itemSize))
	{
		retObj = gauntlets;
		if (remove)
		{
			gauntlets = nullptr;
		}
	}
	else if (relPos.x >= harnessPos.x && relPos.y >= harnessPos.y && relPos.x <= (harnessPos.x + itemSize) && relPos.y <= (harnessPos.y + itemSize))
	{
		retObj = harness;
		if (remove)
		{
			harness = nullptr;
		}
	}
	else if (relPos.x >= helmetPos.x && relPos.y >= helmetPos.y && relPos.x <= (helmetPos.x + itemSize) && relPos.y <= (helmetPos.y + itemSize))
	{
		retObj = helmet;
		if (remove)
		{
			helmet = nullptr;
		}
	}
	else if (relPos.x >= potion1Pos.x && relPos.y >= potion1Pos.y && relPos.x <= (potion1Pos.x + itemSize) && relPos.y <= (potion1Pos.y + itemSize))
	{
		retObj = potion1;
		if (remove)
		{
			potion1 = nullptr;
		}
	}
	else if (relPos.x >= potion2Pos.x && relPos.y >= potion2Pos.y && relPos.x <= (potion2Pos.x + itemSize) && relPos.y <= (potion2Pos.y + itemSize))
	{
		retObj = potion2;
		if (remove)
		{
			potion2 = nullptr;
		}
	}
	else if (relPos.x >= potion3Pos.x && relPos.y >= potion3Pos.y && relPos.x <= (potion3Pos.x + itemSize) && relPos.y <= (potion3Pos.y + itemSize))
	{
		retObj = potion3;
		if (remove)
		{
			potion3 = nullptr;
		}
	}

	return retObj;
}

std::list<std::shared_ptr<RenderableObject>> EquipmentSet::setItemAtPixels(sf::Vector2i pos, std::shared_ptr<RenderableObject> obj)
{
	std::list<std::shared_ptr<RenderableObject>> retObjs;
	EquipHotspots::Enum hotspot = EquipHotspots::UNKNOWN;

	sf::Vector2f armorOffsetsAbs;
	sf::Vector2f potionOffsetsAbs;

	armorOffsetsAbs.x = armorOffsets.x + horSplitAbs;
	armorOffsetsAbs.y = armorOffsets.y;

	potionOffsetsAbs.x = potionOffsets.x + horSplitAbs;
	potionOffsetsAbs.y = potionOffsets.y;

	std::cout << "pos: x =  " << pos.x << ", y = " << pos.y << std::endl;
	std::cout << "armorBounds: left =  " << armorOffsetsAbs.x << ", right = " << (armorOffsetsAbs.x + armorDims.x) << ", top = " << armorOffsetsAbs.y << ", bottom = " << (armorOffsetsAbs.y + armorDims.y) << std::endl;
	std::cout << "potionBounds: left =  " << potionOffsetsAbs.x << ", right = " << (potionOffsetsAbs.x + potionDims.x) << ", top = " << potionOffsetsAbs.y << ", bottom = " << (potionOffsetsAbs.y + potionDims.y) << std::endl;

	if (pos.x >= armorOffsetsAbs.x && pos.x <= (armorOffsetsAbs.x + armorDims.x) && pos.y >= armorOffsetsAbs.y && pos.y <= (armorOffsetsAbs.y + armorDims.y)) //inside armor
	{
		if (pos.x >= armorOffsetsAbs.x + (armorDims.x * 0.5f)){
			hotspot = EquipHotspots::RIGHT;
			std::cout << "right" << std::endl;
		}
		else
		{
			hotspot = EquipHotspots::LEFT;
			std::cout << "left" << std::endl;
		}
	}
	if (pos.x >= potionOffsetsAbs.x && pos.x <= (potionOffsetsAbs.x + potionDims.x) && pos.y >= potionOffsetsAbs.y && pos.y <= (potionOffsetsAbs.y + potionDims.y)) //inside potions
	{
		if (pos.x >= potionOffsetsAbs.x && pos.x < potionOffsetsAbs.x + (potionDims.x * 0.33f)){
			hotspot = EquipHotspots::POTION3;
			std::cout << "potion1" << std::endl;
		}
		else if (pos.x >= potionOffsetsAbs.x + (potionDims.x * 0.33f) && pos.x < potionOffsetsAbs.x + (potionDims.x * 0.66f))
		{
			hotspot = EquipHotspots::POTION2;
			std::cout << "potion2" << std::endl;
		}
		else if (pos.x >= potionOffsetsAbs.x + (potionDims.x * 0.66f) && pos.x < potionOffsetsAbs.x + potionDims.x)
		{
			hotspot = EquipHotspots::POTION1;
			std::cout << "potion3" << std::endl;
		}
	}

	std::cout << "hotspot: " << hotspot << std::endl;

	if (obj->getObjectType() != ObjectType::ITEM || hotspot == EquipHotspots::UNKNOWN)
	{
		std::cout << "do not equip unknown item " << std::endl;
		retObjs.push_back(obj);
		return retObjs; // do not allow key to be dropped in equipment
	}

	std::shared_ptr<Item> item = std::dynamic_pointer_cast<Item>(obj);

	if (item->getItemType() == ItemType::POTION && hotspot != EquipHotspots::POTION1 && hotspot != EquipHotspots::POTION2 && hotspot != EquipHotspots::POTION3) // potion dragged on armament
	{
		std::cout << "dragged into armor" << std::endl;
		retObjs.push_back(obj);
		return retObjs;
	}

	if ((item->getItemType() == ItemType::ARMAMENT || item->getItemType() == ItemType::WEAPON) && hotspot != EquipHotspots::LEFT && hotspot != EquipHotspots::RIGHT)
	{
		std::cout << "dragged into potions" << std::endl;
		retObjs.push_back(obj);
		return retObjs;
	}

	retObjs = setItem(item, hotspot);

	return retObjs;
}

// Player ----------------------------------------------------------------------------------------------------------------------

void Player::init(Map* map_, const unsigned int tileSize_, tgui::ChatBox::Ptr chatBox_, int horSplitAbs_, int rightVerSplitAbs_)
{
	map = map_;
	tileSize = tileSize_;
	chatBox = chatBox_;
	horSplitAbs = horSplitAbs_;
	verRightSplitAbs = rightVerSplitAbs_;

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

void Player::handleInput(sf::Event event, std::shared_ptr<RenderableObject> draggedItem)
{

	if (event.key.code == sf::Keyboard::E)
	{

		std::cout << "facing dir: " << facingDir << std::endl;

		Point facingPoint = playerPosition.getDirPoint(facingDir);
		std::shared_ptr<RenderableObject> object = map->getOverlayObject(facingPoint);

		if (object != nullptr)
		{
			std::cout << "in front of player: " << object->getName() << std::endl;

			if (draggedItem == nullptr)
			{
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
}

std::shared_ptr<RenderableObject> Player::putInInventory(std::shared_ptr<RenderableObject> object, bool output)
{
	if (inventory.size() >= maxInventorySize)
	{
		if (output)
		{
			OutputFormatter::chat(chatBox, "Inventory is full!", sf::Color::White);
		}
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
		if (output)
		{
			OutputFormatter::chat(chatBox, "Picked up " + object->getName(), sf::Color::White);
		}
		return nullptr;
	}
}

void Player::setPositionInInventory(unsigned int idx, std::shared_ptr<RenderableObject> object)
{
	unsigned int colIdx = idx % inventoryHorItemCount;
	unsigned int rowIdx = idx / inventoryHorItemCount;

	object->setPosition(sf::Vector2f(inventoryLeftPadding + colIdx * (inventorySpacing + 2 * tileSize), inventoryTopPadding + rowIdx * (inventorySpacing + 2 * tileSize)));
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

void Player::setEquipmentOffsets(sf::Vector2f armorOffsets, sf::Vector2f potionOffsets, sf::Vector2f armorDims_, sf::Vector2f potionDims_, int horSplitAbs, int verRightSplitAbs)
{
	setOne->setOffsets(armorOffsets, potionOffsets, armorDims_, potionDims_, horSplitAbs, verRightSplitAbs);
	setTwo->setOffsets(armorOffsets, potionOffsets, armorDims_, potionDims_, horSplitAbs, verRightSplitAbs);
}

std::shared_ptr<RenderableObject> Player::getInventoryItemAtPixels(sf::Vector2i pos, bool remove)
{
	sf::Vector2i relativePos;

	relativePos.x = static_cast<int>(pos.x - horSplitAbs - inventoryLeftPadding);
	relativePos.y = static_cast<int>(pos.y - verRightSplitAbs - inventoryTopPadding);

	if (relativePos.x < 0 || relativePos.y < 0)
	{
		return nullptr;
	}

	unsigned int colIdx = relativePos.x / static_cast<unsigned int>(2 * tileSize + inventorySpacing);
	unsigned int rowIdx = relativePos.y / static_cast<unsigned int>(2 * tileSize + inventorySpacing);

	unsigned int itemX = relativePos.x % inventoryHorItemCount;

	if (itemX > 2 * tileSize) // clicked between two items
	{
		return nullptr;
	}

	unsigned int itemY = relativePos.y % inventoryHorItemCount;

	if (itemY > 2 * tileSize) // clicked between two items
	{
		return nullptr;
	}

	if (colIdx >= inventoryHorItemCount)
	{
		return nullptr; // clicked right of allowed items
	}

	if (rowIdx >= maxInventorySize / inventoryHorItemCount)
	{
		return nullptr;
	}

	unsigned int idx = colIdx + inventoryHorItemCount * rowIdx;

	if (idx >= inventory.size())
	{
		return nullptr; // no item at this inventory position
	}

	std::shared_ptr<RenderableObject> retObj = inventory.at(idx);

	if (remove)
	{
		inventory.erase(inventory.begin() + idx);

		for (unsigned int i = idx; i < inventory.size(); i++)
		{
			setPositionInInventory(i, inventory.at(i));
		}
	}

	return retObj;
}