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
	T2* create(const std::string& objectName, float externMultiplier);
	std::set<const std::string> getObjectNames();
	std::shared_ptr<PrototypeTemplate<T2>> getTemplate(const std::string& objectName);

protected:

	std::map<const std::string, std::shared_ptr<PrototypeTemplate<T2>>> objects;

private:

	virtual bool importConfig(const std::string& path) = 0;
};

// prototype template classes
class TexturedTemplate
{
public:

	TexturedTemplate(const std::string& textureName_) : textureName(textureName_) {};
	const std::string getTextureName() { return textureName; };

private:

	const std::string textureName;
};


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

class ArmamentTemplate : public Armament, public TexturedTemplate, public VariableTemplate, public PrototypeTemplate<Armament>
{
public:

	ArmamentTemplate(const std::string& name_, const std::string& textureName_, const Classes::Enum itemClass_, const float classMultiplier_, const float statsLowMultiplier_, const float statsHighMultiplier_, const std::string& type_, const float armor_, const float speed_, const float bonus_) :
		Armament(name_, itemClass_, type_, armor_, speed_, bonus_), TexturedTemplate(textureName_), VariableTemplate(classMultiplier_, statsLowMultiplier_, statsHighMultiplier_) {};

	virtual Armament* clone(float externMultiplier);
};

class MonsterTemplate : public Monster, public TexturedTemplate, public VariableTemplate, public PrototypeTemplate<Monster>
{
public:

	MonsterTemplate(const std::string& name_, const std::string& textureName_, const DifficultyLevel::Enum level_, const Attribute::Enum killBonusType_, const float classMultiplier_, const float statsLowMultiplier_, const float statsHighMultiplier_, const float killBonusLow_, const float killBonusHigh_, float hp_, float strength_, float speed_, float accuracy_) :
		Monster(name_, level_, killBonusType_, 0.0f, hp_, strength_, speed_, accuracy_), TexturedTemplate(textureName_), VariableTemplate(classMultiplier_, statsLowMultiplier_, statsHighMultiplier_), killBonusLow(killBonusLow_), killBonusHigh(killBonusHigh_) {};

	virtual Monster* clone(float externMultiplier);

private:

	const float killBonusLow, killBonusHigh;
};

class PotionTemplate : public Potion, public TexturedTemplate, public VariableTemplate, public PrototypeTemplate<Potion>
{
public:

	PotionTemplate(const std::string& name_, const std::string& textureName_, const Classes::Enum itemClass_, const float classMultiplier_, const float statsLowMultiplier_, const float statsHighMultiplier_, const std::string& description_, const Target::Enum target_, const Attribute::Enum effect_, const Mode::Enum mode_, const float strength_, const unsigned int duration_) :
		Potion(name_, itemClass_, description_, target_, effect_, mode_, strength_, duration_), TexturedTemplate(textureName_), VariableTemplate(classMultiplier_, statsLowMultiplier_, statsHighMultiplier_) {};

	virtual Potion* clone(float externMultiplier);
};

class WeaponTemplate : public Weapon, public TexturedTemplate, public VariableTemplate, public PrototypeTemplate<Weapon>
{
public:

	WeaponTemplate(const std::string& name_, const std::string& textureName_, const Classes::Enum itemClass_, const float classMultiplier_, const float statsLowMultiplier_, const float statsHighMultiplier_, const WeaponType::Enum type_, const float attack_, const float speed_, const float accuracy_, const float defence_, const unsigned int slots_, const unsigned int max_) :
		Weapon(name_, itemClass_, type_, attack_, speed_, accuracy_, defence_, slots_, max_), TexturedTemplate(textureName_), VariableTemplate(classMultiplier_, statsLowMultiplier_, statsHighMultiplier_) {};

	virtual Weapon* clone(float externMultiplier);
};

class AttackTemplate : public Attack, public VariableTemplate, public PrototypeTemplate<Attack>
{
public:

	AttackTemplate(const std::string& name_, const Attribute::Enum effect_, const float classMultiplier_, const float statsLowMultiplier_, const float statsHighMultiplier_, const float hpDamage_, const float hitProbability_, const float attributeDamage_) :
		Attack(name_, effect_, 1.0f, hitProbability_, 1.0f, statsLowMultiplier_, statsHighMultiplier_), VariableTemplate(classMultiplier_, statsLowMultiplier_, statsHighMultiplier_), hpDamage(hpDamage_), attributeDamage(attributeDamage_) {};

	virtual Attack* clone(float externMultiplier);

private:

	const float hpDamage;
	const float attributeDamage;
};

class RoomTemplate : public Room, public PrototypeTemplate<Room>
{
public:

	struct DoorPositions
	{
		bool north;
		bool east;
		bool south;
		bool west;
	};

	struct MonsterProbabilities
	{
		float no;
		float easy;
		float normal;
		float hard;
	};

	struct FindProbabilities
	{
		float no;
		float easy;
		float normal;
		float hard;
	};

	RoomTemplate(const std::string& name_, const std::string& description_, const DoorPositions doorPositions_, MonsterProbabilities monsterProbabilities_, FindProbabilities findProbabilities_, const unsigned int monsterCount_, const unsigned int itemCount_, const float itemMultiplier_) :
		Room(name_, description_), doorPositions(doorPositions_), monsterProbabilities(monsterProbabilities_), findProbabilities(findProbabilities_), monsterCount(monsterCount_), itemCount(itemCount_), itemMultiplier(itemMultiplier_) {};

	virtual Room* clone(float externMultiplier);
	Room* clone() { return clone(1.0f); };

private:

	const DoorPositions doorPositions;
	const MonsterProbabilities monsterProbabilities;
	const FindProbabilities findProbabilities;
	const unsigned int monsterCount, itemCount;
	const float itemMultiplier;
};


// prototype template storage classes
class ArmamentFactory : public PrototypeTemplateFactory<ArmamentTemplate, Armament>
{
public:
	virtual bool importConfig(const std::string& path);
};

class MonsterFactory : public PrototypeTemplateFactory<MonsterTemplate, Monster>
{
public:
	virtual bool importConfig(const std::string& path);
};

class PotionFactory : public PrototypeTemplateFactory<PotionTemplate, Potion>
{
public:
	virtual bool importConfig(const std::string& path);
};

class WeaponFactory : public PrototypeTemplateFactory<WeaponTemplate, Weapon>
{
public:
	virtual bool importConfig(const std::string& path);
};

class AttackFactory : public PrototypeTemplateFactory<AttackTemplate, Attack>
{
public:
	virtual bool importConfig(const std::string& path);
};

class RoomFactory : public PrototypeTemplateFactory<RoomTemplate, Room>
{
public:
	virtual bool importConfig(const std::string& path);
};

// manages and controls prototype templates
class PrototypeStorage
{
public:

	// variable declarations
	const std::string templatePath;

	ArmamentFactory* armamentFactory;
	MonsterFactory* monsterFactory;
	PotionFactory* potionFactory;
	WeaponFactory* weaponFactory;
	AttackFactory* attackFactory;
	RoomFactory* roomFactory;

	// function declarations
	PrototypeStorage(const std::string& templatePath);
	~PrototypeStorage();
	void testPrintGameObjects();
	bool initializedSuccessfully() { return successfullyInitialized; };
                                                                                                                           
private:

	bool successfullyInitialized = false;

	// function declarations
	bool initializeTemplates(const std::string& templatePath);
};


// template definitions in header file to avoid linking errors
template <class T>
PrototypeTemplate<T>::~PrototypeTemplate() {

}

template <class T, class T2>
std::set<const std::string> PrototypeTemplateFactory<T, T2>::getObjectNames() 
{

	std::set<const std::string> objectNames;

	for (std::map<const std::string, std::shared_ptr<PrototypeTemplate<T2>>>::iterator i = objects.begin(); i != objects.end(); ++i)
	{
		const std::string objectName = i->first;
		objectNames.insert(objectName);
	}

	std::cout << "Set contains " << objectNames.size() << " names." << std::endl;

	return objectNames;
}

template <class T, class T2>
std::shared_ptr<PrototypeTemplate<T2>> PrototypeTemplateFactory<T, T2>::getTemplate(const std::string& objectName)
{
	return objects.at(objectName);
}

template <class T, class T2>
T2* PrototypeTemplateFactory<T, T2>::create(const std::string& objectName, float externMultiplier)
{
	return objects[objectName]->clone(externMultiplier);
}

template <class T, class T2>
PrototypeTemplateFactory<T, T2>::~PrototypeTemplateFactory() {

	for (const std::string object : getObjectNames())
	{
		// deallocate PrototypeTemplates in objects map (if no other reference exists) -> shared pointers
		objects.erase(object);
	}

	objects.clear();
}

#endif // PROTOTYPES_INCLUDE