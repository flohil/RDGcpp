#include "prototypes.hpp"
#include "pugixml.hpp"
#include "settings.hpp"
#include "calculation.hpp"
#include <iostream>
#include <memory>
#include "easylogging++.hpp"
#include "resourceManager.hpp"

std::shared_ptr<Armament> ArmamentTemplate::clone(float externMultiplier)
{
	return std::shared_ptr<Armament>(new Armament(name, itemClass, type, armor * externMultiplier * classMultiplier * Calculation::randomFloat(statsLowMultiplier, statsHighMultiplier),
		speed * externMultiplier * classMultiplier * Calculation::randomFloat(statsLowMultiplier, statsHighMultiplier), bonus));
}

bool ArmamentFactory::importConfig(const std::string& path)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file((const char*)path.c_str());

	LOG(DEBUG) << "importing Monster config";

	if (!result)
	{
		LOG(ERROR) << "Failed parsing " << path;
		return false;
	}
	else
	{
		pugi::xml_node armamentsNode = doc.child("Armaments");

		for (pugi::xml_node armamentNode = armamentsNode.first_child(); armamentNode; armamentNode = armamentNode.next_sibling())
		{

			const std::string& name = armamentNode.child("Name").text().as_string();
			const std::string& image = armamentNode.child("Image").text().as_string();
			const Classes::Enum itemClass = EnumMapper::mapClasses(armamentNode.child("Item_Class").text().as_string());
			const float classMultiplier = armamentNode.child("Class_Multiplier").text().as_float();
			const float statsLowMultiplier = armamentNode.child("Stats_Low_Multiplier").text().as_float();
			const float statsHighMultiplier = armamentNode.child("Stats_High_Multiplier").text().as_float();
			const std::string& type = armamentNode.child("Type").text().as_string();
			const float armor = armamentNode.child("Armor").text().as_float();
			const float speed = armamentNode.child("Speed").text().as_float();
			const float bonus = armamentNode.child("Bonus").text().as_float();

			if (itemClass == Classes::UNKNOWN)
			{
				return false;
			}

			switch (itemClass)
			{
				case Classes::WEAK:
					weakList.push_back(name);
					break;
				case Classes::MEDIUM:
					mediumList.push_back(name);
					break;
				case Classes::STRONG:
					strongList.push_back(name);
					break;
				default:
					break;
			}

			ResourceManager::getInstance().loadTexture(name, image);

			std::shared_ptr<ArmamentTemplate> armament(new ArmamentTemplate(name, image, itemClass, classMultiplier, statsLowMultiplier, statsHighMultiplier, type, armor, speed, bonus));

			objects[armament->getName()] = armament;
		}
	}

	armamentsClassified.insert(std::pair<Classes::Enum, std::list<std::string>>(Classes::WEAK, weakList));
	armamentsClassified.insert(std::pair<Classes::Enum, std::list<std::string>>(Classes::MEDIUM, mediumList));
	armamentsClassified.insert(std::pair<Classes::Enum, std::list<std::string>>(Classes::STRONG, strongList));

	LOG(DEBUG) << "Load result: " << result.description();
	LOG(DEBUG) << "ArmamentTemplate now contains " << objects.size() << " objects.";

	return true;
}

std::shared_ptr<Monster> MonsterTemplate::clone(float externMultiplier)
{
	LOG(DEBUG) << "cloning Monster...";

	return std::shared_ptr<Monster>(new Monster(name, level, killBonusType, classMultiplier * Calculation::randomFloat(killBonusLow, killBonusHigh),
		hp * classMultiplier * Calculation::randomFloat(statsLowMultiplier, statsHighMultiplier), strength * classMultiplier * Calculation::randomFloat(statsLowMultiplier, statsHighMultiplier),
		speed * classMultiplier * Calculation::randomFloat(statsLowMultiplier, statsHighMultiplier), accuracy * classMultiplier * Calculation::randomFloat(statsLowMultiplier, statsHighMultiplier)));
}

bool MonsterFactory::importConfig(const std::string& path)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file((const char*)path.c_str());

	LOG(DEBUG) << "importing Monster config";

	if (!result)
	{
		LOG(ERROR) << "Failed parsing " << path;
		return false;
	}
	else
	{
		pugi::xml_node monstersNode = doc.child("Monsters");

		for (pugi::xml_node monsterNode = monstersNode.first_child(); monsterNode; monsterNode = monsterNode.next_sibling())
		{

			const std::string& name = monsterNode.child("Name").text().as_string();
			const std::string& image = monsterNode.child("Image").text().as_string();
			const DifficultyLevel::Enum level = EnumMapper::mapLevel(monsterNode.child("Level").text().as_string());
			const Attribute::Enum killBonusType = EnumMapper::mapAttribute(monsterNode.child("Kill_Bonus_Type").text().as_string());
			const float classMultiplier = monsterNode.child("Class_Multiplier").text().as_float();
			const float statsLowMultiplier = monsterNode.child("Stats_Low_Multiplier").text().as_float();
			const float statsHighMultiplier = monsterNode.child("Stats_High_Multiplier").text().as_float();
			const float killBonusLow = monsterNode.child("Kill_Bonus_Low").text().as_float();
			const float killBonusHigh = monsterNode.child("Kill_Bonus_High").text().as_float();
			float hp = monsterNode.child("HP").text().as_float();
			float strength = monsterNode.child("Strength").text().as_float();
			float speed = monsterNode.child("Speed").text().as_float();
			float accuracy = monsterNode.child("Accuracy").text().as_float();

			if (level == DifficultyLevel::UNKNOWN || killBonusType == Attribute::UNKNOWN) {
				return false;
			}

			switch (level)
			{
				case DifficultyLevel::EASY:
					easyList.push_back(name);
					break;
				case DifficultyLevel::NORMAL:
					normalList.push_back(name);
					break;
				case DifficultyLevel::HARD:
					hardList.push_back(name);
					break;
				default:
					break;
			}

			ResourceManager::getInstance().loadTexture(name, image);

			std::shared_ptr<MonsterTemplate> monster(new MonsterTemplate(name, image, level, killBonusType, classMultiplier, statsLowMultiplier, statsHighMultiplier, killBonusLow, killBonusHigh, hp, strength, speed, accuracy));

			objects[monster->getName()] = monster;
		}
	}

	monstersLeveled.insert(std::pair<DifficultyLevel::Enum, std::list<std::string>>(DifficultyLevel::EASY, easyList));
	monstersLeveled.insert(std::pair<DifficultyLevel::Enum, std::list<std::string>>(DifficultyLevel::NORMAL, normalList));
	monstersLeveled.insert(std::pair<DifficultyLevel::Enum, std::list<std::string>>(DifficultyLevel::HARD, hardList));

	LOG(DEBUG) << "Load result: " << result.description();
	LOG(DEBUG) << "MonsterTemplate now contains " << objects.size() << " objects.";

	return true;
}

std::shared_ptr<Potion> PotionTemplate::clone(float externMultiplier)
{
	LOG(DEBUG) << "cloning Potion...";

	return std::shared_ptr<Potion>(new Potion(name, itemClass, description, target, effect, mode, strength * externMultiplier * classMultiplier * Calculation::randomFloat(statsLowMultiplier, statsHighMultiplier), duration));
}

bool PotionFactory::importConfig(const std::string& path)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file((const char*)path.c_str());

	LOG(DEBUG) << "importing Potion config";

	if (!result)
	{
		LOG(ERROR) << "Failed parsing " << path;
		return false;
	}
	else
	{
		pugi::xml_node potionsNode = doc.child("Potions");

		for (pugi::xml_node potionNode = potionsNode.first_child(); potionNode; potionNode = potionNode.next_sibling())
		{
			const std::string& name = potionNode.child("Name").text().as_string();
			const std::string& image = potionNode.child("Image").text().as_string();
			const Classes::Enum itemClass = EnumMapper::mapClasses(potionNode.child("Item_Class").text().as_string());
			const float classMultiplier = potionNode.child("Class_Multiplier").text().as_float();
			const float statsLowMultiplier = potionNode.child("Stats_Low_Multiplier").text().as_float();
			const float statsHighMultiplier = potionNode.child("Stats_High_Multiplier").text().as_float();
			const std::string& description = potionNode.child("Description").text().as_string();
			const Target::Enum target = EnumMapper::mapTarget(potionNode.child("Target").text().as_string());
			const Attribute::Enum effect = EnumMapper::mapAttribute(potionNode.child("Effect").text().as_string());
			const Mode::Enum mode = EnumMapper::mapMode(potionNode.child("Mode").text().as_string());
			const float strength = potionNode.child("x").text().as_float();
			const unsigned int duration = potionNode.child("n").text().as_uint();

			if (itemClass == Classes::UNKNOWN || target == Target::UNKNOWN || effect == Attribute::UNKNOWN || mode == Mode::UNKNOWN)
			{
				return false;
			}
			
			switch (itemClass)
			{
				case Classes::WEAK:
					weakList.push_back(name);
					break;
				case Classes::MEDIUM:
					mediumList.push_back(name);
					break;
				case Classes::STRONG:
					strongList.push_back(name);
					break;
				default:
					break;
			}

			ResourceManager::getInstance().loadTexture(name, image);

			std::shared_ptr<PotionTemplate> potion(new PotionTemplate(name, image, itemClass, classMultiplier, statsLowMultiplier, statsHighMultiplier, description, target, effect, mode, strength, duration));

			objects[potion->getName()] = potion;
		}
	}

	potionsClassified.insert(std::pair<Classes::Enum, std::list<std::string>>(Classes::WEAK, weakList));
	potionsClassified.insert(std::pair<Classes::Enum, std::list<std::string>>(Classes::MEDIUM, mediumList));
	potionsClassified.insert(std::pair<Classes::Enum, std::list<std::string>>(Classes::STRONG, strongList));

	LOG(DEBUG) << "Load result: " << result.description();
	LOG(DEBUG) << "PotionTemplate now contains " << objects.size() << " objects.";

	return true;
}

std::shared_ptr<Weapon> WeaponTemplate::clone(float externMultiplier)
{
	LOG(DEBUG) << "cloning Weapon...";

	return std::shared_ptr<Weapon>(new Weapon(name, itemClass, type, attack * externMultiplier * classMultiplier * Calculation::randomFloat(statsLowMultiplier, statsHighMultiplier),
		speed * externMultiplier * classMultiplier * Calculation::randomFloat(statsLowMultiplier, statsHighMultiplier),
		accuracy * externMultiplier * classMultiplier * Calculation::randomFloat(statsLowMultiplier, statsHighMultiplier),
		defence * externMultiplier * classMultiplier * Calculation::randomFloat(statsLowMultiplier, statsHighMultiplier), slots, max));
}

bool WeaponFactory::importConfig(const std::string& path)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file((const char*) path.c_str());

	LOG(DEBUG) << "importing Weapon config";

	if (!result)
	{
		LOG(ERROR) << "Failed parsing " << path;
		return false;
	}
	else
	{
		pugi::xml_node weaponsNode = doc.child("Weapons");

		for (pugi::xml_node weaponNode = weaponsNode.first_child(); weaponNode; weaponNode = weaponNode.next_sibling())
		{

			const std::string& name = weaponNode.child("Name").text().as_string();
			const std::string& image = weaponNode.child("Image").text().as_string();
			const Classes::Enum itemClass = EnumMapper::mapClasses(weaponNode.child("Item_Class").text().as_string());
			const float classMultiplier = weaponNode.child("Class_Multiplier").text().as_float();
			const float statsLowMultiplier = weaponNode.child("Stats_Low_Multiplier").text().as_float();
			const float statsHighMultiplier = weaponNode.child("Stats_High_Multiplier").text().as_float();
			const WeaponType::Enum type = EnumMapper::mapWeaponType(weaponNode.child("Type").text().as_string());
			const float attack = weaponNode.child("Attack").text().as_float();
			const float speed = weaponNode.child("Speed").text().as_float();
			const float accuracy = weaponNode.child("Accuracy").text().as_float();
			const float defence = weaponNode.child("Defence").text().as_float();
			const unsigned int slots = weaponNode.child("Slots").text().as_uint();
			const unsigned int max = weaponNode.child("Max").text().as_uint();

			if (itemClass == Classes::UNKNOWN || type == WeaponType::UNKNOWN)
			{
				return false;
			}

			switch (itemClass)
			{
				case Classes::WEAK:
					weakList.push_back(name);
					break;
				case Classes::MEDIUM:
					mediumList.push_back(name);
					break;
				case Classes::STRONG:
					strongList.push_back(name);
					break;
				default:
					break;
			}

			ResourceManager::getInstance().loadTexture(name, image);

			std::shared_ptr<WeaponTemplate> weapon(new WeaponTemplate(name, image, itemClass, classMultiplier, statsLowMultiplier, statsHighMultiplier,
				type, attack, speed, accuracy, defence, slots, max));

			objects[weapon->getName()] = weapon;
		}
	}

	weaponsClassified.insert(std::pair<Classes::Enum, std::list<std::string>>(Classes::WEAK, weakList));
	weaponsClassified.insert(std::pair<Classes::Enum, std::list<std::string>>(Classes::MEDIUM, mediumList));
	weaponsClassified.insert(std::pair<Classes::Enum, std::list<std::string>>(Classes::STRONG, strongList));

	LOG(DEBUG) << "Load result: " << result.description();
	LOG(DEBUG) << "WeaponTemplate now contains " << objects.size() << " objects.";

	return true;
}

std::shared_ptr<Attack> AttackTemplate::clone(float externMultiplier)
{
	LOG(DEBUG) << "cloning Attack...";

	return std::shared_ptr<Attack>(new Attack(name, effect, hpDamage * classMultiplier, hitProbability, attributeDamage * classMultiplier, statsLowMultiplier, statsHighMultiplier));
}

bool AttackFactory::importConfig(const std::string& path)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file((const char*) path.c_str());

	LOG(DEBUG) << "importing Attack config";

	if (!result) 
	{
		LOG(ERROR) << "Failed parsing " << path;
		return false;
	}
	else 
	{
		pugi::xml_node attacksNode = doc.child("Attacks");

		for (pugi::xml_node attackNode = attacksNode.first_child(); attackNode; attackNode = attackNode.next_sibling())
		{

			const std::string& name = attackNode.child("Name").text().as_string();
			const Attribute::Enum effect = EnumMapper::mapAttribute(attackNode.child("Effect").text().as_string());
			const float classMultiplier = attackNode.child("Class_Multiplier").text().as_float();
			const float statsLowMultiplier = attackNode.child("Stats_Low_Multiplier").text().as_float();
			const float statsHighMultiplier = attackNode.child("Stats_High_Multiplier").text().as_float();
			const float hpDamage = attackNode.child("HP_Damage").text().as_float();
			const float hitProbability = attackNode.child("Hit_Probability").text().as_float();
			const float x = attackNode.child("x").text().as_float();

			if (effect == Attribute::UNKNOWN)
			{
				return false;
			}

			std::shared_ptr<AttackTemplate> attack(new AttackTemplate(name, effect, classMultiplier, statsLowMultiplier, statsHighMultiplier, hpDamage, hitProbability, x));

			objects[attack->getName()] = attack;
		}
	}

	LOG(DEBUG) << "Load result: " << result.description();
	LOG(DEBUG) << "AttackTemplate now contains " << objects.size() << " objects.";

	return true;
}

std::shared_ptr<Room> RoomTemplate::clone(float externMultiplier)
{
	LOG(DEBUG) << "cloning Room...";

	return std::shared_ptr<Room>(new Room(name, description));
}

bool RoomFactory::importConfig(const std::string& path)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file((const char*)path.c_str());

	LOG(DEBUG) << "importing Room config";

	if (!result)
	{
		LOG(ERROR) << "Failed parsing " << path;
		return false;
	}
	else
	{
		pugi::xml_node roomsNode = doc.child("Rooms");

		for (pugi::xml_node roomNode = roomsNode.first_child(); roomNode; roomNode = roomNode.next_sibling())
		{
			const std::string& name = roomNode.child("Name").text().as_string();
			const std::string& description = roomNode.child("Description").text().as_string();

			pugi::xml_node doorPosNode = roomNode.child("Door_Positions").first_child();
			pugi::xml_node monsterNode = roomNode.child("Monster").first_child();
			pugi::xml_node findNode = roomNode.child("Find_Probabilities").first_child();

			const RoomTemplate::DoorPositions doorPositions = 
			{
				doorPosNode.child("N").text().as_bool(),
				doorPosNode.child("E").text().as_bool(),
				doorPosNode.child("S").text().as_bool(),
				doorPosNode.child("W").text().as_bool()
			};
			
			const RoomTemplate::MonsterProbabilities monsterProbabilities =
			{
				monsterNode.child("no").text().as_float(),
				monsterNode.child("easy").text().as_float(),
				monsterNode.child("normal").text().as_float(),
				monsterNode.child("hard").text().as_float()
			};

			const RoomTemplate::ItemProbabilities findProbabilities =
			{
				findNode.child("none").text().as_float(),
				findNode.child("weak").text().as_float(),
				findNode.child("medium").text().as_float(),
				findNode.child("strong").text().as_float()
			};

			const unsigned int monsterCount = roomNode.child("Monster_Count").text().as_uint();
			const float itemMultiplier = roomNode.child("Item_Multiplier").text().as_float();
			const unsigned int itemCount = roomNode.child("Item_Count").text().as_uint();

			std::shared_ptr<RoomTemplate> room(new RoomTemplate(name, description, doorPositions, monsterProbabilities, findProbabilities, monsterCount, itemCount, itemMultiplier));

			objects[room->getName()] = room;
		}
	}

	LOG(DEBUG) << "Load result: " << result.description();
	LOG(DEBUG) << "RoomTemplate now contains " << objects.size() << " objects.";

	return true;
}

PrototypeStorage::PrototypeStorage(const std::string& templatePath_) : 
	templatePath(templatePath_)
{
	successfullyInitialized = PrototypeStorage::initializeTemplates(templatePath);
}

PrototypeStorage::~PrototypeStorage()
{
	/*delete armamentFactory;
	delete monsterFactory;
	delete potionFactory;
	delete weaponFactory;
	delete attackFactory;
	delete roomFactory;*/
}

void PrototypeStorage::testPrintGameObjects()
{
	for (const std::string& objectName : armamentFactory->getObjectNames()) {
		armamentFactory->create(objectName, 1.0f)->debugPrint();
	}
	for (std::string objectName : monsterFactory->getObjectNames()) {
		monsterFactory->create(objectName, 1.0f)->debugPrint();
	}
	for (std::string objectName : potionFactory->getObjectNames()) {
		potionFactory->create(objectName, 1.0f)->debugPrint();
	}
	for (std::string objectName : weaponFactory->getObjectNames()) {
		weaponFactory->create(objectName, 1.0f)->debugPrint();
	}
	for (std::string objectName : attackFactory->getObjectNames()) {
		attackFactory->create(objectName, 1.0f)->debugPrint();
	}
	for (std::string objectName : roomFactory->getObjectNames()) {
		roomFactory->create(objectName, 1.0f)->debugPrint();
	}
}

bool PrototypeStorage::initializeTemplates(const std::string& templatePath)
{
	LOG(INFO) << "initializing templates...";

	armamentFactory.reset(new ArmamentFactory());
	monsterFactory.reset(new MonsterFactory());
	potionFactory.reset(new PotionFactory());
	weaponFactory.reset(new WeaponFactory());
	attackFactory.reset(new AttackFactory());
	roomFactory.reset(new RoomFactory());

	if (!armamentFactory->importConfig(templatePath + "Armaments.xml"))
	{
		return false;
	}
	if (!monsterFactory->importConfig(templatePath + "Monsters.xml"))
	{
		return false;
	}
	if (!potionFactory->importConfig(templatePath + "Potions.xml"))
	{
		return false;
	}
	if (!weaponFactory->importConfig(templatePath + "Weapons.xml"))
	{
		return false;
	}
	if (!attackFactory->importConfig(templatePath + "Attacks.xml"))
	{
		return false;
	}
	if (!roomFactory->importConfig(templatePath + "Rooms.xml"))
	{
		return false;
	}

	typedef std::pair<std::string, ItemType::Enum> itemPair;
	std::list<itemPair> weaklist;
	std::list<itemPair> mediumlist;
	std::list<itemPair> stronglist;

	// obtain all items classified
	std::map<Classes::Enum, std::list<std::string>> armamentsClassified = getArmamentsClassified();
	for (std::string itemName : armamentsClassified.at(Classes::WEAK))
	{
		weaklist.push_back(itemPair(itemName, ItemType::ARMAMENT));
	}
	for (std::string itemName : armamentsClassified.at(Classes::MEDIUM))
	{
		mediumlist.push_back(itemPair(itemName, ItemType::ARMAMENT));
	}
	for (std::string itemName : armamentsClassified.at(Classes::STRONG))
	{
		stronglist.push_back(itemPair(itemName, ItemType::ARMAMENT));
	}

	std::map<Classes::Enum, std::list<std::string>> potionsClassified = getPotionsClassified();
	for (std::string itemName : potionsClassified.at(Classes::WEAK))
	{
		weaklist.push_back(itemPair(itemName, ItemType::POTION));
	}
	for (std::string itemName : potionsClassified.at(Classes::MEDIUM))
	{
		mediumlist.push_back(itemPair(itemName, ItemType::POTION));
	}
	for (std::string itemName : potionsClassified.at(Classes::STRONG))
	{
		stronglist.push_back(itemPair(itemName, ItemType::POTION));
	}

	std::map<Classes::Enum, std::list<std::string>> weaponsClassified = getWeaponsClassified();
	for (std::string itemName : weaponsClassified.at(Classes::WEAK))
	{
		weaklist.push_back(itemPair(itemName, ItemType::WEAPON));
	}
	for (std::string itemName : weaponsClassified.at(Classes::MEDIUM))
	{
		mediumlist.push_back(itemPair(itemName, ItemType::WEAPON));
	}
	for (std::string itemName : weaponsClassified.at(Classes::STRONG))
	{
		stronglist.push_back(itemPair(itemName, ItemType::WEAPON));
	}

	typedef std::pair<Classes::Enum, std::list<std::pair<std::string, ItemType::Enum>>> itemClassListPair;

	itemsClassList.insert(itemClassListPair(Classes::WEAK, weaklist));
	itemsClassList.insert(itemClassListPair(Classes::MEDIUM, mediumlist));
	itemsClassList.insert(itemClassListPair(Classes::STRONG, stronglist));

	LOG(INFO) << "All templates initialized succesfully";

	return true;
}