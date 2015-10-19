#ifndef GAME_OBJECTS_INCLUDE
#define GAME_OBJECTS_INCLUDE

#include "debugPrint.hpp"
#include <SFML\Graphics.hpp>
#include "enums.hpp"
#include <map>
#include <string>
#include <memory>

class Map;

class GameObject
{
public:

	GameObject(const std::string& name_, const ObjectType::Enum objectType_) : name(name_), objectType(objectType_) {};

	const std::string getName() const { return name; };
	const ObjectType::Enum getObjectType() const { return objectType; };

protected:

	const std::string name;
	const ObjectType::Enum objectType;
};

// public sf::Drawable, public sf::Transformable, 
class RenderableObject : public GameObject
{
public:

	RenderableObject(const std::string& name_, const ObjectType::Enum objectType_) : RenderableObject(name_, objectType_, 0.f) {};
	RenderableObject(const std::string& name_, const ObjectType::Enum objectType_, float angle_) : GameObject(name_, objectType_), visible(true)
	{
		obtainSprite(objectType_);
		sf::FloatRect globalBounds = sprite.getGlobalBounds();
		sprite.setOrigin(globalBounds.width / 2, globalBounds.height / 2); // we want to rotate sprites around their center generally
		sprite.setRotation(angle_);
	};

	bool isVisible() const { return visible; };
	void setVisible(const bool visible_) { visible = visible_; };
	void setRotation(const float angle_) { sprite.setRotation(angle_); };
	void setPosition(sf::Vector2f pos_) { sprite.setPosition(pos_.x + 16, pos_.y + 16); };
	void setSize(const unsigned int width, const unsigned int height);
	void setScale(const sf::Vector2f scale_) { sprite.setScale(scale_); };
	void draw(sf::RenderWindow& window, float deltaTime);

protected:

	bool visible;

private:

	bool obtainSprite(ObjectType::Enum objectType);
	sf::Sprite sprite;
};

class Item : public RenderableObject
{
public:

	class Item(const std::string& name_, const ObjectType::Enum objectType_, const Classes::Enum itemClass_, const ItemType::Enum itemType_) : RenderableObject(name_, objectType_), itemClass(itemClass_), itemType(itemType_) {};

	Classes::Enum getItemClass() const { return itemClass; };
	ItemType::Enum getItemType() const { return itemType; };

	virtual ~Item() = 0;

protected:

	const Classes::Enum itemClass;
	const ItemType::Enum itemType;
};

class Creature
{
public:

	Creature(float hp_, float strength_, float speed_, float accuracy_, CreatureType::Enum creatureType_) : hp(hp_), strength(strength_), speed(speed_), accuracy(accuracy_), creatureType(creatureType_) {};

	float hp, strength, speed, accuracy;

	CreatureType::Enum getCreatureType() { return creatureType; };

private:

	CreatureType::Enum creatureType;
};

class Player : public RenderableObject, public Creature
{
public:

	Player(const std::string &name_, float hp_, float strength_, float speed_, float accuracy_, const std::string &playerName_) : 
		RenderableObject(name_, ObjectType::CREATURE), Creature(hp_, strength_, speed_, accuracy_, CreatureType::PLAYER), playerName(playerName_) {};

	void init(Map* map_);
	void update(const float deltaTime);
	void setPosition(Point position_);
	Point getPosition() const { return playerPosition; };
	Point getPrevPosition() const { return prevPlayerPosition; };
	void handleInput(sf::Event event);

private:

	const std::string playerName;
	Map* map;
	Point prevPlayerPosition;
	Point playerPosition;
	ViewingDirections::Enum movingDirection;
	float picAngle = 0.f;
};

class Armament : public Item, public DebugPrintObject
{
public:

	Armament(const std::string& name_, const Classes::Enum itemClass_, const std::string& type_, const float armor_, const float speed_, const float bonus_) :
		Item(name_, ObjectType::ITEM, itemClass_, ItemType::ARMAMENT), type(type_), armor(armor_), speed(speed_), bonus(bonus_) {};

	std::string getType() const { return type; };
	float getArmor() const { return armor; };
	float getSpeed() const { return speed; };
	float getBonus() const { return bonus; };
	virtual void debugPrint() const;

protected:

	const std::string type;
	const float armor, speed, bonus;
};

class Monster : public RenderableObject, public Creature, public DebugPrintObject
{
public:

	Monster(const std::string& name_, const DifficultyLevel::Enum level_, const Attribute::Enum killBonusType_, float killBonus_, float hp_, float strength_, float speed_, float accuracy_) :
		RenderableObject(name_, ObjectType::CREATURE), Creature(hp_, strength_, speed_, accuracy_, CreatureType::MONSTER), level(level_), killBonusType(killBonusType_), killBonus(killBonus_) {};

	DifficultyLevel::Enum getLevel() const { return level; };
	Attribute::Enum getKillBonusType() const { return killBonusType; };
	float getKillBonus() const { return killBonus; };
	virtual void debugPrint() const;

protected:

	const DifficultyLevel::Enum level;
	const Attribute::Enum killBonusType;
	const float killBonus;
};

class Potion : public Item, public DebugPrintObject
{
public:

	Potion(const std::string& name_, const Classes::Enum itemClass_, const std::string& description_, const Target::Enum target_, const Attribute::Enum effect_, const Mode::Enum mode_, const float strength_, const unsigned int duration_) :
		Item(name_, ObjectType::ITEM, itemClass_, ItemType::POTION), description(description_), target(target_), effect(effect_), mode(mode_), strength(strength_), duration(duration_) {};

	std::string getDescription() const { return description; };
	Target::Enum getTarget() const { return target; };
	Attribute::Enum getEffect() const { return effect; };
	Mode::Enum getMode() const { return mode; };
	float getStrength() const { return strength; };
	unsigned int getDuration() const { return duration; };
	virtual void debugPrint() const;

protected:

	const std::string description;
	const Target::Enum target;
	const Attribute::Enum effect;
	const Mode::Enum mode;
	const float strength;
	const unsigned int duration;
};

class Weapon : public Item, public DebugPrintObject
{
public:

	Weapon::Weapon(const std::string& name_, const Classes::Enum itemClass_, const WeaponType::Enum type_, const float attack_, const float speed_, const float accuracy_, const float defence_, const unsigned int slots_, const unsigned int max_) :
		Item(name_, ObjectType::ITEM, itemClass_, ItemType::WEAPON), type(type_), attack(attack_), speed(speed_), accuracy(accuracy_), defence(defence_), slots(slots_), maxWeapons(max_) {};

	WeaponType::Enum getType() const { return type; };
	float getAttack() const { return attack; };
	float getSpeed() const { return speed; };
	float getAccuracy() const { return accuracy; };
	float getDefence() const { return defence; };
	unsigned int getSlots() const { return slots; };
	unsigned int getMaxWeapons() const { return maxWeapons; };
	virtual void debugPrint() const;

protected:

	const WeaponType::Enum type;
	const float attack, speed, accuracy, defence;
	const unsigned int slots, maxWeapons;
};

class Attack : public GameObject, public DebugPrintObject
{
public:

	Attack(const std::string& name_, const Attribute::Enum effect_, const float hpDamageMultiplier_, const float hitProbability_, const float attributeDamageMultiplier_, const float attackStatsLowMultiplier_, const float attackStatsHighMultiplier_) :
		GameObject(name_, ObjectType::ATTACK), effect(effect_), hpDamageMultiplier(hpDamageMultiplier_), hitProbability(hitProbability_), attributeDamageMultiplier(attributeDamageMultiplier_), attackStatsHighMultiplier(attackStatsHighMultiplier_), attackStatsLowMultiplier(attackStatsLowMultiplier_) {};

	Attribute::Enum getEffect() const { return effect; };
	float getHpDamageMultiplier() const { return hpDamageMultiplier; };
	float getHitProbability() const { return hitProbability; };
	float getAttributeDamageMultiplier() const { return attributeDamageMultiplier; };
	float getAttackStatsLowMultiplier() const { return attackStatsLowMultiplier; };
	float getAttackStatsHighMultiplier() const { return attackStatsHighMultiplier; };
	virtual void debugPrint() const;

protected:

	const Attribute::Enum effect;
	const float hpDamageMultiplier, hitProbability, attributeDamageMultiplier;

private:

	const float attackStatsLowMultiplier, attackStatsHighMultiplier;
};

class Room : public GameObject, public DebugPrintObject
{
public:

	Room(const std::string& name_, const std::string& description_) :
		GameObject(name_, ObjectType::ROOM), description(description_) {};

	std::vector<std::vector<std::shared_ptr<RenderableObject>>> background;
	std::vector<std::vector<std::shared_ptr<RenderableObject>>> overlay;

	std::string getDescription() const { return description; };
	void initialize(unsigned int width, unsigned int height);
	virtual void debugPrint() const;

protected:

	const std::string description;
};

#endif // GAME_OBJECTS_INCLUDE