#include "prototypes.hpp"
#include "pugixml.hpp"
#include "settings.hpp"
#include "exceptions.hpp"
#include <iostream>
#include <memory>

DoorPositions EnumMapper::mapDoorPositions(std::string input)
{
	if (input == "N")
	{
		return DoorPositions::North;
	}
	else if (input == "E")
	{
		return DoorPositions::East;
	}
	else if (input == "S")
	{
		return DoorPositions::South;
	}
	else if (input == "W")
	{
		return DoorPositions::West;
	}
	else
	{
		throw EnumMappingException(input, "DoorPositions");
	}
}

MonsterProbabilities EnumMapper::mapMonsterProbabilities(std::string input)
{
	if (input == "no")
	{
		return MonsterProbabilities::No;
	}
	else if (input == "easy")
	{
		return MonsterProbabilities::Easy;
	}
	else if (input == "normal")
	{
		return MonsterProbabilities::Normal;
	}
	else if (input == "hard")
	{
		return MonsterProbabilities::Hard;
	}
	else
	{
		throw EnumMappingException(input, "MonsterProbabilities");
	}
}

DifficultyLevel::Level EnumMapper::mapLevel(std::string input)
{
	if (input == "easy")
	{
		return DifficultyLevel::Level::Easy;
	}
	else if (input == "normal")
	{
		return DifficultyLevel::Level::Normal;
	}
	else if (input == "hard")
	{
		return DifficultyLevel::Level::Hard;
	}
	else
	{
		throw EnumMappingException(input, "Level");
	}
}

Classes EnumMapper::mapClasses(std::string input)
{
	if (input == "-" || input == "none")
	{
		return Classes::None;
	}
	else if (input == "weak")
	{
		return Classes::Weak;
	}
	else if (input == "medium")
	{
		return Classes::Medium;
	}
	else if (input == "strong")
	{
		return Classes::Strong;
	}
	else
	{
		throw EnumMappingException(input, "ItemClass");
	}
}

Target EnumMapper::mapTarget(std::string input)
{
	if (input == "self")
	{
		return Target::Self;
	}
	else if (input == "opponent")
	{
		return Target::Opponent;
	}
	else
	{
		throw EnumMappingException(input, "Target");
	}
}

Attribute EnumMapper::mapAttribute(std::string input)
{
	if (input == "hp")
	{
		return Attribute::Hp;
	}
	else if (input == "speed")
	{
		return Attribute::Speed;
	}
	else if (input == "accuracy")
	{
		return Attribute::Accuracy;
	}
	else if (input == "strength")
	{
		return Attribute::Strength;
	}
	else
	{
		throw EnumMappingException(input, "Effect");
	}
}

Mode EnumMapper::mapMode(std::string input)
{
	if (input == "x")
	{
		return Mode::Cure;
	}
	else if (input == "<")
	{
		return Mode::TemporaryDecrease;
	}
	else if (input == "-")
	{
		return Mode::IncrementalDecrease;
	}
	else if (input == ">")
	{
		return Mode::TemporaryIncrease;
	}
	else if (input == "+")
	{
		return Mode::IncrementalIncrease;
	}
	else
	{
		throw EnumMappingException(input, "Mode");
	}
}



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

Item::Item(const std::string name_, const std::string image_, const Classes itemClass_, const float classMultiplier_, const float statsLowMultiplier_, const float statsHighMultiplier_) :
	name(name_), image(image_), itemClass(itemClass_), classMultiplier(classMultiplier_), statsLowMultiplier(statsLowMultiplier_), statsHighMultiplier(statsHighMultiplier_) {

};

Item::~Item()
{

}

Armament::Armament(const std::string name_, const std::string image_, const Classes itemClass_, const float classMultiplier_, const float statsLowMultiplier_, const float statsHighMultiplier_,
	const std::string type_, const float armor_, const float speed_, const float bonus_) :
	type(type_), armor(armor_), speed(speed_), bonus(bonus_), Item(name_, image_, itemClass_, classMultiplier_, statsLowMultiplier_, statsHighMultiplier_)  
{

}

Armament* Armament::clone()
{
	std::cout << "cloning Armament..." << std::endl;

	return new Armament(name, image, itemClass, classMultiplier, statsLowMultiplier, statsHighMultiplier, type, armor, speed, bonus);
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

			std::shared_ptr<Armament> armament(new Armament(name, image, itemClass, classMultiplier, statsLowMultiplier, statsHighMultiplier,
				type, armor, speed, bonus));

			objects[armament->name] = armament;
		}
	}

	std::cout << "Load result: " << result.description() << std::endl;
	std::cout << "ArmamentTemplate now contains " << objects.size() << " objects." << std::endl;
}

Monster::Monster(const std::string name_, const std::string image_, const DifficultyLevel::Level level_, const Attribute killBonusType_,
	const float classMultiplier_, const float statsLowMultiplier_, const float statsHighMultiplier_, const float killBonusLow_, const float killBonusHigh_) :
	name(name_), image(image_), level(level_), killBonusType(killBonusType_), classMultiplier(classMultiplier_), 
	statsLowMultiplier(statsLowMultiplier_), statsHighMultiplier(statsHighMultiplier_), killBonusLow(killBonusLow_), killBonusHigh(killBonusHigh_) {

}

Monster* Monster::clone()
{
	std::cout << "cloning Monster..." << std::endl;

	return NULL;
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

			std::shared_ptr<Monster> monster(new Monster(name, image, level, killBonusType, classMultiplier, statsLowMultiplier, statsHighMultiplier, killBonusLow, killBonusHigh));

			monster->hp = monsterNode.child("HP").text().as_float();
			monster->strength = monsterNode.child("Strength").text().as_float();
			monster->speed = monsterNode.child("Speed").text().as_float();
			monster->accuracy = monsterNode.child("Accuracy").text().as_float();

			objects[monster->name] = monster;
		}
	}

	std::cout << "Load result: " << result.description() << std::endl;
	std::cout << "MonsterTemplate now contains " << objects.size() << " objects." << std::endl;
}

Potion::Potion(const std::string name_, const std::string image_, const Classes itemClass_, const float classMultiplier_, const float statsLowMultiplier_, const float statsHighMultiplier_,
	const std::string description_, const Target target_, const Attribute effect_, const Mode mode_, const float strength_, const unsigned int duration_) : 
	description(description_), target(target_), effect(effect_), mode(mode_), strength(strength_), duration(duration_), Item(name_, image_, itemClass_, classMultiplier_, statsLowMultiplier_, statsHighMultiplier_)
{

};

Potion* Potion::clone()
{
	std::cout << "cloning Potion..." << std::endl;

	return NULL;
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
			const float strength = potionNode.child("Strength").text().as_float();
			const unsigned int duration = potionNode.child("Duration").text().as_uint();
			
			std::shared_ptr<Potion> potion(new Potion(name, image, itemClass, classMultiplier, statsLowMultiplier, statsHighMultiplier,
				description, target, effect, mode, strength, duration));

			objects[potion->name] = potion;
		}
	}

	std::cout << "Load result: " << result.description() << std::endl;
	std::cout << "PotionTemplate now contains " << objects.size() << " objects." << std::endl;
}

Weapon* Weapon::clone()
{
	std::cout << "cloning Weapon..." << std::endl;

	return NULL;
}

Weapon::Weapon(const std::string name_, const std::string image_, const Classes itemClass_, const float classMultiplier_, const float statsLowMultiplier_, const float statsHighMultiplier_,
	const std::string type_, const float attack_, const float speed_, const float accuracy_, const float defence_, const unsigned int slots_, const unsigned int max_) :
	type(type_), attack(attack_), speed(speed_), accuracy(accuracy_), defence(defence_), slots(slots_), max(max_), Item(name_, image_, itemClass_, classMultiplier_, statsLowMultiplier_, statsHighMultiplier_)
{

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
			const std::string type = weaponNode.child("Type").text().as_string();
			const float attack = weaponNode.child("Attack").text().as_float();
			const float speed = weaponNode.child("Speed").text().as_float();
			const float accuracy = weaponNode.child("Accuracy").text().as_float();
			const float defence = weaponNode.child("Defence").text().as_float();
			const unsigned int slots = weaponNode.child("Slots").text().as_uint();
			const unsigned int max = weaponNode.child("Max").text().as_uint();

			std::shared_ptr<Weapon> weapon(new Weapon(name, image, itemClass, classMultiplier, statsLowMultiplier, statsHighMultiplier,
				type, attack, speed, accuracy, defence, slots, max));

			objects[weapon->name] = weapon;
		}
	}

	std::cout << "Load result: " << result.description() << std::endl;
	std::cout << "WeaponTemplate now contains " << objects.size() << " objects." << std::endl;
}

Attack::Attack(const std::string name_, const Attribute effect_, const float classMultiplier_, const float statsLowMultiplier_, const float statsHighMultiplier_,
	const float hpDamage_, const float hitProbability_, const float x_) :
	name(name_), effect(effect_), classMultiplier(classMultiplier_), statsLowMultiplier(statsLowMultiplier_), statsHighMultiplier(statsHighMultiplier_),
	hpDamage(hpDamage_), hitProbability(hitProbability_), x(x_)
{

}

Attack* Attack::clone()
{
	std::cout << "cloning Attack..." << std::endl;

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

			std::shared_ptr<Attack> attack(new Attack(name, effect, classMultiplier, statsLowMultiplier, statsHighMultiplier, hpDamage, hitProbability, x));

			objects[attack->name] = attack;
		}
	}

	std::cout << "Load result: " << result.description() << std::endl;
	std::cout << "AttackTemplate now contains " << objects.size() << " objects." << std::endl;
}

Room::Room(const std::string name_, const std::string description_, const std::string image_,
	const std::map<DoorPositions, const bool> doorPositions_,
	const std::map<MonsterProbabilities, const float> monsterProbabilities_,
	const std::map<Classes, const float> findProbabilities_,
	const unsigned int monsterCount_, const unsigned int itemCount_,
	const float itemMultiplier_) : 
	name(name_), description(description_), image(image_), doorPositions(doorPositions_), monsterProbabilities(monsterProbabilities_),
	findProbabilities(findProbabilities_), monsterCount(monsterCount_), itemCount(itemCount_), itemMultiplier(itemMultiplier_)
{

}

Room* Room::clone()
{
	std::cout << "cloning Room..." << std::endl;

	return NULL;
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

			// std::shared_ptr<Room> room(new Room())

			std::shared_ptr<Room> room(new Room(name, description, image, constDoorPositions, constMonsterProbabilities, constFindProbabilities, monsterCount, itemCount, itemMultiplier));

			objects[room->name] = room;
		}
	}

	std::cout << "Load result: " << result.description() << std::endl;
	std::cout << "RoomTemplate now contains " << objects.size() << " objects." << std::endl;
}