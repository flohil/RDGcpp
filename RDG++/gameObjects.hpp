#ifndef GAME_OBJECTS_INCLUDE
#define GAME_OBJECTS_INCLUDE

#include "debugPrint.hpp"
#include <TGUI/TGUI.hpp>
#include "enums.hpp"
#include <map>
#include <set>
#include <string>
#include <memory>

class Map;
class Weapon;
class Armament;
class Potion;
class Item;
class RenderableObject;
class GameStateGame;

class EquipmentSet
{
private:

	std::shared_ptr<Weapon> primaryWeapon;
	std::shared_ptr<Weapon> secondaryWeapon;
	std::shared_ptr<Armament> helmet;
	std::shared_ptr<Armament> harness;
	std::shared_ptr<Armament> cuisse;
	std::shared_ptr<Armament> gauntlets;
	std::shared_ptr<Armament> boots;
	std::shared_ptr<Potion> potion1;
	std::shared_ptr<Potion> potion2;
	std::shared_ptr<Potion> potion3;
	unsigned int numerator;
	unsigned int itemSize;
	sf::Vector2f armorOffsets;
	sf::Vector2f potionOffsets;

public:

	EquipmentSet(unsigned int numerator_, unsigned int itemSize_, sf::Vector2f armorOffsets_, sf::Vector2f potionOffsets_) :
		primaryWeapon(nullptr), secondaryWeapon(nullptr), helmet(nullptr), harness(nullptr), cuisse(nullptr), gauntlets(nullptr), 
		boots(nullptr), potion1(nullptr), potion2(nullptr), potion3(nullptr), numerator(numerator_), itemSize(itemSize_), armorOffsets(armorOffsets_), potionOffsets(potionOffsets_) {};

	std::shared_ptr<Weapon> getPrimaryWeapon() const { return primaryWeapon; };
	std::shared_ptr<Weapon> getSecondaryWeapon() const { return secondaryWeapon; };
	std::shared_ptr<Armament> getHelmet() const { return helmet; };
	std::shared_ptr<Armament> getHarness() const { return harness; };
	std::shared_ptr<Armament> getCuisse() const { return cuisse; };
	std::shared_ptr<Armament> getGauntlets() const { return gauntlets; };
	std::shared_ptr<Armament> getBoots() const { return boots; };
	std::shared_ptr<Potion> getPotion1() const { return potion1; };
	std::shared_ptr<Potion> getPotion2() const { return potion2; };
	std::shared_ptr<Potion> getPotion3() const { return potion3; };
	unsigned int getNumerator() const { return numerator; };

	std::shared_ptr<Weapon> setPrimaryWeapon(std::shared_ptr<Weapon> weapon_);
	std::shared_ptr<Weapon> setSecondaryWeapon(std::shared_ptr<Weapon> weapon_);
	std::shared_ptr<Armament> setHelmet(std::shared_ptr<Armament> helmet_);
	std::shared_ptr<Armament> setHarness(std::shared_ptr<Armament> harness_);
	std::shared_ptr<Armament> setCuisse(std::shared_ptr<Armament> cuisse_);
	std::shared_ptr<Armament> setGauntlets(std::shared_ptr<Armament> gauntlets_);
	std::shared_ptr<Armament> setBoots(std::shared_ptr<Armament> boots_);
	std::shared_ptr<Potion> setPotion1(std::shared_ptr<Potion> potion_);
	std::shared_ptr<Potion> setPotion2(std::shared_ptr<Potion> potion_);
	std::shared_ptr<Potion> setPotion3(std::shared_ptr<Potion> potion_);

	std::list<std::shared_ptr<RenderableObject>> setItem(std::shared_ptr<Item> obj);

	float getStats(ItemType::Enum, ArmorStatsMode::Enum, ArmorStatsAttributes::Enum);
};

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
	RenderableObject(const std::string& name_, const ObjectType::Enum objectType_, float angle_) : GameObject(name_, objectType_), visible(true),
		size(sf::Vector2f(-1.f, -1.f)), scale(sf::Vector2f(-1.f, -1.f)), rawPos(sf::Vector2f(-1.f, -1.f)), finalPos(sf::Vector2f(-1.f, -1.f))
	{
		obtainSprite(objectType_);
		sf::FloatRect globalBounds = sprite.getGlobalBounds();
		sprite.setOrigin(globalBounds.width / 2, globalBounds.height / 2); // we want to rotate sprites around their center generally
		sprite.setRotation(angle_);
	};

	bool isVisible() const { return visible; };
	void setVisible(const bool visible_) { visible = visible_; };
	void setRotation(const float angle_) { sprite.setRotation(angle_); };
	void setPosition(const sf::Vector2f pos_); 
	void setSize(const unsigned int width, const unsigned int height);
	void setScale(const sf::Vector2f scale_);
	void draw(sf::RenderWindow& window, float deltaTime);
	sf::Vector2f getPosition() const { return finalPos; };
	sf::Vector2f getSize() const { return size; };
	sf::Vector2f getScale() const { return scale; };

protected:

	bool visible;

private:

	bool obtainSprite(ObjectType::Enum objectType);
	sf::Sprite sprite;
	sf::Vector2f size;
	sf::Vector2f scale;
	sf::Vector2f rawPos;
	sf::Vector2f finalPos;
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

	Creature(float hp_, float strength_, float speed_, float accuracy_, CreatureType::Enum creatureType_)
		: orHp(hp_), orStrength(strength_), orSpeed(speed_), orAccuracy(accuracy_), hp(hp_), strength(strength_), speed(speed_), accuracy(accuracy_), creatureType(creatureType_) {};

	float hp, strength, speed, accuracy;

	float getOrHP() { return orHp; };
	float getOrSpeed() { return orSpeed; };
	float getOrAccuracy() { return orAccuracy; };
	float getOrStrength() { return orStrength; };
	virtual DifficultyLevel::Enum getLevel() const = 0;
	void resetOriginals();
	CreatureType::Enum getCreatureType() { return creatureType; };
	std::vector<std::shared_ptr<Potion>> activePotions;
	void removeActivePotions(std::shared_ptr<Potion> potion);
	void addActivePotion(std::shared_ptr<Potion> potion);
	void emptyActivePotions() { activePotions.clear(); };

private:

	float const orHp, orStrength, orSpeed, orAccuracy;
	CreatureType::Enum creatureType;
};

class Player : public RenderableObject, public Creature
{
public:

	Player(const std::string &name_, float hp_, float strength_, float speed_, float accuracy_, const std::string &playerName_, float moveDistance_, unsigned int maxInventorySize_, sf::Vector2f armorOffsets_, sf::Vector2f potionOffsets_) : 
		RenderableObject(name_, ObjectType::CREATURE), Creature(hp_, strength_, speed_, accuracy_, CreatureType::PLAYER), playerName(playerName_), moveDistance(moveDistance_), maxInventorySize(maxInventorySize_), armorOffsets(armorOffsets_), potionOffsets(potionOffsets_) {};

	void init(Map* map_, const unsigned int tileSize_, tgui::ChatBox::Ptr chatBox_);
	void update(const float deltaTime);
	void handleInput(sf::Event event);
	bool putInInventar(std::shared_ptr<RenderableObject> object);
	Point getPlayerPosition() const { return playerPosition; };
	sf::Vector2f getPixelPosition() const { return RenderableObject::getPosition(); };
	DifficultyLevel::Enum getLevel() const { return DifficultyLevel::UNKNOWN; };
	std::shared_ptr<EquipmentSet> getEquipmentSet() const
	{
		if (setOne->getNumerator() == activeSet) return setOne;
		else if (setTwo->getNumerator() == activeSet) return setTwo;
		else return setOne;
	};
	std::string getPlayerName() const { return playerName; };
	void drawInventory(sf::RenderWindow& window, const float deltaTime) const;
	void drawEquipment(sf::RenderWindow& window, const float deltaTime) const;
	void setActiveEquipmentSet(unsigned int numerator) { activeSet = numerator; };

private:

	const std::string playerName;
	const unsigned int maxInventorySize;
	Map* map;
	Point prevPlayerPosition; // in tiles
	Point playerPosition; // in tiles
	sf::Vector2f offset = sf::Vector2f(0.f, 0.f); //used for smooth movement between tiles - in pixels
	float picAngle = 0.f;
	unsigned int tileSize;
	float accumulatedTime = 0;
	const float moveInterval = 0.015f;
	float vFactor = 110.f;
	float velocity = 0.f;
	float toMove = 0.f;
	const unsigned int initialDelaySpan = 5u;
	const unsigned int waitTillStandingSpan = 5u;
	unsigned int initialDelayCtr = 0;
	unsigned int waitTillStandingCtr = 0;
	bool initialWait = true;
	float moveDistance;
	ViewingDirections::Enum movDir = ViewingDirections::UNKNOWN;
	ViewingDirections::Enum lastDir = ViewingDirections::UNKNOWN;
	ViewingDirections::Enum intendedMovDir = ViewingDirections::UNKNOWN;
	ViewingDirections::Enum facingDir = ViewingDirections::N;
	MoveState::Enum moveState = MoveState::RESTING; // make sure a move finishes
	std::vector<std::shared_ptr<RenderableObject>> inventory;
	sf::Vector2f armorOffsets;
	sf::Vector2f potionOffsets;
	std::shared_ptr<EquipmentSet> setOne = std::shared_ptr<EquipmentSet>(new EquipmentSet(1u, tileSize, armorOffsets, potionOffsets));
	std::shared_ptr<EquipmentSet> setTwo = std::shared_ptr<EquipmentSet>(new EquipmentSet(2u, tileSize, armorOffsets, potionOffsets));
	unsigned int activeSet = 1;

	tgui::ChatBox::Ptr chatBox;

	void setPosition(Point position_);
	void preMove();
	void move(const float deltaTime);
	void setPositionInInventory(unsigned int idx, std::shared_ptr<RenderableObject> object);
};

class Armament : public Item, public DebugPrintObject
{
public:

	Armament(const std::string& name_, const Classes::Enum itemClass_, const std::string& material_, const float armor_, const float speed_, const float bonus_, const ArmamentType::Enum armamentType_) :
		Item(name_, ObjectType::ITEM, itemClass_, ItemType::ARMAMENT), material(material_), armamentType(armamentType_), armor(armor_), speed(speed_), bonus(bonus_) {};

	std::string getMaterial() const { return material; };
	ArmamentType::Enum getArmamentType() const { return armamentType; };
	float getArmor() const { return armor; };
	float getSpeed() const { return speed; };
	float getBonus() const { return bonus; };
	virtual void debugPrint() const;

protected:

	const std::string material; // leather, 
	const ArmamentType::Enum armamentType; // helmet, harness, cuisse, gauntlets, boots
	const float armor, speed, bonus;
};

class Monster : public RenderableObject, public Creature, public DebugPrintObject
{
public:

	Monster(const std::string& name_, const DifficultyLevel::Enum level_, const Attribute::Enum killBonusType_, float killBonus_, float hp_, float strength_, float speed_, float accuracy_) :
		RenderableObject(name_, ObjectType::CREATURE), Creature(hp_, strength_, speed_, accuracy_, CreatureType::MONSTER), level(level_), killBonusType(killBonusType_), killBonus(killBonus_) {};

	Attribute::Enum getKillBonusType() const { return killBonusType; };
	float getKillBonus() const { return killBonus; };
	virtual void debugPrint() const;
	DifficultyLevel::Enum getLevel() const { return level; };

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
	void setDuration(unsigned int decrease) { duration = duration - decrease; };
	virtual void debugPrint() const;

protected:

	const std::string description;
	const Target::Enum target;
	const Attribute::Enum effect;
	const Mode::Enum mode;
	const float strength;
	unsigned int duration;
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
