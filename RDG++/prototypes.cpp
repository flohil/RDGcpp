#include "prototypes.hpp"
#include "pugixml.hpp"
#include "settings.hpp"
#include "exceptions.hpp"
#include "calculation.hpp"
#include <iostream>
#include <memory>

PrototypeStorage::PrototypeStorage(std::string templatePath)
{
	try {
		PrototypeStorage::initializeTemplates(templatePath);
	}
	catch (ConfigParseException& e)
	{
		throw e;
	}
}

PrototypeStorage::~PrototypeStorage()
{
	delete armamentFactory;
	delete monsterFactory;
	delete potionFactory;
	delete weaponFactory;
	delete attackFactory;
	delete roomFactory;
}

void PrototypeStorage::testPrintGameObjects()
{
	/*for (std::string objectName : armamentFactory->getObjectNames()) {
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
	}*/
	for (std::string objectName : roomFactory->getObjectNames()) {
		// roomFactory->create(objectName, 1.0f)->debugPrint();
	}
}

void PrototypeStorage::initializeTemplates(std::string templatePath)
{
	std::cout << "initializing templates..." << std::endl;

	armamentFactory = new ArmamentFactory();
	monsterFactory = new MonsterFactory();
	potionFactory = new PotionFactory();
	weaponFactory = new WeaponFactory();
	attackFactory = new AttackFactory();
	roomFactory = new RoomFactory();

	try {
		armamentFactory->importConfig(templatePath + "Armaments.xml");
		monsterFactory->importConfig(templatePath + "Monsters.xml");
		potionFactory->importConfig(templatePath + "Potions.xml");
		weaponFactory->importConfig(templatePath + "Weapons.xml");
		attackFactory->importConfig(templatePath + "Attacks.xml");
		roomFactory->importConfig(templatePath + "Rooms.xml");
	}
	catch (ConfigParseException& e)
	{
		throw e;
	}
}

VariableTemplate::VariableTemplate(const float classMultiplier_, const float statsLowMultiplier_, const float statsHighMultiplier_) :
	classMultiplier(classMultiplier_), statsLowMultiplier(statsLowMultiplier_), statsHighMultiplier(statsHighMultiplier_)
{

};

float VariableTemplate::getClassMultiplier() const
{
	return classMultiplier;
}

float VariableTemplate::getStatsLowMultiplier() const
{
	return statsLowMultiplier;
}

float VariableTemplate::getStatsHighMultiplier() const
{
	return statsHighMultiplier;
}

ArmamentTemplate::ArmamentTemplate(const std::string name_, const std::string image_, const Classes itemClass_, const float classMultiplier_, const float statsLowMultiplier_, const float statsHighMultiplier_,
	const std::string type_, const float armor_, const float speed_, const float bonus_) :
	Armament(name_, image_, itemClass_, type_, armor_, speed_, bonus_), VariableTemplate(classMultiplier_, statsLowMultiplier_, statsHighMultiplier_)
{

}

Armament* ArmamentTemplate::clone(float externMultiplier)
{
	std::cout << "cloning Armament..." << std::endl;

	return new Armament(name, image, itemClass, type, armor * externMultiplier * classMultiplier * Calculation::randomFloat(statsLowMultiplier, statsHighMultiplier), 
		speed * externMultiplier * classMultiplier * Calculation::randomFloat(statsLowMultiplier, statsHighMultiplier), bonus);
}

std::string ArmamentTemplate::getTextureName()
{
	return image;
}

void ArmamentFactory::importConfig(std::string path)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file((const char*)path.c_str());

	std::cout << "importing Armament config" << std::endl;

	if (!result)
	{
		throw ConfigParseException(path);
	}
	else
	{
		pugi::xml_node armamentsNode = doc.child("Armaments");

		for (pugi::xml_node armamentNode = armamentsNode.first_child(); armamentNode; armamentNode = armamentNode.next_sibling())
		{

			const std::string name = armamentNode.child("Name").text().as_string();
			const std::string image = armamentNode.child("Image").text().as_string();
			const Classes itemClass = EnumMapper::mapClasses(armamentNode.child("Item_Class").text().as_string());
			const float classMultiplier = armamentNode.child("Class_Multiplier").text().as_float();
			const float statsLowMultiplier = armamentNode.child("Stats_Low_Multiplier").text().as_float();
			const float statsHighMultiplier = armamentNode.child("Stats_High_Multiplier").text().as_float();
			const std::string type = armamentNode.child("Type").text().as_string();
			const float armor = armamentNode.child("Armor").text().as_float();
			const float speed = armamentNode.child("Speed").text().as_float();
			const float bonus = armamentNode.child("Bonus").text().as_float();

			std::shared_ptr<ArmamentTemplate> armament(new ArmamentTemplate(name, image, itemClass, classMultiplier, statsLowMultiplier, statsHighMultiplier,
				type, armor, speed, bonus));

			objects[armament->getName()] = armament;
		}
	}

	std::cout << "Load result: " << result.description() << std::endl;
	std::cout << "ArmamentTemplate now contains " << objects.size() << " objects." << std::endl;
}

MonsterTemplate::MonsterTemplate(const std::string name_, const std::string image_, const DifficultyLevel::Level level_, const Attribute killBonusType_,
	const float classMultiplier_, const float statsLowMultiplier_, const float statsHighMultiplier_, const float killBonusLow_, const float killBonusHigh_, float hp_, float strength_, float speed_, float accuracy_) :
	killBonusLow(killBonusLow_), killBonusHigh(killBonusHigh_), Monster(name_, image_, level_, killBonusType_, 0.0f, hp_, strength_, speed_, accuracy_), VariableTemplate(classMultiplier_, statsLowMultiplier_, statsHighMultiplier_) 
{

}

Monster* MonsterTemplate::clone(float externMultiplier)
{
	std::cout << "cloning Monster..." << std::endl;

	return new Monster(name, image, level, killBonusType, classMultiplier * Calculation::randomFloat(killBonusLow, killBonusHigh),
		hp * classMultiplier * Calculation::randomFloat(statsLowMultiplier, statsHighMultiplier), strength * classMultiplier * Calculation::randomFloat(statsLowMultiplier, statsHighMultiplier),
		speed * classMultiplier * Calculation::randomFloat(statsLowMultiplier, statsHighMultiplier), accuracy * classMultiplier * Calculation::randomFloat(statsLowMultiplier, statsHighMultiplier));
}

std::string MonsterTemplate::getTextureName()
{
	return image;
}

void MonsterFactory::importConfig(std::string path)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file((const char*)path.c_str());

	std::cout << "importing Monster config" << std::endl;

	if (!result)
	{
		throw ConfigParseException(path);
	}
	else
	{
		pugi::xml_node monstersNode = doc.child("Monsters");

		for (pugi::xml_node monsterNode = monstersNode.first_child(); monsterNode; monsterNode = monsterNode.next_sibling())
		{

			const std::string name = monsterNode.child("Name").text().as_string();
			const std::string image = monsterNode.child("Image").text().as_string();
			const DifficultyLevel::Level level = EnumMapper::mapLevel(monsterNode.child("Level").text().as_string());
			const Attribute killBonusType = EnumMapper::mapAttribute(monsterNode.child("Kill_Bonus_Type").text().as_string());
			const float classMultiplier = monsterNode.child("Class_Multiplier").text().as_float();
			const float statsLowMultiplier = monsterNode.child("Stats_Low_Multiplier").text().as_float();
			const float statsHighMultiplier = monsterNode.child("Stats_High_Multiplier").text().as_float();
			const float killBonusLow = monsterNode.child("Kill_Bonus_Low").text().as_float();
			const float killBonusHigh = monsterNode.child("Kill_Bonus_High").text().as_float();
			float hp = monsterNode.child("HP").text().as_float();
			float strength = monsterNode.child("Strength").text().as_float();
			float speed = monsterNode.child("Speed").text().as_float();
			float accuracy = monsterNode.child("Accuracy").text().as_float();

			std::shared_ptr<MonsterTemplate> monster(new MonsterTemplate(name, image, level, killBonusType, classMultiplier, statsLowMultiplier, statsHighMultiplier, killBonusLow, killBonusHigh, hp, strength, speed, accuracy));

			objects[monster->getName()] = monster;
		}
	}

	std::cout << "Load result: " << result.description() << std::endl;
	std::cout << "MonsterTemplate now contains " << objects.size() << " objects." << std::endl;
}

PotionTemplate::PotionTemplate(const std::string name_, const std::string image_, const Classes itemClass_, const float classMultiplier_, const float statsLowMultiplier_, const float statsHighMultiplier_,
	const std::string description_, const Target target_, const Attribute effect_, const Mode mode_, const float strength_, const unsigned int duration_) : 
	Potion(name_, image_, itemClass_, description_, target_, effect_, mode_, strength_, duration_), VariableTemplate(classMultiplier_, statsLowMultiplier_, statsHighMultiplier_)
{

};

Potion* PotionTemplate::clone(float externMultiplier)
{
	std::cout << "cloning Potion..." << std::endl;

	return new Potion(name, image, itemClass, description, target, effect, mode, strength * externMultiplier * classMultiplier * Calculation::randomFloat(statsLowMultiplier, statsHighMultiplier), duration);
}

std::string PotionTemplate::getTextureName()
{
	return image;
}

void PotionFactory::importConfig(std::string path)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file((const char*)path.c_str());

	std::cout << "importing Potion config" << std::endl;

	if (!result)
	{
		throw ConfigParseException(path);
	}
	else
	{
		pugi::xml_node potionsNode = doc.child("Potions");

		for (pugi::xml_node potionNode = potionsNode.first_child(); potionNode; potionNode = potionNode.next_sibling())
		{
			const std::string name = potionNode.child("Name").text().as_string();
			const std::string image = potionNode.child("Image").text().as_string();
			const Classes itemClass = EnumMapper::mapClasses(potionNode.child("Item_Class").text().as_string());
			const float classMultiplier = potionNode.child("Class_Multiplier").text().as_float();
			const float statsLowMultiplier = potionNode.child("Stats_Low_Multiplier").text().as_float();
			const float statsHighMultiplier = potionNode.child("Stats_High_Multiplier").text().as_float();
			const std::string description = potionNode.child("Description").text().as_string();
			const Target target = EnumMapper::mapTarget(potionNode.child("Target").text().as_string());
			const Attribute effect = EnumMapper::mapAttribute(potionNode.child("Effect").text().as_string());
			const Mode mode = EnumMapper::mapMode(potionNode.child("Mode").text().as_string());
			const float strength = potionNode.child("x").text().as_float();
			const unsigned int duration = potionNode.child("n").text().as_uint();
			
			std::shared_ptr<PotionTemplate> potion(new PotionTemplate(name, image, itemClass, classMultiplier, statsLowMultiplier, statsHighMultiplier,
				description, target, effect, mode, strength, duration));

			objects[potion->getName()] = potion;
		}
	}

	std::cout << "Load result: " << result.description() << std::endl;
	std::cout << "PotionTemplate now contains " << objects.size() << " objects." << std::endl;
}

Weapon* WeaponTemplate::clone(float externMultiplier)
{
	std::cout << "cloning Weapon..." << std::endl;

	return new Weapon(name, image, itemClass, type, attack * externMultiplier * classMultiplier * Calculation::randomFloat(statsLowMultiplier, statsHighMultiplier),
		speed * externMultiplier * classMultiplier * Calculation::randomFloat(statsLowMultiplier, statsHighMultiplier),
		accuracy * externMultiplier * classMultiplier * Calculation::randomFloat(statsLowMultiplier, statsHighMultiplier),
		defence * externMultiplier * classMultiplier * Calculation::randomFloat(statsLowMultiplier, statsHighMultiplier), slots, max);
}

WeaponTemplate::WeaponTemplate(const std::string name_, const std::string image_, const Classes itemClass_, const float classMultiplier_, const float statsLowMultiplier_, const float statsHighMultiplier_,
	const WeaponType type_, const float attack_, const float speed_, const float accuracy_, const float defence_, const unsigned int slots_, const unsigned int max_) :
	Weapon(name_, image_, itemClass_, type_, attack_, speed_, accuracy_, defence_, slots_, max_), VariableTemplate(classMultiplier_, statsLowMultiplier_, statsHighMultiplier_)
{

}

std::string WeaponTemplate::getTextureName()
{
	return image;
}

void WeaponFactory::importConfig(std::string path)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file((const char*) path.c_str());

	std::cout << "importing Weapon config" << std::endl;

	if (!result)
	{
		throw ConfigParseException(path);
	}
	else
	{
		pugi::xml_node weaponsNode = doc.child("Weapons");

		for (pugi::xml_node weaponNode = weaponsNode.first_child(); weaponNode; weaponNode = weaponNode.next_sibling())
		{

			const std::string name = weaponNode.child("Name").text().as_string();
			const std::string image = weaponNode.child("Image").text().as_string();
			const Classes itemClass = EnumMapper::mapClasses(weaponNode.child("Item_Class").text().as_string());
			const float classMultiplier = weaponNode.child("Class_Multiplier").text().as_float();
			const float statsLowMultiplier = weaponNode.child("Stats_Low_Multiplier").text().as_float();
			const float statsHighMultiplier = weaponNode.child("Stats_High_Multiplier").text().as_float();
			const WeaponType type = EnumMapper::mapWeaponType(weaponNode.child("Type").text().as_string());
			const float attack = weaponNode.child("Attack").text().as_float();
			const float speed = weaponNode.child("Speed").text().as_float();
			const float accuracy = weaponNode.child("Accuracy").text().as_float();
			const float defence = weaponNode.child("Defence").text().as_float();
			const unsigned int slots = weaponNode.child("Slots").text().as_uint();
			const unsigned int max = weaponNode.child("Max").text().as_uint();

			std::shared_ptr<WeaponTemplate> weapon(new WeaponTemplate(name, image, itemClass, classMultiplier, statsLowMultiplier, statsHighMultiplier,
				type, attack, speed, accuracy, defence, slots, max));

			objects[weapon->getName()] = weapon;
		}
	}

	std::cout << "Load result: " << result.description() << std::endl;
	std::cout << "WeaponTemplate now contains " << objects.size() << " objects." << std::endl;
}

AttackTemplate::AttackTemplate(const std::string name_, const Attribute effect_, const float classMultiplier_, const float statsLowMultiplier_, const float statsHighMultiplier_,
	const float hpDamage_, const float hitProbability_, const float attributeDamage_) :
	hpDamage(hpDamage_), attributeDamage(attributeDamage_), Attack(name_, effect_, 1.0f, hitProbability_, 1.0f, statsLowMultiplier_, statsHighMultiplier_), VariableTemplate(classMultiplier_, statsLowMultiplier_, statsHighMultiplier_)
{

}

Attack* AttackTemplate::clone(float externMultiplier)
{
	std::cout << "cloning Attack..." << std::endl;

	return new Attack(name, effect, hpDamage * classMultiplier, hitProbability, attributeDamage * classMultiplier, statsLowMultiplier, statsHighMultiplier);
}

std::string AttackTemplate::getTextureName()
{
	return NULL;
}

void AttackFactory::importConfig(std::string path)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file((const char*) path.c_str());

	std::cout << "importing Attack config" << std::endl;

	if (!result) 
	{
		throw ConfigParseException(path);
	}
	else 
	{
		pugi::xml_node attacksNode = doc.child("Attacks");

		for (pugi::xml_node attackNode = attacksNode.first_child(); attackNode; attackNode = attackNode.next_sibling())
		{

			const std::string name = attackNode.child("Name").text().as_string();
			const Attribute effect = EnumMapper::mapAttribute(attackNode.child("Effect").text().as_string());
			const float classMultiplier = attackNode.child("Class_Multiplier").text().as_float();
			const float statsLowMultiplier = attackNode.child("Stats_Low_Multiplier").text().as_float();
			const float statsHighMultiplier = attackNode.child("Stats_High_Multiplier").text().as_float();
			const float hpDamage = attackNode.child("HP_Damage").text().as_float();
			const float hitProbability = attackNode.child("Hit_Probability").text().as_float();
			const float x = attackNode.child("x").text().as_float();

			std::shared_ptr<AttackTemplate> attack(new AttackTemplate(name, effect, classMultiplier, statsLowMultiplier, statsHighMultiplier, hpDamage, hitProbability, x));

			objects[attack->getName()] = attack;
		}
	}

	std::cout << "Load result: " << result.description() << std::endl;
	std::cout << "AttackTemplate now contains " << objects.size() << " objects." << std::endl;
}

RoomTemplate::RoomTemplate(const std::string name_, const std::string description_, const std::string image_,
	const std::map<DoorPositions, const bool> doorPositions_,
	const std::map<MonsterProbabilities, const float> monsterProbabilities_,
	const std::map<Classes, const float> findProbabilities_,
	const unsigned int monsterCount_, const unsigned int itemCount_,
	const float itemMultiplier_) :
	doorPositions(doorPositions_), monsterProbabilities(monsterProbabilities_), findProbabilities(findProbabilities_), monsterCount(monsterCount_), 
	itemCount(itemCount_), itemMultiplier(itemMultiplier_), Room(name_, description_, image_)
{

}

Room* RoomTemplate::clone()
{
	return clone(1.0f);
}

Room* RoomTemplate::clone(float externMultiplier)
{
	std::cout << "cloning Room..." << std::endl;

	return NULL;
}

std::string RoomTemplate::getTextureName()
{
	return image;
}

void RoomFactory::importConfig(std::string path)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file((const char*)path.c_str());

	std::cout << "importing Room config" << std::endl;

	if (!result)
	{
		throw ConfigParseException(path);
	}
	else
	{
		pugi::xml_node roomsNode = doc.child("Rooms");

		for (pugi::xml_node roomNode = roomsNode.first_child(); roomNode; roomNode = roomNode.next_sibling())
		{
			const std::string name = roomNode.child("Name").text().as_string();
			const std::string image = roomNode.child("Image").text().as_string();
			const std::string description = roomNode.child("Description").text().as_string();

			std::map<DoorPositions, const bool> doorPositions;
			for (pugi::xml_node doorPosNode = roomNode.child("Door_Positions").first_child(); doorPosNode; doorPosNode = doorPosNode.next_sibling())
			{
				doorPositions.insert(std::pair<DoorPositions, const bool>(EnumMapper::mapDoorPositions(doorPosNode.name()), doorPosNode.text().as_bool()));
			}
			const std::map<DoorPositions, const bool> constDoorPositions = doorPositions;

			std::map<MonsterProbabilities, const float> monsterProbabilities;
			for (pugi::xml_node monsterNode = roomNode.child("Monster").first_child(); monsterNode; monsterNode = monsterNode.next_sibling())
			{
				monsterProbabilities.insert(std::pair<MonsterProbabilities, const float>(EnumMapper::mapMonsterProbabilities(monsterNode.name()), monsterNode.text().as_float()));
			}
			const std::map<MonsterProbabilities, const float> constMonsterProbabilities = monsterProbabilities;

			const unsigned int monsterCount = roomNode.child("Monster_Count").text().as_uint();
			const float itemMultiplier = roomNode.child("Item_Multiplier").text().as_float();
			
			std::map<Classes, const float> findProbabilities;
			for (pugi::xml_node findNode = roomNode.child("Find_Probabilities").first_child(); findNode; findNode = findNode.next_sibling())
			{
				findProbabilities.insert(std::pair<Classes, const float>(EnumMapper::mapClasses(findNode.name()), findNode.text().as_float()));
			}
			const std::map<Classes, const float> constFindProbabilities;
			
			const unsigned int itemCount = roomNode.child("Item_Count").text().as_uint();

			std::shared_ptr<RoomTemplate> room(new RoomTemplate(name, description, image, constDoorPositions, constMonsterProbabilities, constFindProbabilities, monsterCount, itemCount, itemMultiplier));

			objects[room->getName()] = room;
		}
	}

	std::cout << "Load result: " << result.description() << std::endl;
	std::cout << "RoomTemplate now contains " << objects.size() << " objects." << std::endl;
}