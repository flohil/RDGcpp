#include <map>
#include <set>

enum DoorPositions { North, East, South, West };
enum MonsterProbabilities { No, Easy, Normal, Hard };
enum FindProbabilities { None, Weak, Medium, Strong };

// template/interface for all prototype templates
template <class T>
class PrototypeTemplate
{
public:

	virtual T* clone(std::string objectName) = 0;
	std::set<std::string> getObjectNames();

private:

	std::map<std::string, T*> objects;
	virtual void importConfig() = 0;
};

// prototype template classes
class Item
{
public:

	std::string name, itemClass, image;
	float classMultiplier, statsLowMultiplier, statsHighMultiplier;

	virtual Item::~Item() = 0;
};

class Armament : public Item, public PrototypeTemplate<Armament>
{
public:

	float armor, speed, bonus;

	Armament* clone(std::string objectName);
	void importConfig();
};

class Monster : public PrototypeTemplate<Monster>
{
public:

	std::string name, level, killBonusType, image;
	float classMultiplier, statsLowMultiplier, statsHighMultiplier, hp, strength, speed, accuracy, killBonusLow, killBonusHigh;

	Monster* clone(std::string objectName);
	void importConfig();
};

class Potion : public Item, public PrototypeTemplate<Potion>
{
public:

	std::string description, target, effect, mode;
	float strength;
	unsigned int duration;

	Potion* clone(std::string objectName);
	void importConfig();
};

class Weapon : public Item, public PrototypeTemplate<Weapon>
{
public:

	std::string type;
	float attack, accuracy, defence;
	unsigned int slots, max;

	Weapon* clone(std::string objectName);
	void importConfig();
};

class Attack : public PrototypeTemplate<Attack>
{
public:

	std::string name, effect;
	float classMultiplier, statsLowMultiplier, statsHighMultiplier, hpDamage, hitProbability, x;

	Attack* clone(std::string objectName);
	void importConfig();
};

class Room : public PrototypeTemplate<Room>
{
public:

	std::string name, description, image;
	std::map<DoorPositions, bool> doorPositions;
	std::map<MonsterProbabilities, float> monsterProbabilities;
	std::map<FindProbabilities, float> findProbabilities;
	unsigned int monsterCount, itemCount;
	float itemMultiplier;

	Room* clone(std::string objectName);
	void importConfig();
};

// manages and controls prototype templates
class PrototypeStorage
{
public:

	// variable declarations
	Armament* armamentTemplate;
	Monster* monsterTemplate;
	Potion* potionTemplate;
	Weapon* weaponTemplate;
	Attack* attackTemplate;
	Room* roomTemplate;

	// function declarations
	PrototypeStorage();
	~PrototypeStorage();
	template <class T>
	PrototypeTemplate<T>* getPrototypeTemplate();

private:

	// function declarations
	void initializeTemplates();
};