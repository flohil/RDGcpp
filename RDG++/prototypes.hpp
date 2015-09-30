#include <map>
#include <set>
#include <memory>

enum DoorPositions { North, East, South, West };
enum MonsterProbabilities { No, Easy, Normal, Hard };
namespace DifficultyLevel
{
	enum Level { Easy, Normal, Hard };
}
enum Classes { None, Weak, Medium, Strong };
enum Target { Self, Opponent };
enum Attribute { Hp, Speed, Accuracy, Strength };
enum Mode { Cure, TemporaryDecrease, IncrementalDecrease, TemporaryIncrease, IncrementalIncrease };

// templates are defined in header to avoid linker errors

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

// maps strings to enums
class EnumMapper
{
public:

	static DoorPositions mapDoorPositions(std::string input);
	static MonsterProbabilities mapMonsterProbabilities(std::string input);
	static DifficultyLevel::Level mapLevel(std::string input);
	static Classes mapClasses(std::string input);
	static Target mapTarget(std::string input);
	static Attribute mapAttribute(std::string input);
	static Mode mapMode(std::string input);
};


// prototype template classes
class Item
{
public:

	Item(const std::string name, const std::string image, const Classes itemClass, const float classMultiplier, const float statsLowMultiplier, const float statsHighMultiplier);

	const std::string name, image;
	const Classes itemClass;
	const float classMultiplier, statsLowMultiplier, statsHighMultiplier;

	virtual Item::~Item() = 0;
};

class Armament : public Item, public PrototypeTemplate<Armament>
{
public:

	Armament(const std::string name, const std::string image, const Classes itemClass, const float classMultiplier, const float statsLowMultiplier, const float statsHighMultiplier, 
		const std::string type, const float armor, const float speed, const float bonus);

	const std::string type;
	const float armor, speed, bonus;

	virtual Armament* clone();
};

class Monster : public PrototypeTemplate<Monster>
{
public:

	Monster(const std::string name, const std::string image, const DifficultyLevel::Level level, const Attribute killBonusType,
		const float classMultiplier, const float statsLowMultiplier, const float statsHighMultiplier, const float killBonusLow, const float killBonusHigh);

	const std::string name, image;
	const DifficultyLevel::Level level;
	const Attribute killBonusType;
	const float classMultiplier, statsLowMultiplier, statsHighMultiplier, killBonusLow, killBonusHigh;
	float hp, strength, speed, accuracy;

	virtual Monster* clone();
};

class Potion : public Item, public PrototypeTemplate<Potion>
{
public:

	Potion(const std::string name, const std::string image, const Classes itemClass, const float classMultiplier, const float statsLowMultiplier, const float statsHighMultiplier, 
		const std::string description, const Target target, const Attribute effect, const Mode mode, const float strength, const unsigned int duration);

	const std::string description;
	const Target target;
	const Attribute effect;
	const Mode mode;
	const float strength;
	const unsigned int duration;

	virtual Potion* clone();
};

class Weapon : public Item, public PrototypeTemplate<Weapon>
{
public:

	Weapon(const std::string name, const std::string image, const Classes itemClass, const float classMultiplier, const float statsLowMultiplier, const float statsHighMultiplier, 
		const std::string type, const float attack, const float speed, const float accuracy, const float defence, const unsigned int slots, const unsigned int max);

	const std::string type;
	const float attack, speed, accuracy, defence;
	const unsigned int slots, max;

	virtual Weapon* clone();
};

class Attack : public PrototypeTemplate<Attack>
{
public:

	Attack(const std::string name, const Attribute effect, const float classMultiplier, const float statsLowMultiplier, const float statsHighMultiplier,
		const float hpDamage, const float hitProbability, const float x);

	const std::string name;
	const Attribute effect;
	const float classMultiplier, statsLowMultiplier, statsHighMultiplier, hpDamage, hitProbability, x;

	virtual Attack* clone();
};

class Room : public PrototypeTemplate<Room>
{
public:

	Room(const std::string name, const std::string description, const std::string image,
		const std::map<DoorPositions, const bool> doorPositions,
		const std::map<MonsterProbabilities, const float> monsterProbabilities,
		const std::map<Classes, const float> findProbabilities,
		const unsigned int monsterCount, const unsigned int itemCount,
		const float itemMultiplier);

	const std::string name, description, image;
	const std::map<DoorPositions, const bool> doorPositions;
	const std::map<MonsterProbabilities, const float> monsterProbabilities;
	const std::map<Classes, const float> findProbabilities;
	const unsigned int monsterCount, itemCount;
	const float itemMultiplier;

	virtual Room* clone();
};


// prototype template storage classes
class ArmamentFactory : public PrototypeTemplateFactory<Armament>
{
public:
	virtual void importConfig(std::string path);
};

class MonsterFactory : public PrototypeTemplateFactory<Monster>
{
public:
	virtual void importConfig(std::string path);
};

class PotionFactory : public PrototypeTemplateFactory<Potion>
{
public:
	virtual void importConfig(std::string path);
};

class WeaponFactory : public PrototypeTemplateFactory<Weapon>
{
public:
	virtual void importConfig(std::string path);
};

class AttackFactory : public PrototypeTemplateFactory<Attack>
{
public:
	virtual void importConfig(std::string path);
};

class RoomFactory : public PrototypeTemplateFactory<Room>
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