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
	virtual std::string getTextureName() const = 0;
};

// handles the different prototype templates for one object class 
// - decouple from prototype templates because object classes need const variables in constructors
template <class T, class T2>
class PrototypeTemplateFactory
{
public:

	PrototypeTemplateFactory::~PrototypeTemplateFactory();
	T2* create(std::string objectName, float externMultiplier);
	std::set<std::string> getObjectNames();
	std::shared_ptr<PrototypeTemplate<T2>> getTemplate(std::string objectName);

protected:

	std::map<std::string, std::shared_ptr<PrototypeTemplate<T2>>> objects;

private:

	virtual void importConfig(std::string path) = 0;
};

// prototype template classes
class VariableTemplate
{
public:

	VariableTemplate(const float classMultiplier_, const float statsLowMultiplier_, const float statsHighMultiplier_) :
		classMultiplier(classMultiplier_), statsLowMultiplier(statsLowMultiplier_), statsHighMultiplier(statsHighMultiplier_) {};

	float getClassMultiplier() const { return classMultiplier; };
	float getStatsLowMultiplier() const { return statsLowMultiplier; };
	float getStatsHighMultiplier() const { return statsHighMultiplier; };

protected:

	const float classMultiplier, statsLowMultiplier, statsHighMultiplier;
};

class ArmamentTemplate : public Armament, public VariableTemplate, public PrototypeTemplate<Armament>
{
public:

	ArmamentTemplate(const std::string name_, const std::string image_, const Classes itemClass_, const float classMultiplier_, const float statsLowMultiplier_, const float statsHighMultiplier_, const std::string type_, const float armor_, const float speed_, const float bonus_) :
		Armament(name_, image_, itemClass_, type_, armor_, speed_, bonus_), VariableTemplate(classMultiplier_, statsLowMultiplier_, statsHighMultiplier_) {};

	virtual Armament* clone(float externMultiplier);
	virtual std::string getTextureName() const { return image; };
};

class MonsterTemplate : public Monster, public VariableTemplate, public PrototypeTemplate<Monster>
{
public:

	MonsterTemplate(const std::string name_, const std::string image_, const DifficultyLevel::Level level_, const Attribute killBonusType_, const float classMultiplier_, const float statsLowMultiplier_, const float statsHighMultiplier_, const float killBonusLow_, const float killBonusHigh_, float hp_, float strength_, float speed_, float accuracy_) :
		killBonusLow(killBonusLow_), killBonusHigh(killBonusHigh_), Monster(name_, image_, level_, killBonusType_, 0.0f, hp_, strength_, speed_, accuracy_), VariableTemplate(classMultiplier_, statsLowMultiplier_, statsHighMultiplier_) {};

	virtual Monster* clone(float externMultiplier);
	virtual std::string getTextureName() const { return image; };

private:

	const float killBonusLow, killBonusHigh;
};

class PotionTemplate : public Potion, public VariableTemplate, public PrototypeTemplate<Potion>
{
public:

	PotionTemplate(const std::string name_, const std::string image_, const Classes itemClass_, const float classMultiplier_, const float statsLowMultiplier_, const float statsHighMultiplier_, const std::string description_, const Target target_, const Attribute effect_, const Mode mode_, const float strength_, const unsigned int duration_) :
		Potion(name_, image_, itemClass_, description_, target_, effect_, mode_, strength_, duration_), VariableTemplate(classMultiplier_, statsLowMultiplier_, statsHighMultiplier_) {};

	virtual Potion* clone(float externMultiplier);
	virtual std::string getTextureName() const { return image; };
};

class WeaponTemplate : public Weapon, public VariableTemplate, public PrototypeTemplate<Weapon>
{
public:

	WeaponTemplate(const std::string name_, const std::string image_, const Classes itemClass_, const float classMultiplier_, const float statsLowMultiplier_, const float statsHighMultiplier_, const WeaponType type_, const float attack_, const float speed_, const float accuracy_, const float defence_, const unsigned int slots_, const unsigned int max_) :
		Weapon(name_, image_, itemClass_, type_, attack_, speed_, accuracy_, defence_, slots_, max_), VariableTemplate(classMultiplier_, statsLowMultiplier_, statsHighMultiplier_) {};

	virtual Weapon* clone(float externMultiplier);
	virtual std::string getTextureName() const { return image; };
};

class AttackTemplate : public Attack, public VariableTemplate, public PrototypeTemplate<Attack>
{
public:

	AttackTemplate(const std::string name_, const Attribute effect_, const float classMultiplier_, const float statsLowMultiplier_, const float statsHighMultiplier_, const float hpDamage_, const float hitProbability_, const float attributeDamage_) :
		hpDamage(hpDamage_), attributeDamage(attributeDamage_), Attack(name_, effect_, 1.0f, hitProbability_, 1.0f, statsLowMultiplier_, statsHighMultiplier_), VariableTemplate(classMultiplier_, statsLowMultiplier_, statsHighMultiplier_) {};

	virtual Attack* clone(float externMultiplier);
	virtual std::string getTextureName() const { return NULL; };

private:

	const float hpDamage;
	const float attributeDamage;
};

class RoomTemplate : public Room, public PrototypeTemplate<Room>
{
public:

	RoomTemplate(const std::string name_, const std::string description_, const std::string image_,
		const std::map<DoorPositions, const bool> doorPositions_,
		const std::map<MonsterProbabilities, const float> monsterProbabilities_,
		const std::map<Classes, const float> findProbabilities_,
		const unsigned int monsterCount_, const unsigned int itemCount_,
		const float itemMultiplier_) :
		doorPositions(doorPositions_), monsterProbabilities(monsterProbabilities_), findProbabilities(findProbabilities_), monsterCount(monsterCount_),
		itemCount(itemCount_), itemMultiplier(itemMultiplier_), Room(name_, description_, image_) {};

	virtual Room* clone(float externMultiplier);
	Room* clone() { return clone(1.0f); };
	virtual std::string getTextureName() const { return image; };

	/*std::map<DoorPositions, const bool> getDoorPositions() const;
	std::map<MonsterProbabilities, const float> getMonsterProbabilities() const;
	std::map<Classes, const float> getFindProbabilities() const;
	unsigned int getMonsterCount() const;
	unsigned int getItemCount() const;
	float getItemMultiplier() const;*/

private:

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
std::shared_ptr<PrototypeTemplate<T2>> PrototypeTemplateFactory<T, T2>::getTemplate(std::string objectName)
{
	return objects.at(objectName);
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