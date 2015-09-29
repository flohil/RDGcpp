#include "prototypes.hpp"
#include "pugixml.hpp"
#include "settings.hpp"
#include "exceptions.hpp"
#include <iostream>

template <class T>
std::set<std::string> PrototypeTemplate<T>::getObjectNames() 
{

	std::set<std::string> objectNames;

	for (std::map<std::string, std::shared_ptr<T>>::iterator i = objects.begin(); i != objects.end(); ++i)
	{
		std::string objectName = i->first;
		objectNames.insert(objectName);
	}

	std::cout << "Set contains " << objectNames.size() << " names." << std::endl;

	return objectNames;
}

template <class T>
PrototypeTemplate<T>* PrototypeStorage::getPrototypeTemplate()
{
	return new T();
}

template <class T>
PrototypeTemplate<T>::~PrototypeTemplate() {

	for (std::string object : getObjectNames())
	{
		// deallocate PrototypeTemplates in objects map (if no other reference exists) -> shared pointers
		objects.erase(object);
	}

	objects.clear();
}

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
	delete armamentTemplate;
	delete monsterTemplate;
	delete potionTemplate;
	delete weaponTemplate;
	delete attackTemplate;
	delete roomTemplate;
}

void PrototypeStorage::initializeTemplates(std::string templatePath)
{
	std::cout << "initializing templates..." << std::endl;

	armamentTemplate = new Armament();
	monsterTemplate = new Monster();
	potionTemplate = new Potion();
	weaponTemplate = new Weapon();
	attackTemplate = new Attack();
	roomTemplate = new Room();

	try {
		armamentTemplate->importConfig(templatePath + "Armaments.xml");
		monsterTemplate->importConfig(templatePath + "Monsters.xml");
		potionTemplate->importConfig(templatePath + "Potions.xml");
		weaponTemplate->importConfig(templatePath + "Weapons.xml");
		attackTemplate->importConfig(templatePath + "Attacks.xml");
		roomTemplate->importConfig(templatePath + "Rooms.xml");
	}
	catch (ConfigParseException& e)
	{
		throw e;
	}
	
}

Item::~Item()
{

}

Armament* Armament::clone(std::string objectName)
{
	std::cout << "cloning Armament..." << std::endl;

	return NULL;
}

void Armament::importConfig(std::string path)
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

			std::shared_ptr<Armament> armament(new Armament());

			armament->name = armamentNode.child("Name").text().as_string();
			armament->itemClass = EnumMapper::mapClasses(armamentNode.child("Item_Class").text().as_string());
			armament->classMultiplier = armamentNode.child("Class_Multiplier").text().as_float();
			armament->statsLowMultiplier = armamentNode.child("Stats_Low_Multiplier").text().as_float();
			armament->statsHighMultiplier = armamentNode.child("Stats_High_Multiplier").text().as_float();
			armament->armor = armamentNode.child("Armor").text().as_float();
			armament->speed = armamentNode.child("Speed").text().as_float();
			armament->type = armamentNode.child("Type").text().as_string();
			armament->bonus = armamentNode.child("Bonus").text().as_float();
			armament->image = armamentNode.child("Image").text().as_string();

			objects[armament->name] = armament;
		}
	}

	std::cout << "Load result: " << result.description() << std::endl;
	std::cout << "ArmamentTemplate now contains " << objects.size() << " objects." << std::endl;
}

Monster* Monster::clone(std::string objectName)
{
	std::cout << "cloning Monster..." << std::endl;

	return NULL;
}

void Monster::importConfig(std::string path)
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

			std::shared_ptr<Monster> monster(new Monster());

			monster->name = monsterNode.child("Name").text().as_string();
			monster->level = EnumMapper::mapLevel(monsterNode.child("Level").text().as_string());
			monster->classMultiplier = monsterNode.child("Class_Multiplier").text().as_float();
			monster->statsLowMultiplier = monsterNode.child("Stats_Low_Multiplier").text().as_float();
			monster->statsHighMultiplier = monsterNode.child("Stats_High_Multiplier").text().as_float();
			monster->hp = monsterNode.child("HP").text().as_float();
			monster->strength = monsterNode.child("Strength").text().as_float();
			monster->speed = monsterNode.child("Speed").text().as_float();
			monster->accuracy = monsterNode.child("Accuracy").text().as_float();
			monster->killBonusType = EnumMapper::mapAttribute(monsterNode.child("Kill_Bonus_Type").text().as_string());
			monster->killBonusLow = monsterNode.child("Kill_Bonus_Low").text().as_float();
			monster->killBonusHigh = monsterNode.child("Kill_Bonus_High").text().as_float();
			monster->image = monsterNode.child("Image").text().as_string();

			objects[monster->name] = monster;
		}
	}

	std::cout << "Load result: " << result.description() << std::endl;
	std::cout << "MonsterTemplate now contains " << objects.size() << " objects." << std::endl;
}

Potion* Potion::clone(std::string objectName)
{
	std::cout << "cloning Potion..." << std::endl;

	return NULL;
}

void Potion::importConfig(std::string path)
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

			std::shared_ptr<Potion> potion(new Potion());

			potion->name = potionNode.child("Name").text().as_string();
			potion->itemClass = EnumMapper::mapClasses(potionNode.child("Item_Class").text().as_string());
			potion->classMultiplier = potionNode.child("Class_Multiplier").text().as_float();
			potion->statsLowMultiplier = potionNode.child("Stats_Low_Multiplier").text().as_float();
			potion->statsHighMultiplier = potionNode.child("Stats_High_Multiplier").text().as_float();
			potion->description = potionNode.child("Description").text().as_string();
			potion->target = EnumMapper::mapTarget(potionNode.child("Target").text().as_string());
			potion->effect = EnumMapper::mapAttribute(potionNode.child("Effect").text().as_string());
			potion->mode = EnumMapper::mapMode(potionNode.child("Mode").text().as_string());
			potion->strength = potionNode.child("Strength").text().as_float();
			potion->duration = potionNode.child("Duration").text().as_uint();
			potion->image = potionNode.child("Image").text().as_string();

			objects[potion->name] = potion;
		}
	}

	std::cout << "Load result: " << result.description() << std::endl;
	std::cout << "PotionTemplate now contains " << objects.size() << " objects." << std::endl;
}

Weapon* Weapon::clone(std::string objectName)
{
	std::cout << "cloning Weapon..." << std::endl;

	return NULL;
}

void Weapon::importConfig(std::string path)
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

			std::shared_ptr<Weapon> weapon(new Weapon());

			weapon->name = weaponNode.child("Name").text().as_string();
			weapon->itemClass = EnumMapper::mapClasses(weaponNode.child("Item_Class").text().as_string());
			weapon->classMultiplier = weaponNode.child("Class_Multiplier").text().as_float();
			weapon->statsLowMultiplier = weaponNode.child("Stats_Low_Multiplier").text().as_float();
			weapon->statsHighMultiplier = weaponNode.child("Stats_High_Multiplier").text().as_float();
			weapon->attack = weaponNode.child("Attack").text().as_float();
			weapon->speed = weaponNode.child("Speed").text().as_float();
			weapon->accuracy = weaponNode.child("Accuracy").text().as_float();
			weapon->defence = weaponNode.child("Defence").text().as_float();
			weapon->slots = weaponNode.child("Slots").text().as_uint();
			weapon->max = weaponNode.child("Max").text().as_uint();
			weapon->type = weaponNode.child("Type").text().as_string();
			weapon->image = weaponNode.child("Image").text().as_string();

			objects[weapon->name] = weapon;
		}
	}

	std::cout << "Load result: " << result.description() << std::endl;
	std::cout << "WeaponTemplate now contains " << objects.size() << " objects." << std::endl;
}

Attack* Attack::clone(std::string objectName)
{
	std::cout << "cloning Attack..." << std::endl;

	return NULL;
}

void Attack::importConfig(std::string path)
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

			std::shared_ptr<Attack> attack(new Attack());

			attack->name = attackNode.child("Name").text().as_string();
			attack->classMultiplier = attackNode.child("Class_Multiplier").text().as_float();
			attack->statsLowMultiplier = attackNode.child("Stats_Low_Multiplier").text().as_float();
			attack->statsHighMultiplier = attackNode.child("Stats_High_Multiplier").text().as_float();
			attack->hpDamage = attackNode.child("HP_Damage").text().as_float();
			attack->hitProbability = attackNode.child("Hit_Probability").text().as_float();
			attack->effect = EnumMapper::mapAttribute(attackNode.child("Effect").text().as_string());
			attack->x = attackNode.child("x").text().as_float();

			objects[attack->name] = attack;
		}
	}

	std::cout << "Load result: " << result.description() << std::endl;
	std::cout << "AttackTemplate now contains " << objects.size() << " objects." << std::endl;
}

Room* Room::clone(std::string objectName)
{
	std::cout << "cloning Room..." << std::endl;

	return NULL;
}

void Room::importConfig(std::string path)
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

			std::shared_ptr<Room> room(new Room());

			room->name = roomNode.child("Name").text().as_string();
			room->description = roomNode.child("Description").text().as_string();

			for (pugi::xml_node doorPosNode = roomNode.child("Door_Positions").first_child(); doorPosNode; doorPosNode = doorPosNode.next_sibling())
			{
				room->doorPositions[EnumMapper::mapDoorPositions(doorPosNode.name())] = doorPosNode.text().as_bool();
			}

			for (pugi::xml_node monsterNode = roomNode.child("Monster").first_child(); monsterNode; monsterNode = monsterNode.next_sibling())
			{
				room->monsterProbabilities[EnumMapper::mapMonsterProbabilities(monsterNode.name())] = monsterNode.text().as_float();
			}

			room->monsterCount = roomNode.child("Monster_Count").text().as_uint();
			room->itemMultiplier = roomNode.child("Item_Multiplier").text().as_float();
			
			for (pugi::xml_node findNode = roomNode.child("Find_Probabilities").first_child(); findNode; findNode = findNode.next_sibling())
			{
				room->findProbabilities[EnumMapper::mapClasses(findNode.name())] = findNode.text().as_float();
			}
			
			room->itemCount = roomNode.child("Item_Count").text().as_uint();
			room->image = roomNode.child("Image").text().as_string();

			objects[room->name] = room;
		}
	}

	std::cout << "Load result: " << result.description() << std::endl;
	std::cout << "RoomTemplate now contains " << objects.size() << " objects." << std::endl;
}



