#include "gameObjects.hpp"
#include "map.hpp"
#include "OutputFormatter.hpp"
#include "fight.hpp"

float EquipmentSet::getStats(ItemType::Enum, ArmorStatsMode::Enum, ArmorStatsAttributes::Enum)
{
	//implement this!!!

	return 0.f;
}

std::shared_ptr<Weapon> EquipmentSet::setPrimaryWeapon(std::shared_ptr<Weapon> weapon_)
{
	std::shared_ptr<Weapon> oldWeapon = primaryWeapon;

	primaryWeapon = weapon_;

	if (weapon_ != nullptr) // position new weapon
	{
		if (weapon_->getName() == "Shield")
		{
			ResourceManager::getInstance().getSound("error").play();
			OutputFormatter::chat(chatbox, "Shield can only be secondary weapon", sf::Color::White);
			return weapon_;
		}

		primaryWeapon->setSize(itemSize, itemSize);
		primaryWeapon->setPosition(primaryWeaponPos);
	}
	else // passing nullptr only when other weapon is twohander
	{

	}

	if (oldWeapon == fists1) // never return fists
	{
		oldWeapon = nullptr;
	}

	return oldWeapon;
};

std::shared_ptr<Weapon> EquipmentSet::setSecondaryWeapon(std::shared_ptr<Weapon> weapon_)
{
	std::shared_ptr<Weapon> oldWeapon = secondaryWeapon;

	secondaryWeapon = weapon_;

	if (secondaryWeapon != nullptr) // position new weapon
	{
		secondaryWeapon->setSize(itemSize, itemSize);
		secondaryWeapon->setPosition(secondaryWeaponPos);
	}
	else // passing nullptr only when other weapon is twohander
	{

	}

	if (oldWeapon == fists2) // never return fists
	{
		oldWeapon = nullptr;
	}

	return oldWeapon;
};

std::shared_ptr<Armament> EquipmentSet::setHelmet(std::shared_ptr<Armament> helmet_)
{
	std::shared_ptr<Armament> oldArmament = helmet;

	helmet = helmet_;

	if (helmet != nullptr)
	{
		helmet->setSize(itemSize, itemSize);
		helmet->setPosition(helmetPos);
	}

	return oldArmament;
};

std::shared_ptr<Armament> EquipmentSet::setHarness(std::shared_ptr<Armament> harness_)
{
	std::shared_ptr<Armament> oldArmament = harness;

	harness = harness_;

	if (harness != nullptr)
	{
		harness->setSize(itemSize, itemSize);
		harness->setPosition(harnessPos);
	}

	return oldArmament;
};

std::shared_ptr<Armament> EquipmentSet::setCuisse(std::shared_ptr<Armament> cuisse_)
{
	std::shared_ptr<Armament> oldArmament = cuisse;

	cuisse = cuisse_;

	if (cuisse != nullptr)
	{
		cuisse->setSize(itemSize, itemSize);
		cuisse->setPosition(cuissePos);
	}

	return oldArmament;
};

std::shared_ptr<Armament> EquipmentSet::setGauntlets(std::shared_ptr<Armament> gauntlets_)
{
	std::shared_ptr<Armament> oldArmament = gauntlets;

	gauntlets = gauntlets_;

	if (gauntlets != nullptr)
	{
		gauntlets->setSize(itemSize, itemSize);
		gauntlets->setPosition(gauntletsPos);
	}

	return oldArmament;
};

std::shared_ptr<Armament> EquipmentSet::setBoots(std::shared_ptr<Armament> boots_)
{
	std::shared_ptr<Armament> oldArmament = boots;

	boots = boots_;

	if (boots != nullptr)
	{
		boots->setSize(itemSize, itemSize);
		boots->setPosition(bootsPos);
	}

	return oldArmament;
};

std::shared_ptr<Potion> EquipmentSet::setPotion1(std::shared_ptr<Potion> potion_)
{
	std::shared_ptr<Potion> oldPotion = potion1;

	potion1 = potion_;

	if (potion1 != nullptr)
	{
		potion1->setSize(itemSize, itemSize);
		potion1->setPosition(potion1Pos);
		ResourceManager::getInstance().getSound(potion1->getName()).play();
	}

	return oldPotion;
};

std::shared_ptr<Potion> EquipmentSet::setPotion2(std::shared_ptr<Potion> potion_)
{
	std::shared_ptr<Potion> oldPotion = potion2;

	potion2 = potion_;

	if (potion2 != nullptr)
	{
		potion2->setSize(itemSize, itemSize);
		potion2->setPosition(potion2Pos);
		ResourceManager::getInstance().getSound(potion2->getName()).play();
	}

	return oldPotion;
};

std::shared_ptr<Potion> EquipmentSet::setPotion3(std::shared_ptr<Potion> potion_)
{
	std::shared_ptr<Potion> oldPotion = potion3;

	potion3 = potion_;

	if (potion3 != nullptr)
	{
		potion3->setSize(itemSize, itemSize);
		potion3->setPosition(potion3Pos);
		ResourceManager::getInstance().getSound(potion3->getName()).play();
	}

	return oldPotion;
};

void EquipmentSet::setFists(std::shared_ptr<Weapon> fists1_, std::shared_ptr<Weapon> fists2_)
{
	fists1 = fists1_;
	fists2 = fists2_;

	fists1->setCenteredPosition(sf::Vector2i(static_cast<int>(primaryWeaponPos.x + itemSize * 0.5f), static_cast<int>(primaryWeaponPos.y + itemSize * 0.5f)));
	fists2->setCenteredPosition(sf::Vector2i(static_cast<int>(secondaryWeaponPos.x + itemSize * 0.5f), static_cast<int>(secondaryWeaponPos.y + itemSize * 0.5f)));
	fists1->setSize(itemSize, itemSize);
	fists2->setSize(itemSize, itemSize);

	primaryWeapon = fists1;
	secondaryWeapon = fists2;
}

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

				if (secondaryWeapon == nullptr)
				{
					retArm2 = setSecondaryWeapon(fists2);
				}
				else if (secondaryWeapon->getSlots() == 2)
				{
					retArm2 = setSecondaryWeapon(fists2);
				}
			}
			else if (weapon->getSlots() == 2)
			{
				retArm1 = setPrimaryWeapon(weapon);
				retArm2 = setSecondaryWeapon(nullptr);
			}
		}
		else if (hotspot == EquipHotspots::RIGHT)
		{
			if (weapon->getSlots() == 1)
			{
				retArm1 = setSecondaryWeapon(weapon);

				if (primaryWeapon == nullptr)
				{
					retArm2 = setPrimaryWeapon(fists1);
				}
				else if (primaryWeapon->getSlots() == 2)
				{
					retArm2 = setPrimaryWeapon(fists1);
				}
			}
			else if (weapon->getSlots() == 2)
			{
				retArm1 = setPrimaryWeapon(nullptr);
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

void EquipmentSet::setGeometry(sf::Vector2f armorOffsets_, sf::Vector2f potionOffsets_, sf::Vector2f armorDims_, sf::Vector2f potionDims_, int itemSize_) {

	armorOffsets = armorOffsets_;
	potionOffsets = potionOffsets_;
	armorDims = armorDims_;
	potionDims = potionDims_;
	itemSize = itemSize_;

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

std::shared_ptr<RenderableObject> EquipmentSet::getItemAtPixels(sf::Vector2i pos, bool remove, bool usePotion)
{
	std::shared_ptr<RenderableObject> retObj = nullptr;

	std::cout << "called getItemAtPixels with remove " << remove << std::endl;

	if (!usePotion)
	{
		if (pos.x >= primaryWeaponPos.x && pos.y >= primaryWeaponPos.y && pos.x <= (primaryWeaponPos.x + itemSize) && pos.y <= (primaryWeaponPos.y + itemSize))
		{
			retObj = primaryWeapon;

			if (remove)
			{
				if (retObj == fists1)
				{
					retObj = nullptr;
				}

				if (primaryWeapon->getSlots() == 2)
				{
					secondaryWeapon = fists2;
				}

				// primaryWeapon = nullptr;
				primaryWeapon = fists1;
			}
		}
		else if (pos.x >= secondaryWeaponPos.x && pos.y >= secondaryWeaponPos.y && pos.x <= (secondaryWeaponPos.x + itemSize) && pos.y <= (secondaryWeaponPos.y + itemSize))
		{
			retObj = secondaryWeapon;

			if (remove)
			{
				if (retObj == fists2)
				{
					retObj = nullptr;
				}

				if (secondaryWeapon->getSlots() == 2)
				{
					primaryWeapon = fists1;
				}

				// secondaryWeapon = nullptr;
				secondaryWeapon = fists2;
			}
		}
		else if (pos.x >= bootsPos.x && pos.y >= bootsPos.y && pos.x <= (bootsPos.x + itemSize) && pos.y <= (bootsPos.y + itemSize))
		{
			retObj = boots;
			if (remove)
			{
				boots = nullptr;
			}
		}
		else if (pos.x >= cuissePos.x && pos.y >= cuissePos.y && pos.x <= (cuissePos.x + itemSize) && pos.y <= (cuissePos.y + itemSize))
		{
			retObj = cuisse;
			if (remove)
			{
				cuisse = nullptr;
			}
		}
		else if (pos.x >= gauntletsPos.x && pos.y >= gauntletsPos.y && pos.x <= (gauntletsPos.x + itemSize) && pos.y <= (gauntletsPos.y + itemSize))
		{
			retObj = gauntlets;
			if (remove)
			{
				gauntlets = nullptr;
			}
		}
		else if (pos.x >= harnessPos.x && pos.y >= harnessPos.y && pos.x <= (harnessPos.x + itemSize) && pos.y <= (harnessPos.y + itemSize))
		{
			retObj = harness;
			if (remove)
			{
				harness = nullptr;
			}
		}
		else if (pos.x >= helmetPos.x && pos.y >= helmetPos.y && pos.x <= (helmetPos.x + itemSize) && pos.y <= (helmetPos.y + itemSize))
		{
			retObj = helmet;
			if (remove)
			{
				helmet = nullptr;
			}
		}
	}
	if (pos.x >= potion1Pos.x && pos.y >= potion1Pos.y && pos.x <= (potion1Pos.x + itemSize) && pos.y <= (potion1Pos.y + itemSize))
	{
		retObj = potion1;
		if (remove)
		{
			potion1 = nullptr;
		}
	}
	else if (pos.x >= potion2Pos.x && pos.y >= potion2Pos.y && pos.x <= (potion2Pos.x + itemSize) && pos.y <= (potion2Pos.y + itemSize))
	{
		retObj = potion2;
		if (remove)
		{
			potion2 = nullptr;
		}
	}
	else if (pos.x >= potion3Pos.x && pos.y >= potion3Pos.y && pos.x <= (potion3Pos.x + itemSize) && pos.y <= (potion3Pos.y + itemSize))
	{
		retObj = potion3;
		if (remove)
		{
			potion3 = nullptr;
		}
	}

	return retObj;
}

std::list<std::shared_ptr<RenderableObject>> EquipmentSet::setItemAtPixels(sf::Vector2i pos, std::shared_ptr<RenderableObject> obj, bool usePotion, std::shared_ptr<Fight> fight)
{
	std::list<std::shared_ptr<RenderableObject>> retObjs;
	EquipHotspots::Enum hotspot = EquipHotspots::UNKNOWN;

	std::cout << "pos: x =  " << pos.x << ", y = " << pos.y << std::endl;
	std::cout << "armorBounds: left =  " << armorOffsets.x << ", right = " << (armorOffsets.x + armorDims.x) << ", top = " << armorOffsets.y << ", bottom = " << (armorOffsets.y + armorDims.y) << std::endl;
	std::cout << "potionBounds: left =  " << potionOffsets.x << ", right = " << (potionOffsets.x + potionDims.x) << ", top = " << potionOffsets.y << ", bottom = " << (potionOffsets.y + potionDims.y) << std::endl;

	if (pos.x >= armorOffsets.x && pos.x <= (armorOffsets.x + armorDims.x) && pos.y >= armorOffsets.y && pos.y <= (armorOffsets.y + armorDims.y)) //inside armor
	{
		if (pos.x >= armorOffsets.x + (armorDims.x * 0.5f)){
			hotspot = EquipHotspots::RIGHT;
			std::cout << "right" << std::endl;
		}
		else
		{
			hotspot = EquipHotspots::LEFT;
			std::cout << "left" << std::endl;
		}
	}
	if (pos.x >= potionOffsets.x && pos.x <= (potionOffsets.x + potionDims.x) && pos.y >= potionOffsets.y && pos.y <= (potionOffsets.y + potionDims.y)) //inside potions
	{
		if (pos.x >= potionOffsets.x && pos.x < potionOffsets.x + (potionDims.x * 0.33f)){
			hotspot = EquipHotspots::POTION3;
			std::cout << "potion1" << std::endl;
		}
		else if (pos.x >= potionOffsets.x + (potionDims.x * 0.33f) && pos.x < potionOffsets.x + (potionDims.x * 0.66f))
		{
			hotspot = EquipHotspots::POTION2;
			std::cout << "potion2" << std::endl;
		}
		else if (pos.x >= potionOffsets.x + (potionDims.x * 0.66f) && pos.x < potionOffsets.x + potionDims.x)
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
		if (usePotion) // drink a potion
		{
			std::shared_ptr<Potion> potion = std::dynamic_pointer_cast<Potion>(item);

			// add to active Potions list
			//fight->(potion)...
			if (fight != nullptr)
			{
				fight->addPotionToBeUsed(potion);
				fight->fightRound(Attacks::POTION, 1u);
			}
		}
		else
		{
			retObjs.push_back(obj);
		}
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

void Player::init(Map* map_, const unsigned int tileSize_, tgui::ChatBox::Ptr chatBox_)
{
	map = map_;
	tileSize = tileSize_;
	chatBox = chatBox_;

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
					ResourceManager::getInstance().getSound("footsteps").play();
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

std::shared_ptr<Monster> Player::handleInput(sf::Event event, std::shared_ptr<RenderableObject> draggedItem)
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
				{
					std::shared_ptr<Creature> creature = std::dynamic_pointer_cast<Creature>(object);

					if (creature->getCreatureType() == CreatureType::MONSTER)
					{
						std::shared_ptr<Monster> monster = std::dynamic_pointer_cast<Monster>(creature);
						return monster;
					}

					break;
				}
				default:
					break;
				}
			}
		}
	}

	return nullptr;
}

std::shared_ptr<RenderableObject> Player::putInInventory(std::shared_ptr<RenderableObject> object, bool output)
{
	if (inventory.size() >= maxInventorySize)
	{
		if (output)
		{
			ResourceManager::getInstance().getSound("error").play();
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

		ResourceManager::getInstance().getSound("putInInventory").play();

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

void Player::setEquipmentFists(std::shared_ptr<Weapon> fists1, std::shared_ptr<Weapon> fists2)
{
	setOne->setFists(fists1, fists2);
	setTwo->setFists(fists1, fists2);
}

void Player::setPosition(Point position_)
{
	prevPlayerPosition = playerPosition;
	playerPosition = position_;
}

void Player::setEquipmentGeometry(sf::Vector2f armorOffsets, sf::Vector2f potionOffsets, sf::Vector2f armorDims_, sf::Vector2f potionDims_, int itemSize_)
{
	setOne->setGeometry(armorOffsets, potionOffsets, armorDims_, potionDims_, itemSize_);
	setTwo->setGeometry(armorOffsets, potionOffsets, armorDims_, potionDims_, itemSize_);
}

std::shared_ptr<RenderableObject> Player::getInventoryItemAtPixels(sf::Vector2i pos, bool remove)
{
	sf::Vector2i relativePos;

	relativePos.x = static_cast<int>(pos.x - inventoryLeftPadding);
	relativePos.y = static_cast<int>(pos.y - inventoryTopPadding);

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

void Player::setPlayerPosition(Point position_)
{
	prevPlayerPosition = position_;
	playerPosition = prevPlayerPosition;
	facingDir = ViewingDirections::N;
	setRotation(0.f);
	RenderableObject::setPosition(sf::Vector2f(static_cast<float>(playerPosition.x * tileSize), static_cast<float>(playerPosition.y * tileSize)));
}
