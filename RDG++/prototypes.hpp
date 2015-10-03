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
	virtual T* clone(float externMultiplier) = 0;
};

// handles the different prototype templates for one object class 
// - decouple from prototype templates because object classes need const variables in constructors
template <class T, class T2>
class PrototypeTemplateFactory
{
public:

	PrototypeTemplateFactory::~PrototypeTemplateFactory();
	T2* PrototypeTemplateFactory<T, T2>::create(std::string objectName, float externMultiplier);
	std::set<std::string> getObjectNames();

protected:

	std::map<std::string, std::shared_ptr<PrototypeTemplate<T2>>> objects;

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

protected:

	const float classMultiplier, statsLowMultiplier, statsHighMultiplier;
};

class ArmamentTemplate : public Armament, public VariableTemplate, public PrototypeTemplate<Armament>
{
public:

	ArmamentTemplate(const std::string name, const std::string image, const Classes itemClass, const float classMultiplier, const float statsLowMultiplier, const float statsHighMultiplier, 
		const std::string type, const float armor, const float speed, const float bonus);

	virtual Armament* clone(float externMultiplier);
};

class MonsterTemplate : public Monster, public VariableTemplate, public PrototypeTemplate<Monster>
{
public:

	MonsterTemplate(const std::string name, const std::string image, const DifficultyLevel::Level level, const Attribute killBonusType,
		const float classMultiplier, const float statsLowMultiplier, const float statsHighMultiplier, const float killBonusLow, const float killBonusHigh, float hp, float strength, float speed, float accuracy);

	virtual Monster* clone(float externMultiplier);

private:

	const float killBonusLow, killBonusHigh;
};

class PotionTemplate : public Potion, public VariableTemplate, public PrototypeTemplate<Potion>
{
public:

	PotionTemplate(const std::string name, const std::string image, const Classes itemClass, const float classMultiplier, const float statsLowMultiplier, const float statsHighMultiplier, 
		const std::string description, const Target target, const Attribute effect, const Mode mode, const float strength, const unsigned int duration);

	virtual Potion* clone(float externMultiplier);
};

class WeaponTemplate : public Weapon, public VariableTemplate, public PrototypeTemplate<Weapon>
{
public:

	WeaponTemplate(const std::string name, const std::string image, const Classes itemClass, const float classMultiplier, const float statsLowMultiplier, const float statsHighMultiplier, 
		const std::string type, const float attack, const float speed, const float accuracy, const float defence, const unsigned int slots, const unsigned int max);

	virtual Weapon* clone(float externMultiplier);
};

class AttackTemplate : public Attack, public VariableTemplate, public PrototypeTemplate<Attack>
{
public:

	AttackTemplate(const std::string name, const Attribute effect, const float classMultiplier, const float statsLowMultiplier, const float statsHighMultiplier,
		const float hpDamage, const float hitProbability, const float attributeDamage);

	virtual Attack* clone(float externMultiplier);

private:

	const float hpDamage;
	const float attributeDamage;
};

class RoomTemplate : public Room, public PrototypeTemplate<Room>
{
public:

	RoomTemplate(const std::string name, const std::string description, const std::string image,
		const std::map<DoorPositions, const bool> doorPositions,
		const std::map<MonsterProbabilities, const float> monsterProbabilities,
		const std::map<Classes, const float> findProbabilities,
		const unsigned int monsterCount, const unsigned int itemCount,
		const float itemMultiplier);

	virtual Room* clone(float externMultiplier);
	Room* clone();

	/*std::map<DoorPositions, const bool> getDoorPositions() const;
	std::map<MonsterProbabilities, const float> getMonsterProbabilities() const;
	std::map<Classes, const float> getFindProbabilities() const;
	unsigned int getMonsterCount() const;
	unsigned int getItemCount() const;
	float getItemMultiplier() const;*/

private:

	const std::string image;
	const std::map<DoorPositions, const bool> doorPositions;
	const std::map<MonsterProbabilities, const float> monsterProbabilities;
	const std::map<Classes, const float> findProbabilities;
	const unsigned int monsterCount, itemCount;
	const float itemMultiplier;
};


// prototype template storage classes
class ArmamentFactory : public PrototypeTemplateFactory<ArmamentTemplate, Armament>
{
public:
	virtual void importConfig(std::string path);
};

class MonsterFactory : public PrototypeTemplateFactory<MonsterTemplate, Monster>
{
public:
	virtual void importConfig(std::string path);
};

class PotionFactory : public PrototypeTemplateFactory<PotionTemplate, Potion>
{
public:
	virtual void importConfig(std::string path);
};

class WeaponFactory : public PrototypeTemplateFactory<WeaponTemplate, Weapon>
{
public:
	virtual void importConfig(std::string path);
};

class AttackFactory : public PrototypeTemplateFactory<AttackTemplate, Attack>
{
public:
	virtual void importConfig(std::string path);
};

class RoomFactory : public PrototypeTemplateFactory<RoomTemplate, Room>
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
	void testPrintGameObjects();
                                                                                                                           
private:

	// function declarations
	void initializeTemplates(std::string templatePath);
};


// template definitions in header file to avoid linking errors
template <class T>
PrototypeTemplate<T>::~PrototypeTemplate() {

}

template <class T, class T2>
std::set<std::string> PrototypeTemplateFactory<T, T2>::getObjectNames() 
{

	std::set<std::string> objectNames;

	for (std::map<std::string, std::shared_ptr<PrototypeTemplate<T2>>>::iterator i = objects.begin(); i != objects.end(); ++i)
	{
		std::string objectName = i->first;
		objectNames.insert(objectName);
	}

	std::cout << "Set contains " << objectNames.size() << " names." << std::endl;

	return objectNames;
}

template <class T, class T2>
T2* PrototypeTemplateFactory<T, T2>::create(std::string objectName, float externMultiplier)
{
	return objects[objectName]->clone(externMultiplier);
}

template <class T, class T2>
PrototypeTemplateFactory<T, T2>::~PrototypeTemplateFactory() {

	for (std::string object : getObjectNames())
	{
		// deallocate PrototypeTemplates in objects map (if no other reference exists) -> shared pointers
		objects.erase(object);
	}

	objects.clear();
}

#endif // PROTOTYPES_INCLUDE