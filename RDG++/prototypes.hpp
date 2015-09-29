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

// template/interface for all prototype templates
template <class T>
class PrototypeTemplate
{
public:

	PrototypeTemplate::~PrototypeTemplate();
	virtual T* clone(std::string objectName) = 0;
	std::set<std::string> getObjectNames();

protected:

	std::map<std::string, std::shared_ptr<T>> objects;

private:

	virtual void importConfig(std::string path) = 0;
};

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

	std::string name, image;
	Classes itemClass;
	float classMultiplier, statsLowMultiplier, statsHighMultiplier;

	virtual Item::~Item() = 0;
};

class Armament : public Item, public PrototypeTemplate<Armament>
{
public:

	std::string type;
	float armor, speed, bonus;

	Armament* clone(std::string objectName);
	void importConfig(std::string path);
};

class Monster : public PrototypeTemplate<Monster>
{
public:

	std::string name, image;
	DifficultyLevel::Level level;
	Attribute killBonusType;
	float classMultiplier, statsLowMultiplier, statsHighMultiplier, hp, strength, speed, accuracy, killBonusLow, killBonusHigh;

	Monster* clone(std::string objectName);
	void importConfig(std::string path);
};

class Potion : public Item, public PrototypeTemplate<Potion>
{
public:

	std::string description;
	Target target;
	Attribute effect;
	Mode mode;
	float strength;
	unsigned int duration;

	Potion* clone(std::string objectName);
	void importConfig(std::string path);
};

class Weapon : public Item, public PrototypeTemplate<Weapon>
{
public:

	std::string type;
	float attack, speed, accuracy, defence;
	unsigned int slots, max;

	Weapon* clone(std::string objectName);
	void importConfig(std::string path);
};

class Attack : public PrototypeTemplate<Attack>
{
public:

	std::string name;
	Attribute effect;
	float classMultiplier, statsLowMultiplier, statsHighMultiplier, hpDamage, hitProbability, x;

	Attack* clone(std::string objectName);
	void importConfig(std::string path);
};

class Room : public PrototypeTemplate<Room>
{
public:

	std::string name, description, image;
	std::map<DoorPositions, bool> doorPositions;
	std::map<MonsterProbabilities, float> monsterProbabilities;
	std::map<Classes, float> findProbabilities;
	unsigned int monsterCount, itemCount;
	float itemMultiplier;

	Room* clone(std::string objectName);
	void importConfig(std::string path);
};

// manages and controls prototype templates
class PrototypeStorage
{
public:

	// variable declarations
	std::string templatePath;

	Armament* armamentTemplate;
	Monster* monsterTemplate;
	Potion* potionTemplate;
	Weapon* weaponTemplate;
	Attack* attackTemplate;
	Room* roomTemplate;

	// function declarations
	PrototypeStorage(std::string templatePath);
	~PrototypeStorage();
	template <class T>
	PrototypeTemplate<T>* getPrototypeTemplate();

private:

	// function declarations
	void initializeTemplates(std::string templatePath);
};