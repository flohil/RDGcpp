#include "prototypes.hpp"
#include <iostream>

template <class T>
std::set<std::string> PrototypeTemplate<T>::getObjectNames() 
{

	std::set<std::string> objectNames;

	for (std::map<std::string, T*>::iterator i = objects.begin(); i != objects.end(); ++i)
	{
		std::string objectName = i->first;
		objectNames.insert(objectName);
	}

	std::cout << "Set contains " << objectNames.size << " names." << std::endl;

	return objectNames;
}

template <class T>
PrototypeTemplate<T>* PrototypeStorage::getPrototypeTemplate()
{
	return new T();
}

PrototypeStorage::PrototypeStorage()
{
	PrototypeStorage::initializeTemplates();
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

void PrototypeStorage::initializeTemplates()
{
	std::cout << "initializing templates..." << std::endl;

	armamentTemplate = new Armament();
	monsterTemplate = new Monster();
	potionTemplate = new Potion();
	weaponTemplate = new Weapon();
	attackTemplate = new Attack();
	roomTemplate = new Room();

	armamentTemplate->importConfig();
	monsterTemplate->importConfig();
	potionTemplate->importConfig();
	weaponTemplate->importConfig();
	attackTemplate->importConfig();
	roomTemplate->importConfig();
}

Item::~Item()
{

}

Armament* Armament::clone(std::string objectName)
{
	std::cout << "cloning Armament..." << std::endl;

	return NULL;
}

void Armament::importConfig()
{
	std::cout << "importing Armament config" << std::endl;
}

Monster* Monster::clone(std::string objectName)
{
	std::cout << "cloning Monster..." << std::endl;

	return NULL;
}

void Monster::importConfig()
{
	std::cout << "importing Monster config" << std::endl;
}

Potion* Potion::clone(std::string objectName)
{
	std::cout << "cloning Potion..." << std::endl;

	return NULL;
}

void Potion::importConfig()
{
	std::cout << "importing Potion config" << std::endl;
}

Weapon* Weapon::clone(std::string objectName)
{
	std::cout << "cloning Weapon..." << std::endl;

	return NULL;
}

void Weapon::importConfig()
{
	std::cout << "importing Weapon config" << std::endl;
}

Attack* Attack::clone(std::string objectName)
{
	std::cout << "cloning Attack..." << std::endl;

	return NULL;
}

void Attack::importConfig()
{
	std::cout << "importing Attack config" << std::endl;
}

Room* Room::clone(std::string objectName)
{
	std::cout << "cloning Room..." << std::endl;

	return NULL;
}

void Room::importConfig()
{
	std::cout << "importing Room config" << std::endl;
}



