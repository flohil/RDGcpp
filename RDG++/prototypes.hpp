#ifndef PROTOTYPES_INCLUDE
#define PROTOTYPES_INCLUDE

#include <map>
#include <set>
#include <memory>
#include "enums.hpp"
#include "gameObjects.hpp"

// templates are defined in header to avoid linker errors

// prototypes for actual game objects

// template/interface for all prototype templates
template <class T>
class PrototypeTemplate
{
public:

	PrototypeTemplate::~PrototypeTemplate();
	virtual T* clone() = 0;
};

// handles the different prototype templates for one object class 
// - decouple from prototype templates because object classes need const variables in constructors
template <class T>
class PrototypeTemplateFactory
{
public:

	PrototypeTemplateFactory::~PrototypeTemplateFactory();
	T* PrototypeTemplateFactory<T>::create(std::string objectName);
	std::set<std::string> getObjectNames();

protected:

	std::map<std::string, std::shared_ptr<PrototypeTemplate<T>>> objects;

private:

	virtual void importConfig(std::string path) = 0;
};

// prototype template classes
class VariableTemplate
{
public:

	VariableTemplate(const float classMultiplier, const float statsLowMultiplier, const float statsHighMultiplier);

	float getClassMultiplier() const;
	float getStatsLowMultiplier() const;
	float getStatsHighMultiplier() const;

private:

	const float classMultiplier, statsLowMultiplier, statsHighMultiplier;
};

class ArmamentTemplate : public Armament, public VariableTemplate, public PrototypeTemplate<ArmamentTemplate>
{
public:

	ArmamentTemplate(const std::string name, const std::string image, const Classes itemClass, const float classMultiplier, const float statsLowMultiplier, const float statsHighMultiplier, 
		const std::string type, const float armor, const float speed, const float bonus);

	virtual ArmamentTemplate* clone();
};

class MonsterTemplate : public Monster, public VariableTemplate, public PrototypeTemplate<MonsterTemplate>
{
public:

	MonsterTemplate(const std::string name, const std::string image, const DifficultyLevel::Level level, const Attribute killBonusType,
		const float classMultiplier, const float statsLowMultiplier, const float statsHighMultiplier, const float killBonusLow, const float killBonusHigh, float hp, float strength, float speed, float accuracy);

	virtual MonsterTemplate* clone();
};

class PotionTemplate : public Potion, public VariableTemplate, public PrototypeTemplate<PotionTemplate>
{
public:

	PotionTemplate(const std::string name, const std::string image, const Classes itemClass, const float classMultiplier, const float statsLowMultiplier, const float statsHighMultiplier, 
		const std::string description, const Target target, const Attribute effect, const Mode mode, const float strength, const unsigned int duration);

	virtual PotionTemplate* clone();
};

class WeaponTemplate : public Weapon, public VariableTemplate, public PrototypeTemplate<WeaponTemplate>
{
public:

	WeaponTemplate(const std::string name, const std::string image, const Classes itemClass, const float classMultiplier, const float statsLowMultiplier, const float statsHighMultiplier, 
		const std::string type, const float attack, const float speed, const float accuracy, const float defence, const unsigned int slots, const unsigned int max);

	virtual WeaponTemplate* clone();
};

class AttackTemplate : public Attack, public VariableTemplate, public PrototypeTemplate<AttackTemplate>
{
public:

	AttackTemplate(const std::string name, const Attribute effect, const float classMultiplier, const float statsLowMultiplier, const float statsHighMultiplier,
		const float hpDamage, const float hitProbability, const float x);

	virtual AttackTemplate* clone();
};

class RoomTemplate : public Room, public PrototypeTemplate<RoomTemplate>
{
public:

	RoomTemplate(const std::string name, const std::string description, const std::string image,
		const std::map<DoorPositions, const bool> doorPositions,
		const std::map<MonsterProbabilities, const float> monsterProbabilities,
		const std::map<Classes, const float> findProbabilities,
		const unsigned int monsterCount, const unsigned int itemCount,
		const float itemMultiplier);

	virtual RoomTemplate* clone();
};


// prototype template storage classes
class ArmamentFactory : public PrototypeTemplateFactory<ArmamentTemplate>
{
public:
	virtual void importConfig(std::string path);
};

class MonsterFactory : public PrototypeTemplateFactory<MonsterTemplate>
{
public:
	virtual void importConfig(std::string path);
};

class PotionFactory : public PrototypeTemplateFactory<PotionTemplate>
{
public:
	virtual void importConfig(std::string path);
};

class WeaponFactory : public PrototypeTemplateFactory<WeaponTemplate>
{
public:
	virtual void importConfig(std::string path);
};

class AttackFactory : public PrototypeTemplateFactory<AttackTemplate>
{
public:
	virtual void importConfig(std::string path);
};

class RoomFactory : public PrototypeTemplateFactory<RoomTemplate>
{
public:
	virtual void importConfig(std::string path);
};

// manages and controls prototype templates
class PrototypeStorage
{
public:

	// variable declarations
	std::string templatePath;

	ArmamentFactory* armamentFactory;
	MonsterFactory* monsterFactory;
	PotionFactory* potionFactory;
	WeaponFactory* weaponFactory;
	AttackFactory* attackFactory;
	RoomFactory* roomFactory;

	// function declarations
	PrototypeStorage(std::string templatePath);
	~PrototypeStorage();
                                                                                                                           
private:

	// function declarations
	void initializeTemplates(std::string templatePath);
};


// template definitions in header file to avoid linking errors
template <class T>
PrototypeTemplate<T>::~PrototypeTemplate() {

}

template <class T>
std::set<std::string> PrototypeTemplateFactory<T>::getObjectNames() 
{

	std::set<std::string> objectNames;

	for (std::map<std::string, std::shared_ptr<PrototypeTemplate<T>>>::iterator i = objects.begin(); i != objects.end(); ++i)
	{
		std::string objectName = i->first;
		objectNames.insert(objectName);
	}

	std::cout << "Set contains " << objectNames.size() << " names." << std::endl;

	return objectNames;
}

template <class T>
T* PrototypeTemplateFactory<T>::create(std::string objectName)
{
	return objects[objectName]->clone(
		);
}

template <class T>
PrototypeTemplateFactory<T>::~PrototypeTemplateFactory() {

	for (std::string object : getObjectNames())
	{
		// deallocate PrototypeTemplates in objects map (if no other reference exists) -> shared pointers
		objects.erase(object);
	}

	objects.clear();
}

#endif // PROTOTYPES_INCLUDE