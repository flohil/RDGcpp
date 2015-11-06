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
class Monster;
class RenderableObject;
class GameStateGame;
class Fight;

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
	sf::Vector2f armorDims;
	sf::Vector2f potionDims;
	int horSplitAbs;
	int verRightSplitAbs;
	tgui::ChatBox::Ptr chatbox;

	//positions - left, top
	sf::Vector2f primaryWeaponPos;
	sf::Vector2f secondaryWeaponPos;
	sf::Vector2f helmetPos;
	sf::Vector2f harnessPos;
	sf::Vector2f cuissePos;
	sf::Vector2f gauntletsPos;
	sf::Vector2f bootsPos;
	sf::Vector2f potion1Pos;
	sf::Vector2f potion2Pos;
	sf::Vector2f potion3Pos;

	std::list<std::shared_ptr<RenderableObject>> setItem(std::shared_ptr<Item> obj, EquipHotspots::Enum hotspot);

public:

	EquipmentSet(unsigned int numerator_) :
		primaryWeapon(nullptr), secondaryWeapon(nullptr), helmet(nullptr), harness(nullptr), cuisse(nullptr), gauntlets(nullptr), 
		boots(nullptr), potion1(nullptr), potion2(nullptr), potion3(nullptr), numerator(numerator_), itemSize(0u), 
		armorOffsets(sf::Vector2f(0,0)), potionOffsets(sf::Vector2f(0,0)) {};

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

	void setChatbox(tgui::ChatBox::Ptr chatbox_) { chatbox = chatbox_; };
	void setOffsets(sf::Vector2f armorOffsets_, sf::Vector2f potionOffsets_, sf::Vector2f armorDims_, sf::Vector2f potionDims_, int horSplitAbs_, int verRightSplitAbs_);
	void setItemSize(unsigned int itemSize_) { itemSize = itemSize_; };
	std::shared_ptr<RenderableObject> getItemAtPixels(sf::Vector2i pos, bool remove, bool usePotion);
	std::list<std::shared_ptr<RenderableObject>> setItemAtPixels(sf::Vector2i pos, std::shared_ptr<RenderableObject> obj, bool usePotion, std::shared_ptr<Fight> fight);

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

	virtual ~RenderableObject(){};

	bool isVisible() const { return visible; };
	void setVisible(const bool visible_) { visible = visible_; };
	void setRotation(const float angle_) { sprite.setRotation(angle_); };
	void setPosition(const sf::Vector2f pos_); 
	void setCenteredPosition(const sf::Vector2i pos_);
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
		RenderableObject(name_, ObjectType::CREATURE), Creature(hp_, strength_, speed_, accuracy_, CreatureType::PLAYER), playerName(playerName_), moveDistance(moveDistance_), maxInventorySize(maxInventorySize_) {};

	void init(Map* map_, const unsigned int tileSize_, tgui::ChatBox::Ptr chatBox_, int horSplitAbs_, int rightVerSplitAbs_);
	void update(const float deltaTime);
	void handleInput(sf::Event event, std::shared_ptr<RenderableObject> draggedItem);
	std::shared_ptr<RenderableObject> putInInventory(std::shared_ptr<RenderableObject> object) { return putInInventory(object, true); };
	std::shared_ptr<RenderableObject> putInInventory(std::shared_ptr<RenderableObject> object, bool output); // return object if inventory is full
	Point getPlayerPosition() const { return playerPosition; };
	ViewingDirections::Enum getFacingDir() const { return facingDir; };
	sf::Vector2f getPixelPosition() const { return RenderableObject::getPosition(); };
	DifficultyLevel::Enum getLevel() const { return DifficultyLevel::UNKNOWN; };
	std::shared_ptr<EquipmentSet> getEquipmentSet() const
	{
		return activeSet;
	};
	std::string getPlayerName() const { return playerName; };
	void drawInventory(sf::RenderWindow& window, const float deltaTime) const;
	void drawEquipment(sf::RenderWindow& window, const float deltaTime) const;
	void setActiveEquipmentSet(unsigned int numerator) { 
		if (numerator == 1u)
		{
			activeSet = setOne;
		}
		else if (numerator == 2u)
		{
			activeSet = setTwo;
		}
	};
	void setEquipmentOffsets(sf::Vector2f armorOffsets, sf::Vector2f potionOffsets, sf::Vector2f armorDims_, sf::Vector2f potionDims_, int horSplitAbs, int verRightSplitAbs);
	std::shared_ptr<RenderableObject> getArmorItemAtPixels(sf::Vector2i pos) { return activeSet->getItemAtPixels(pos, false, false); };
	std::shared_ptr<RenderableObject> getArmorItemAtPixels(sf::Vector2i pos, bool remove, bool usePotion) { return activeSet->getItemAtPixels(pos, remove, usePotion); };
	std::shared_ptr<RenderableObject> getInventoryItemAtPixels(sf::Vector2i pos) { return getInventoryItemAtPixels(pos, false); };
	std::shared_ptr<RenderableObject> getInventoryItemAtPixels(sf::Vector2i pos, bool remove);
	void setChatbox(tgui::ChatBox::Ptr chatbox_) { activeSet->setChatbox(chatbox_); };
	std::shared_ptr<Monster> getPendingFightEnemy() const { return pendingFightEnemy; };
	void setPendingFightEnemy(std::shared_ptr<Monster> pendingFightEnemy_) { pendingFightEnemy = pendingFightEnemy_; };

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
	std::shared_ptr<EquipmentSet> setOne = std::shared_ptr<EquipmentSet>(new EquipmentSet(1u));
	std::shared_ptr<EquipmentSet> setTwo = std::shared_ptr<EquipmentSet>(new EquipmentSet(2u));
	std::shared_ptr<EquipmentSet> activeSet = setOne;

	float inventoryLeftPadding = 17.f;
	float inventoryTopPadding = 12.f;
	float inventorySpacing = 5.f;
	unsigned int inventoryHorItemCount = 3u;

	int horSplitAbs;
	int verRightSplitAbs;

	tgui::ChatBox::Ptr chatBox;

	std::shared_ptr<Monster> pendingFightEnemy = nullptr;

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
